//
// Created by melissaa on 2022-04-30.
//

/**
 * Unreal Glue
 * This handles all interactions with the Unreal Engine.
 * It's pretty much just a fancy SDLLaunch with some extra hacks in there to get it booting the editor code-path.
 */


#include "Core.h"
#include "Engine.h"
#include "EditorPrivate.h"

#include "UnrealGlue.h"

#include <sys/stat.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_syswm.h>
#include <map>

#include "../helpers/SDLHelper.h"

// Very rude of X11 to define Success
#ifdef Success
#undef Success
#endif

TCHAR GPackageInternal[64] = TEXT("MultiEd");
extern "C" { const TCHAR *GPackage = GPackageInternal; }

// Memory allocator.
#include "FMallocAnsi.h"

FMallocAnsi Malloc;

// Log file.
#include "FOutputDeviceFile.h"

FOutputDeviceFile Log;

// Error handler.
#include "../FOutputDeviceSDLError.h"

FOutputDeviceSDLError Error;

// Feedback.
#include "../FFeedbackContextSDL.h"

FFeedbackContextSDL Warn;

// File manager.
#include "FFileManagerLinux.h"

FFileManagerLinux FileManager;

// Config.
#include "FConfigCacheIni.h"

#include "../components/LogWindow/LogWindow.h"

enum eLASTDIR {
    eLASTDIR_UNR   = 0,
    eLASTDIR_UTX   = 1,
    eLASTDIR_PCX   = 2,
    eLASTDIR_UAX   = 3,
    eLASTDIR_WAV   = 4,
    eLASTDIR_BRUSH = 5,
    eLASTDIR_2DS   = 6,
    eLASTDIR_USM   = 7,
    eLASTDIR_UMX   = 8,
    eLASTDIR_MUS   = 9,
    eLASTDIR_MAX   = 10
};

const INT s_DefaultViewportFlags =
              SHOW_Frame
              //| SHOW_Menu
              | SHOW_Actors
              | SHOW_Brush // Crashes right now
              | SHOW_StandardView
              //| SHOW_ChildWindow // We don't have a parent window that uses Unreal events
              | SHOW_MovingBrushes
              | SHOW_Coords
              // SDLDrv doesn't handle non-realtime viewports, so just make them all realtime for now.
              //| SHOW_PlayerCtrl // Let's the viewport update
              | SHOW_RealTime;

FString GLastDir[eLASTDIR_MAX];

extern FString GLastText;
#if __STATIC_LINK
extern FString GMapExt;
#else
EDITOR_API FString GMapExt;
#endif

//class qtLogWindow;
class LogWindow : public FOutputDevice {
public:
    FOutputDevice           *AuxOut;
    Components::qtLogWindow *LogWin;
    //wxLogWindow* LogWin;
    //FramePos* MyFramePos;
    UBOOL ShowLog;

    LogWindow()
        : AuxOut(nullptr), LogWin(nullptr)
        //, MyFramePos( NULL )
        , ShowLog(FALSE) {}

    void Serialize(const TCHAR *V, EName Event) {
        guard(LogWindow::Serialize) ;
                if (LogWin) {
                    LogWin->Log(V);
                    //wxLogMessage(wxT("%s"), V);
                }
                if (AuxOut) {
                    AuxOut->Serialize(V, Event);
                }
        unguard;
    }
};

LogWindow g_LogWindow;

/**
 * Exit handler
 * Helps us track down some obscure bugs, and clean up!
 */
static void sdl_atexit_handler(void) {
    static bool already_called = false;

    // Don't log here!
    if (!already_called) {
        already_called = true;
        SDL_Quit();
    }
}

/**
 * Boot UE similar to how SDLLaunch might.
 * @param argc
 * @param argv
 */
