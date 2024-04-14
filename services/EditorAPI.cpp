//
// Created by melissaa on 2022-05-03.
//
/**
 * Handles most of the Editor's API calls.
 * Keeps things mostly nicely-ish separated.
 */

#include "EditorAPI.h"
#include "../helpers/SDLHelper.h"

#include "Core.h"
#include "Engine.h"
#include "EditorPrivate.h"

#include "../ue/UnrealGlue.h"
extern UnrealGlue* g_pUnreal;

#define API_IS_AVAILABLE if (!GEditor) { return; }
#define API_IS_AVAILABLE_VAL(x) if (!GEditor) { return x; }

/*
 * PRIVATE
 */


UViewport *Services::EditorAPI::FindViewport(WId pViewportID) {

        guard(FindViewport)

                if (m_pCachedViewports.find(pViewportID) != m_pCachedViewports.end()) {
                    return m_pCachedViewports.at(pViewportID);
                }

                auto aViewports = GEditor->Client->Viewports;

                for(int i = 0; i < aViewports.Num(); i++) {
                    auto pLookup = aViewports(i);
                    auto pWindow = pLookup->GetWindow();
                    auto pWndID = (WId)Helpers::GetWindowHandle((SDL_Window*)pWindow);

                    if (pWndID == pViewportID) {
                        m_pCachedViewports.insert({pWndID, pLookup});
                        return pLookup;
                    }
                }

                return nullptr;
        unguard

}

bool Services::EditorAPI::ExecCommand(const QString &sCommand)  {
    API_IS_AVAILABLE_VAL(false);

    qDebug() << "[EditorAPI::ExecCommand] " + sCommand;
    // This feels illegal
    return GEditor->Exec(sCommand.toStdWString().c_str());
}

/*
 * CONSUMABLE API
 */

/**
 * Yea this isn't great.
 * @param sCommand
 * @return
 */
bool Services::EditorAPI::LogExec(const QString &sCommand)  {
    return ExecCommand(sCommand);
}

void Services::EditorAPI::RedrawLevel() {
    GEditor->RedrawLevel( GEditor->Level );
}

void Services::EditorAPI::BrushClip() {
    this->ExecCommand("BRUSHCLIP");
    this->RedrawLevel();
}

void Services::EditorAPI::BrushSplit() {
    this->ExecCommand("BRUSHCLIP SPLIT");
    this->RedrawLevel();
}

void Services::EditorAPI::BrushFlip() {
    this->ExecCommand("BRUSHCLIP FLIP");
    this->RedrawLevel();
}

void Services::EditorAPI::BrushDelete() {
    this->ExecCommand("BRUSHCLIP DELETE");
    this->RedrawLevel();
}

void Services::EditorAPI::BrushAdd() {
    this->ExecCommand("BRUSH ADD");
    this->RedrawLevel();
}

void Services::EditorAPI::BrushSubtract() {
    this->ExecCommand("BRUSH SUBTRACT");
    this->RedrawLevel();
}

void Services::EditorAPI::BrushIntersect() {
    this->ExecCommand("BRUSH FROM INTERSECTION");
    this->RedrawLevel();
}

void Services::EditorAPI::BrushDeintersect() {
    this->ExecCommand("BRUSH FROM DEINTERSECTION");
    this->RedrawLevel();
}

void Services::EditorAPI::BrushSpecial() {
    qWarning() << "[EditorAPI::BrushSpecial] Not implemented!";
    this->RedrawLevel();
}

void Services::EditorAPI::BrushAddMover() {
    this->ExecCommand("BRUSH ADDMOVER");
    this->RedrawLevel();
}

void Services::EditorAPI::ActorShowSelected() {
    // TODO: This might be wrong?
    this->ExecCommand("ACTOR HIDE UNSELECTED");
}

void Services::EditorAPI::ActorHideSelected() {
    this->ExecCommand("ACTOR HIDE SELECTED");
}

void Services::EditorAPI::ActorSelectInside() {
    this->ExecCommand("ACTOR SELECT INSIDE");
}

void Services::EditorAPI::ActorSelectAll() {
    this->ExecCommand("ACTOR SELECT ALL");
}

void Services::EditorAPI::ActorSelectInvert() {
    this->ExecCommand("ACTOR SELECT INVERT");
}

void Services::EditorAPI::ActorShowAll() {
    this->ExecCommand("ACTOR UNHIDE ALL");
}

void Services::EditorAPI::CameraSpeed(int nSpeed) {
    QString sCommand = "MODE SPEED=";
    sCommand += std::to_string(nSpeed).c_str();
    this->ExecCommand(sCommand);
}

void Services::EditorAPI::CameraAlign() {
    this->ExecCommand("CAMERA ALIGN");
}

