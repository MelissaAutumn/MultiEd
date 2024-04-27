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
#include <ue/FOutputDeviceSDLError.h>

FOutputDeviceSDLError Error;

// Feedback.
#include <ue/FFeedbackContextSDL.h>

FFeedbackContextSDL Warn;

// File manager.
#include "FFileManagerLinux.h"

FFileManagerLinux FileManager;

// Needed for FConfigCacheIni
[[maybe_unused]]
BOOL GRecoveryMode = false;

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
 * Boots some additional editor logic
 * @param engine Instance of a booted UEngine object (hopefully the editor variant!)
 */
void UnrealGlue::Boot(UEngine* engine) {
    guard(load_unreal)
            try {

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

                BeforeMapLoad();
                MakeViewports();
                AfterMapLoad();
            }
            catch (...) {
                // Chained abort.  Do cleanup.
                Error.HandleError();
            }

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

    auto pSDLWindow = static_cast<SDL_Window *>(pViewport->GetWindow());
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