void UnrealGlue::Boot(int argc, char **argv) {
    // For FString?
    Malloc.Init();
    GMalloc = &Malloc;

    if (!appChdirSystem()) {
        wprintf(TEXT("WARNING: Could not chdir into the game's System folder\n"));
    }

    debugf(TEXT("Booting UnrealGlue"));

    // Boot up SDL2, if we haven't already
    if (!SDL_WasInit(SDL_INIT_VIDEO)) {
        if (SDL_Init(SDL_INIT_VIDEO) != 0) {
            appErrorf(TEXT("SDL_Init failed: %s"), SDL_GetError());
        }
    } else {
        debugf(TEXT("SDL is already init!"));
    }

    guard(load_unreal)
            try {
                // Get the command line.
                FString  CmdLine;
                for (INT i = 1; i < argc; i++) {
                    if (i > 1)
                        CmdLine += TEXT(" ");
                    CmdLine += ANSI_TO_TCHAR(argv[i]);
                }

                GDoCompatibilityChecks  = !ParseParam(*CmdLine, TEXT("bytehax")) && !ParseParam(*CmdLine, TEXT("nocompat"));
                GFixCompatibilityIssues = ParseParam(*CmdLine, TEXT("fixcompat"));
                GIsScriptable           = 0;

                appStrncpy(GModule, TEXT("MultiEd"), ARRAY_COUNT(GModule));

                // Set the package name.
                appStrncpy(const_cast<TCHAR *>(GPackage), appPackage(), 64);

                // Create a fully qualified pathname for the log file.  If we don't do this, pieces of the log file
                // tends to get written into various directories as the editor starts up.
                FString TmpFilename = appBaseDir();
                FString TmpLogName;

                if (Parse(*CmdLine, TEXT("LOG="), TmpLogName))
                    TmpFilename += TmpLogName;
                else if (Parse(*CmdLine, TEXT("ABSLOG="), TmpLogName))
                    TmpFilename = TmpLogName;
                else
                    TmpFilename += TEXT("MultiEd.log");

                Log.Filename = TmpFilename;

                GIsGuarded = 1;
                GIsStarted    = 1;
                GIsClient     = GIsServer = GIsEditor = GLazyLoad = 1;
                GIsScriptable = 0;

                atexit(sdl_atexit_handler);

                appInit(TEXT("MultiEd"), *CmdLine, &Malloc, &Log, &Error, &Warn, &FileManager, FConfigCacheIni::Factory, 1);

                {
                    // Init Logger
                    g_LogWindow = LogWindow();
                    g_LogWindow.LogWin = new Components::qtLogWindow();
                    g_LogWindow.LogWin->Show();
                    g_LogWindow.AuxOut = GLog;
                    GLog = &g_LogWindow;
                }

                FString sUseGammaRampVal;
                GConfig->GetString(TEXT("OpenGLDrv.OpenGLRenderDevice"), TEXT("ColorCorrectionMode"), sUseGammaRampVal, TEXT("MultiEd.ini"));

                // FIXME: Hack because other ColourCorrectionMode's in OpenGLDrv are broken for othro views.
                if (sUseGammaRampVal != TEXT("UseGammaRamp")) {
                    // Force GammaRamp
                    GConfig->SetString(TEXT("OpenGLDrv.OpenGLRenderDevice"), TEXT("ColorCorrectionMode"), TEXT("UseGammaRamp"), TEXT("MultiEd.ini"));
                    warnf(TEXT("OpenGLDrv Setting Conflict With MultiEd: Due to a bug work-around with OpenGLDrv. ColorCorrectionMode has been set to UseGammaRamp. However this will only apply on the next load. Please re-launch MultiEd for this to take effect."));
                }

                debugf(NAME_Init, TEXT("Booting MultiEd's Unreal integration"));

                // Init console log.
                if (ParseParam(*CmdLine, TEXT("LOG"))) {
                    Warn.AuxOut = GLog;
                    GLog = &Warn;
                }

                // Init engine.
                auto pEngine = InitEngine();
                GEditor = CastChecked<UEditorEngine>(pEngine);

                {
                    // Initialize "last dir" array
                    if (!GConfig->GetString(TEXT("Directories"), TEXT("PCX"), GLastDir[eLASTDIR_PCX], GUEDIni)) GLastDir[eLASTDIR_PCX]       = TEXT("..\\Textures");
                    if (!GConfig->GetString(TEXT("Directories"), TEXT("WAV"), GLastDir[eLASTDIR_WAV], GUEDIni)) GLastDir[eLASTDIR_WAV]       = TEXT("..\\Sounds");
                    if (!GConfig->GetString(TEXT("Directories"), TEXT("BRUSH"), GLastDir[eLASTDIR_BRUSH], GUEDIni)) GLastDir[eLASTDIR_BRUSH] = TEXT("..\\Maps");
                    if (!GConfig->GetString(TEXT("Directories"), TEXT("2DS"), GLastDir[eLASTDIR_2DS], GUEDIni)) GLastDir[eLASTDIR_2DS]       = TEXT("..\\Maps");
#if ENGINE_VERSION == 227
                    if (!GConfig->GetString(TEXT("Directories"), TEXT("USM"), GLastDir[eLASTDIR_USM], GUEDIni))		GLastDir[eLASTDIR_USM] = TEXT("..\\Meshes");
#else
                    if (!GConfig->GetString(TEXT("Directories"), TEXT("USM"), GLastDir[eLASTDIR_USM], GUEDIni)) GLastDir[eLASTDIR_USM] = TEXT("..\\System");
#endif
                    if (!GConfig->GetString(TEXT("Directories"), TEXT("UMX"), GLastDir[eLASTDIR_UMX], GUEDIni)) GLastDir[eLASTDIR_UMX] = TEXT("..\\Music");
                    if (!GConfig->GetString(TEXT("Directories"), TEXT("UAX"), GLastDir[eLASTDIR_UAX], GUEDIni)) GLastDir[eLASTDIR_UAX] = TEXT("..\\Sounds");
                    if (!GConfig->GetString(TEXT("Directories"), TEXT("UTX"), GLastDir[eLASTDIR_UTX], GUEDIni)) GLastDir[eLASTDIR_UTX] = TEXT("..\\Textures");
                    if (!GConfig->GetString(TEXT("Directories"), TEXT("UNR"), GLastDir[eLASTDIR_UNR], GUEDIni)) GLastDir[eLASTDIR_UNR] = TEXT("..\\Maps");

                    if (!GConfig->GetString(TEXT("URL"), TEXT("MapExt"), GMapExt, SYSTEM_INI)) GMapExt = TEXT("unr");
                    GEditor->Exec(*(FString::Printf(TEXT("MODE MAPEXT=%ls"), *GMapExt)));
                }

                // Init input.
                UInput::StaticInitInput();

                BeforeMapLoad();
                MakeViewports();
                AfterMapLoad();

                return;

            }
            catch (...) {
                // Chained abort.  Do cleanup.
                Error.HandleError();
            }

    unguard;
}