int Services::EditorAPI::GetCameraSpeed() {
    API_IS_AVAILABLE_VAL(16);

    auto movementSpeed = GEditor->MovementSpeed;

    if (movementSpeed == 1) { return 4; }
    if (movementSpeed == 4) { return 16; }
    return 1;
}

TArray<UBrushBuilder *> Services::EditorAPI::GetBrushBuilders() {
    API_IS_AVAILABLE_VAL(TArray<UBrushBuilder*>());

    return GEditor->BrushBuilders;
}

void Services::EditorAPI::BuildBrush(UBrushBuilder *pBrush) {
    qDebug() << "Event Build";

    // I think we need IsScriptable to be flipped in order for this to work.
    UBOOL GIsSavedScriptableSaved = 1;
    Exchange(GIsScriptable,GIsSavedScriptableSaved);
    pBrush->eventBuild();
    Exchange(GIsScriptable,GIsSavedScriptableSaved);

    this->RedrawLevel();
}

void Services::EditorAPI::NewMap(bool bAdditive) {
    guard(NewMap)

            m_pCachedViewports.clear();

            g_pUnreal->BeforeMapLoad();

            GEditor->Exec(*FString::Printf(TEXT("MAP NEW%ls"), bAdditive ? TEXT(" ADDITIVE") : TEXT("")));

            g_pUnreal->AfterMapLoad();
    unguard
}

void Services::EditorAPI::LoadMap(const TCHAR *sMap) {
    guard(LoadMap)

            m_pCachedViewports.clear();

            g_pUnreal->BeforeMapLoad();
            GEditor->Exec(*(FString::Printf(TEXT("MAP LOAD FILE=\"%ls\""), sMap)));
            g_pUnreal->AfterMapLoad();
    unguard
}

void Services::EditorAPI::SetViewportMode(WId pViewportID, Helpers::ViewportModes mode) {
    guard(SetViewportMode)
            API_IS_AVAILABLE;

            auto pViewport = FindViewport(pViewportID);

            if (!pViewport) {
                warnf(TEXT("Could not find viewport!"));
                return;
            }

            std::map<Helpers::ViewportModes, ERenderType> map = {
                {Helpers::ViewportModes::Ignore, REN_None},	// Hide completely.
                {Helpers::ViewportModes::Wireframe, REN_Wire},	// Wireframe of EdPolys.
                {Helpers::ViewportModes::ZonesNPortals, REN_Zones},	// Show zones and zone portals.
                {Helpers::ViewportModes::TextureUsage, REN_Polys},	// Flat-shaded Bsp.
                {Helpers::ViewportModes::BSPCuts, REN_PolyCuts},	// Flat-shaded Bsp with normals displayed.
                {Helpers::ViewportModes::DynamicLighting, REN_DynLight},	// Illuminated texture mapping.
                {Helpers::ViewportModes::Textured, REN_PlainTex},	// Plain texture mapping.
                {Helpers::ViewportModes::Top, REN_OrthXY},	// Orthogonal overhead (XY) view.
                {Helpers::ViewportModes::Front, REN_OrthXZ},	// Orthogonal XZ view.
                {Helpers::ViewportModes::Side, REN_OrthYZ},	// Orthogonal YZ view.
                {Helpers::ViewportModes::Ignore, REN_TexView},	// Viewing a texture (no actor).
                {Helpers::ViewportModes::Ignore, REN_TexBrowser},	// Viewing a texture browser (no actor).
                {Helpers::ViewportModes::Ignore, REN_MeshView},	// Viewing a mesh.
                {Helpers::ViewportModes::Ignore, REN_MAX}
            };

            if (map.find(mode) == map.end()) {
                return;
            }

            auto nRenMap = map.at(mode);

            pViewport->Actor->RendMap = nRenMap;
            pViewport->Repaint(1);
    unguard
}

void Services::EditorAPI::ToggleViewportFlag(WId pViewportID, Helpers::ViewportShowFlags flag) {
    guard(SetViewportMode)
            API_IS_AVAILABLE;

            auto pViewport = FindViewport(pViewportID);

            if (!pViewport) {
                warnf(TEXT("Could not find viewport!"));
                return;
            }

            pViewport->Actor->ShowFlags ^= flag;
            pViewport->Repaint(1);
    unguard
}

Helpers::ViewportShowFlags Services::EditorAPI::GetViewportFlags(WId pViewportID) {
    guard(GetViewportFlags)
            API_IS_AVAILABLE_VAL((Helpers::ViewportShowFlags)0);

            auto pViewport = FindViewport(pViewportID);

            if (!pViewport) {
                warnf(TEXT("Could not find viewport!"));
                return (Helpers::ViewportShowFlags)0;
            }

            return (Helpers::ViewportShowFlags)pViewport->Actor->ShowFlags;
    unguard
}

void Services::EditorAPI::SetMode(Helpers::EditorModes mode) {
    API_IS_AVAILABLE;

    GEditor->edcamSetMode(mode);
}

