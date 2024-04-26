//
// Created by melissaa on 26/04/24.
//
#include "Launch.h"

#include "Core.h"
#include "Engine.h"
#include <sys/stat.h>

// Memory allocator.
#include "FMallocAnsi.h"

FMallocAnsi Malloc;

// Log file.
#include "FOutputDeviceFile.h"

FOutputDeviceFile Log;

// Error handler.
#include "FOutputDeviceSDLError.h"

FOutputDeviceSDLError Error;

// Feedback.
#include "FFeedbackContextSDL.h"

FFeedbackContextSDL Warn;

// File manager.
#include "FFileManagerLinux.h"

FFileManagerLinux FileManager;

// Needed for FConfigCacheIni
[[maybe_unused]]
BOOL GRecoveryMode = false;

// Config.
#include "FConfigCacheIni.h"

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


/**
 * Boot UE similar to how SDLLaunch might.
 * @param argc
 * @param argv
 * @param moduleName
 */
UEngine *UnrealLaunch::Boot(int argc, char **argv, const char* moduleName) {
    // For FString?
    Malloc.Init();
    GMalloc = &Malloc;


    if (!appChdirSystem()) {
        wprintf(TEXT("WARNING: Could not chdir into the game's System folder\n"));
    }

    debugf(TEXT("Booting SharedLaunch"));

    // Boot up SDL2, if we haven't already
    if (!SDL_WasInit(SDL_INIT_VIDEO|SDL_INIT_JOYSTICK)) {
        if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_JOYSTICK) != 0) {
            appErrorf(TEXT("SDL_Init failed: %hs"), SDL_GetError());
        }
    } else {
        debugf(TEXT("SDL is already init!"));
    }

    guard(load_unreal)
            try {
                GIsStarted    = 1;

                // Get the command line.
                FString  CmdLine;
                for (INT i = 1; i < argc; i++) {
                    if (i > 1)
                        CmdLine += TEXT(" ");
                    CmdLine += ANSI_TO_TCHAR(argv[i]);
                }

                GDoCompatibilityChecks  = !ParseParam(*CmdLine, TEXT("bytehax")) && !ParseParam(*CmdLine, TEXT("nocompat"));
                GFixCompatibilityIssues = ParseParam(*CmdLine, TEXT("fixcompat"));

                appStrncpy(GModule, ANSI_TO_TCHAR(moduleName), ARRAY_COUNT(GModule));

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
                else {
                    TmpFilename += ANSI_TO_TCHAR(moduleName);
                    TmpFilename += TEXT(".log");
                }

                Log.Filename = TmpFilename;


                // Init core.
                GIsClient		= 1;
                GIsGuarded		= 1;
                appInit(ANSI_TO_TCHAR(moduleName), *CmdLine, &Malloc, &Log, &Error, &Warn, &FileManager, FConfigCacheIni::Factory, 1);

#if 0
                {
                    // Init Logger
                    g_LogWindow = LogWindow();
                    g_LogWindow.LogWin = new Components::qtLogWindow();
                    g_LogWindow.LogWin->Show();
                    g_LogWindow.AuxOut = GLog;
                    GLog = &g_LogWindow;
                }
#endif

                // Check various config settings
                {
                    FString sMultiEdConfigFile = ANSI_TO_TCHAR(moduleName);
                    sMultiEdConfigFile += ".ini";
                    FString sViewportManager;
                    GConfig->GetString(TEXT("Engine.Engine"), TEXT("ViewportManager"), sViewportManager, *sMultiEdConfigFile);

                    if (sViewportManager == TEXT("WinDrv.WindowsClient")) {
                        GConfig->SetString(TEXT("Engine.Engine"), TEXT("ViewportManager"), TEXT("SDLDrv.SDLClient"), *sMultiEdConfigFile);
                        GConfig->SetString(TEXT("SDLDrv.SDLClient"), TEXT("StartupFullscreen"), TEXT("False"), *sMultiEdConfigFile);
                    }

                    FString sUseGammaRampVal;
                    GConfig->GetString(TEXT("OpenGLDrv.OpenGLRenderDevice"), TEXT("ColorCorrectionMode"),
                                       sUseGammaRampVal, *sMultiEdConfigFile);

                    // FIXME: Hack because other ColourCorrectionMode's in OpenGLDrv are broken for othro views.
                    if (sUseGammaRampVal != TEXT("UseGammaRamp")) {
                        // Force GammaRamp
                        GConfig->SetString(TEXT("OpenGLDrv.OpenGLRenderDevice"), TEXT("ColorCorrectionMode"),
                                           TEXT("UseGammaRamp"), *sMultiEdConfigFile);
                        warnf(TEXT("OpenGLDrv Setting Conflict With MultiEd: Due to a bug work-around with OpenGLDrv. ColorCorrectionMode has been set to UseGammaRamp. However this will only apply on the next load. Please re-launch MultiEd for this to take effect."));
                    }

                }

                debugf(NAME_Init, TEXT("Booting MultiEd's Unreal integration"));

                // Init console log.
                if (ParseParam(*CmdLine, TEXT("LOG"))) {
                    Warn.AuxOut = GLog;
                    GLog = &Warn;
                }

#ifdef EDITOR_BUILD
                GIsClient     = GIsServer = GIsEditor = GLazyLoad = 1;
                GIsScriptable = 0;
#else
                GIsServer		= 1;
                GIsClient		= !ParseParam(appCmdLine(), TEXT("SERVER"));
                GIsEditor		= 0;
                GIsScriptable	= 1;
                GLazyLoad		= !GIsClient || ParseParam(appCmdLine(), TEXT("LAZY"));
#endif

                // Init engine.
                const auto engine = InitEngine();
#ifdef EDITOR_BUILD
                GEditor = CastChecked<UEditorEngine>(engine);

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
#endif

                return engine;
            }
            catch (...) {
                // Chained abort.  Do cleanup.
                Error.HandleError();
            }

    return nullptr;
    unguard;
}

UEngine * UnrealLaunch::InitEngine() {
    guard(InitEngine) ;
    DOUBLE LoadTime = appSecondsNew();

    // Set exec hook.
    GExec = nullptr;

    // Update first-run.
    INT FirstRun = 0;
    GConfig->GetInt(TEXT("FirstRun"), TEXT("FirstRun"), FirstRun);
    if (FirstRun < ENGINE_VERSION) {
        FirstRun = ENGINE_VERSION;
    }
    GConfig->SetInt(TEXT("FirstRun"), TEXT("FirstRun"), FirstRun);

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

bool UnrealLaunch::Loop(UEngine *engine) {
        guard(MainLoop) ;
            check(engine);

            // Loop while running.
            GIsRunning = 1;
            static auto *args = new MainLoopArgs;
            if (!args->Init) {
                args->OldTime         = appSecondsNew();
                args->SecondStartTime = args->OldTime;
                args->TickCount       = 0;
                args->Engine          = engine;
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
                guard(UpdateWorld);
                        DOUBLE NewTime   = appSecondsNew();
                        FLOAT  DeltaTime = NewTime - OldTime;
                        Engine->Tick(DeltaTime);

                        if (GWindowManager) {
                            GWindowManager->Tick(DeltaTime);
                        }

                        OldTime = NewTime;
                        TickCount++;
                        if (OldTime > SecondStartTime + 1) {
                            Engine->CurrentTickRate = static_cast<FLOAT>(TickCount) / (OldTime - SecondStartTime);
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