/**
 * Initialize and create the editor engine object.
 * @return UEngine
 */
UEngine *UnrealGlue::InitEngine() {
    guard(InitEngine) ;
            DOUBLE LoadTime = appSecondsNew();

            // Set exec hook.
            GExec = nullptr;

            // Update first-run.
            INT FirstRun = 0;
            if (FirstRun < ENGINE_VERSION)
                FirstRun = ENGINE_VERSION;
            GConfig->SetInt(TEXT("FirstRun"), TEXT("FirstRun"), FirstRun);

            //FixIni();  // check for legacy and Windows-specific INI entries...

            // Create the global engine object.
            UClass *EngineClass;
            if (!GIsEditor) {
                // Create game engine.
                EngineClass = UObject::StaticLoadClass(UGameEngine::StaticClass(), nullptr, TEXT("ini:Engine.Engine.GameEngine"), nullptr, LOAD_NoFail, nullptr);
            } else {
                // Editor.
                EngineClass = UObject::StaticLoadClass(UEngine::StaticClass(), nullptr, TEXT("ini:Engine.Engine.EditorEngine"), nullptr, LOAD_NoFail, nullptr);
            }

            UEngine *Engine = ConstructObject<UEngine>(EngineClass);
            Engine->Init();

            debugf(TEXT("Startup time: %f seconds."), appSecondsNew() - LoadTime);

            return Engine;
    unguard;
}

struct MainLoopArgs {
    DOUBLE  OldTime         = 0.0;
    DOUBLE  SecondStartTime = 0.0;
    INT     TickCount       = 0;
    UEngine *Engine         = nullptr;
    bool    Init            = false;
};

/**
 * Unreal Glue Event Loop
 * Let the engine have some tick time.
 * @return
 */
bool UnrealGlue::Loop() {
    guard(MainLoop) ;
            check(GEditor);

            // Loop while running.
            GIsRunning = 1;
            static auto *args = new MainLoopArgs;
            if (!args->Init) {
                args->OldTime         = appSecondsNew();
                args->SecondStartTime = args->OldTime;
                args->TickCount       = 0;
                args->Engine          = GEditor;
                args->Init            = true;
            }

            if (!GIsRunning || GIsRequestingExit) {
                GIsRunning = 0;
                return false;
            }

            try {
                DOUBLE  &OldTime         = args->OldTime;
                DOUBLE  &SecondStartTime = args->SecondStartTime;
                INT     &TickCount       = args->TickCount;
                UEngine *Engine          = args->Engine;

                // Update the world.
                guard(UpdateWorld) ;
                        // If we have no viewports we have no actors. Don't tick yet because someone might be expecting an actor!
                        if (m_pViewports.empty()) {
                            return true;
                        }

                        DOUBLE NewTime   = appSecondsNew();
                        FLOAT  DeltaTime = NewTime - OldTime;
                        Engine->Tick(DeltaTime);

                        if (GWindowManager) {
                            GWindowManager->Tick(DeltaTime);
                        }

                        OldTime = NewTime;
                        TickCount++;
                        if (OldTime > SecondStartTime + 1) {
                            Engine->CurrentTickRate = (FLOAT) TickCount / (OldTime - SecondStartTime);
                            SecondStartTime = OldTime;
                            TickCount       = 0;
                        }
                unguard;
            }
            catch (...) {
                Error.HandleError();
                //raise(SIGUSR1);
            }

            return true;
    unguard;
}

/**
 * Create Viewport
 * Spawns a view actor, and creates a editor viewport
 * @param Name
 * @param RenMap
 * @param Flags
 * @return
 */
UViewport *UnrealGlue::CreateViewport(const wchar_t *Name, int RenMap, int Flags) {
    auto pViewport = GEditor->Client->NewViewport(Name);
    check(pViewport);


    GEditor->Level->SpawnViewActor(pViewport);
    pViewport->Input->Init(pViewport);

    check(pViewport->Actor);

    pViewport->Actor->ShowFlags = Flags;
    pViewport->Actor->RendMap   = RenMap;
    pViewport->Group            = NAME_Editor;
    pViewport->MiscRes          = nullptr;
    pViewport->Actor->Misc1     = 0;
    pViewport->Actor->Misc2     = 0;

    // SDL doesn't even use parent window
    // And we only have OpenGLDrv right now
    pViewport->OpenWindow(nullptr, 0, 320, 240, 0, 0, DEFAULT_UED_RENDERER_PATHNAME);

    m_pViewports.push_back(pViewport);

    auto pSDLWindow = (SDL_Window *) pViewport->GetWindow();
    auto pWndID     = Helpers::GetWindowHandle(pSDLWindow);
    m_pWndIDs.push_back(pWndID);

    std::map<ERenderType, Helpers::ViewportModes> map = {
        {REN_None,       Helpers::ViewportModes::Ignore},    // Hide completely.
        {REN_Wire,       Helpers::ViewportModes::Wireframe},    // Wireframe of EdPolys.
        {REN_Zones,      Helpers::ViewportModes::ZonesNPortals},    // Show zones and zone portals.
        {REN_Polys,      Helpers::ViewportModes::TextureUsage},    // Flat-shaded Bsp.
        {REN_PolyCuts,   Helpers::ViewportModes::BSPCuts},    // Flat-shaded Bsp with normals displayed.
        {REN_DynLight,   Helpers::ViewportModes::DynamicLighting},    // Illuminated texture mapping.
        {REN_PlainTex,   Helpers::ViewportModes::Textured},    // Plain texture mapping.
        {REN_OrthXY,     Helpers::ViewportModes::Top},    // Orthogonal overhead (XY) view.
        {REN_OrthXZ,     Helpers::ViewportModes::Front},    // Orthogonal XZ view.
        {REN_OrthYZ,     Helpers::ViewportModes::Side},    // Orthogonal YZ view.
        {REN_TexView,    Helpers::ViewportModes::Ignore},    // Viewing a texture (no actor).
        {REN_TexBrowser, Helpers::ViewportModes::Ignore},    // Viewing a texture browser (no actor).
        {REN_MeshView,   Helpers::ViewportModes::Ignore},    // Viewing a mesh.
        {REN_MAX,        Helpers::ViewportModes::Ignore}
    };

    m_nViewportModes.push_back(map.at((ERenderType) RenMap));

    return pViewport;
}

/**
 * Before Map Load
 * Handles some viewport flag fixes before a map is loaded
 */
void UnrealGlue::BeforeMapLoad() {
    m_vViewportFlagsBeforeMapLoad.clear();

    // Instead of clearing viewports, let's just remove SHOW_Brush
    for (auto pViewport: m_pViewports) {
        m_vViewportFlagsBeforeMapLoad.push_back(pViewport->Actor->ShowFlags);
        pViewport->Actor->ShowFlags = 0;
    }
}

/**
 * After Map Load
 * Initializes our builder brushes, and reassigns any view filters.
 */
void UnrealGlue::AfterMapLoad() {
    if (m_pViewports.empty() || m_vViewportFlagsBeforeMapLoad.empty()) {
        return;
    }

    // We're probably (poorly) calling this to fixup any issues with new/load map.
    InitBrushBuilders();

    int       i = 0;
    for (auto pViewport: m_pViewports) {
        if (!m_vViewportFlagsBeforeMapLoad.at(i)) {
            pViewport->Actor->ShowFlags = s_DefaultViewportFlags;
        } else {
            pViewport->Actor->ShowFlags = m_vViewportFlagsBeforeMapLoad[i];
        }
        i++;
    }

}

/**
 * Make Viewports
 * Generate our favourite four guys!
 */
void UnrealGlue::MakeViewports() {
    InitBrushBuilders();

    CreateViewport(TEXT("Standard1V"), REN_OrthXY, s_DefaultViewportFlags);
    CreateViewport(TEXT("Standard2V"), REN_OrthXZ, s_DefaultViewportFlags);
    CreateViewport(TEXT("Standard3V"), REN_DynLight, s_DefaultViewportFlags);
    CreateViewport(TEXT("Standard4V"), REN_OrthYZ, s_DefaultViewportFlags);
}

/**
 * Init Brush Builders
 * Queries the engine for the types of brush builders available, and makes 'em.
 */
void UnrealGlue::InitBrushBuilders() {
    GEditor->UpdateBrushBuilders();

    // Is level loaded?
    // Check to see if we already have a valid builder brush!
    if (GEditor->Level && GEditor->Level->Brush()) {
        auto pBuilderBrush = GEditor->Level->Brush();

        if (pBuilderBrush->Brush->Polys
            && pBuilderBrush->Brush->Polys->Element.Num() > 0) {
            return;
        }
    }

    // Update builders - This fixes out issue with viewport crashing on SHOW_Brush
    auto aBrushBuilders = GEditor->BrushBuilders;
    if (aBrushBuilders.Num() > 0) {
        // I think we need IsScriptable to be flipped in order for this to work.
        UBOOL GIsSavedScriptableSaved = 1;
        Exchange(GIsScriptable, GIsSavedScriptableSaved);
        aBrushBuilders(0)->eventBuild();
        Exchange(GIsScriptable, GIsSavedScriptableSaved);
    } else {
        appErrorf(TEXT("No builder brushes were found."));
    }
}
