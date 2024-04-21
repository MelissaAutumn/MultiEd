//
// Created by melissaa on 2022-05-03.
//
/**
 * Handles most of the Editor's API calls.
 * Keeps things mostly nicely-ish separated.
 */

#include "EditorAPI.h"

#include <qdatetime.h>
#include <qdir.h>
#include <qprocess.h>

#include "../helpers/SDLHelper.h"

#include "Core.h"
#include "Engine.h"
#include "EditorPrivate.h"

#include "../ue/UnrealGlue.h"
#include <SDLDrv.h>
extern UnrealGlue *g_pUnreal;

#define API_IS_AVAILABLE if (!GEditor) { return; }
#define API_IS_AVAILABLE_VAL(x) if (!GEditor) { return x; }

/*
 * PRIVATE
 */


ViewportData *Services::EditorAPI::FindViewport(WId pViewportID) {
    guard(FindViewport)
        if (m_pCachedViewports.find(pViewportID) != m_pCachedViewports.end()) {
            return m_pCachedViewports.at(pViewportID);
        }

        auto aViewports = GEditor->Client->Viewports;

        for (int i = 0; i < aViewports.Num(); i++) {
            auto pLookup = aViewports(i);
            auto pWindow = pLookup->GetWindow();
            auto pWndID  = (WId) Helpers::GetWindowHandle((SDL_Window *) pWindow);

            if (pWndID == pViewportID) {
                auto viewportData                  = new ViewportData();
                viewportData->viewport             = pLookup;
                viewportData->rightClickPressTime = 0;
                m_pCachedViewports.insert({pWndID, viewportData});
                return viewportData;
            }
        }

        return nullptr;
    unguard
}

uint32_t Services::EditorAPI::GetViewportSDLWindowId(WId pViewportID)
{
    auto viewportData = this->FindViewport(pViewportID);
    auto window = viewportData->viewport->GetWindow();
    return SDL_GetWindowID(static_cast<SDL_Window*>(window));
}

bool Services::EditorAPI::ExecCommand(const QString &sCommand) {
    API_IS_AVAILABLE_VAL(false);

    qDebug() << "[EditorAPI::ExecCommand] " + sCommand;
    guard(ExecCommand);
    // This feels illegal
    return GEditor->Exec(sCommand.toStdWString().c_str());
    unguard;
}

/*
 * CONSUMABLE API
 */

/**
 * Yea this isn't great.
 * @param sCommand
 * @return
 */
bool Services::EditorAPI::LogExec(const QString &sCommand) {
    return ExecCommand(sCommand);
}

void Services::EditorAPI::RedrawLevel() {
    GEditor->RedrawLevel(GEditor->Level);
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

void Services::EditorAPI::Undo() {
    this->ExecCommand("TRANSACTION UNDO");
}

void Services::EditorAPI::Redo() {
    this->ExecCommand("TRANSACTION REDO");
}

void Services::EditorAPI::BuildGeometry() {
    this->ExecCommand(QString("MAP REBUILD VISIBLEONLY=%1").arg("0"));
}

void Services::EditorAPI::BuildLighting() {
    this->ExecCommand(QString("LIGHT APPLY SELECTED=%1").arg("0"));
    GEditor->Flush(1);
}

void Services::EditorAPI::BuildPaths() {
    this->ExecCommand("PATHS DEFINE");
}

void Services::EditorAPI::BuildAll() {
    // Args: (Based on default settings right now)
    // 1. Optimal Build Quality
    // 2. Optimize Geometry
    // 3. Build Visibility Zones
    // 4. Minimize Cuts vs Balance Tree
    // 5. Ignore Portals vs Portals Cut All
    this->ExecCommand(QString("BSP REBUILD %1 %2 %3 BALANCE=%4 PORTALBIAS=%5").arg("OPTIMAL", "OPTGEOM", "ZONES", "15", "7"));
}

void Services::EditorAPI::PlayMap() {
    // Doesn't seem to work. Windows specific?
    //this->ExecCommand("HOOK PLAYMAP");

    if (this->SaveMap(L"../Maps/_multied_playmap.unr")) {
        // Launch via qt
        auto process = new QProcess();
#ifdef __LINUX__
        process->start("./ut-bin", {"_multied_playmap.unr"});
#else
        this->ExecCommand("HOOK PLAYMAP");
#endif
    }
}

int Services::EditorAPI::GetCameraSpeed() {
    API_IS_AVAILABLE_VAL(16);

    auto movementSpeed = GEditor->MovementSpeed;

    if (movementSpeed == 1) { return 4; }
    if (movementSpeed == 4) { return 16; }
    return 1;
}

void Services::EditorAPI::AddLightHere() {
    this->ExecCommand("ACTOR ADD CLASS=ENGINE.LIGHT");
    this->ExecCommand("POLY SELECT NONE");
}

TArray<UBrushBuilder *> Services::EditorAPI::GetBrushBuilders() {
    API_IS_AVAILABLE_VAL(TArray<UBrushBuilder*>());

    return GEditor->BrushBuilders;
}

void Services::EditorAPI::BuildBrush(UBrushBuilder *pBrush) {
    qDebug() << "Event Build";

    // I think we need IsScriptable to be flipped in order for this to work.
    UBOOL GIsSavedScriptableSaved = 1;
    Exchange(GIsScriptable, GIsSavedScriptableSaved);
    pBrush->eventBuild();
    Exchange(GIsScriptable, GIsSavedScriptableSaved);

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

bool Services::EditorAPI::SaveMap(const TCHAR *sMap) {
    guard(SaveMap)
    return GEditor->Exec(*(FString::Printf(TEXT("MAP SAVE FILE=\"%ls\""), sMap)));
    unguard
}


void Services::EditorAPI::SetViewportMode(WId pViewportID, Helpers::ViewportModes mode) {
    guard(SetViewportMode)
        API_IS_AVAILABLE;

        auto viewportData = FindViewport(pViewportID);

        if (!viewportData || !viewportData->viewport) {
            warnf(TEXT("Could not find viewport!"));
            return;
        }

        auto pViewport = viewportData->viewport;

        std::map<Helpers::ViewportModes, ERenderType> map = {
            {Helpers::ViewportModes::Ignore, REN_None}, // Hide completely.
            {Helpers::ViewportModes::Wireframe, REN_Wire}, // Wireframe of EdPolys.
            {Helpers::ViewportModes::ZonesNPortals, REN_Zones}, // Show zones and zone portals.
            {Helpers::ViewportModes::TextureUsage, REN_Polys}, // Flat-shaded Bsp.
            {Helpers::ViewportModes::BSPCuts, REN_PolyCuts}, // Flat-shaded Bsp with normals displayed.
            {Helpers::ViewportModes::DynamicLighting, REN_DynLight}, // Illuminated texture mapping.
            {Helpers::ViewportModes::Textured, REN_PlainTex}, // Plain texture mapping.
            {Helpers::ViewportModes::Top, REN_OrthXY}, // Orthogonal overhead (XY) view.
            {Helpers::ViewportModes::Front, REN_OrthXZ}, // Orthogonal XZ view.
            {Helpers::ViewportModes::Side, REN_OrthYZ}, // Orthogonal YZ view.
            {Helpers::ViewportModes::Ignore, REN_TexView}, // Viewing a texture (no actor).
            {Helpers::ViewportModes::Ignore, REN_TexBrowser}, // Viewing a texture browser (no actor).
            {Helpers::ViewportModes::Ignore, REN_MeshView}, // Viewing a mesh.
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
        auto viewportData = FindViewport(pViewportID);

        if (!viewportData || !viewportData->viewport) {
            warnf(TEXT("Could not find viewport!"));
            return;
        }

        auto pViewport = viewportData->viewport;

        pViewport->Actor->ShowFlags ^= flag;
        pViewport->Repaint(1);
    unguard
}

Helpers::ViewportShowFlags Services::EditorAPI::GetViewportFlags(WId pViewportID) {
    guard(GetViewportFlags)
        API_IS_AVAILABLE_VAL((Helpers::ViewportShowFlags)0);

        auto viewportData = FindViewport(pViewportID);

        if (!viewportData || !viewportData->viewport) {
            warnf(TEXT("Could not find viewport!"));
            return (Helpers::ViewportShowFlags) 0;
        }
        auto pViewport = viewportData->viewport;
        return (Helpers::ViewportShowFlags) pViewport->Actor->ShowFlags;
    unguard
}

/**
 * A simple class to get a return value from Exec
 * There may be another way to do this but I don't know it!
 * Leave a github issue or PR :)
 */
class FReturnDevice : public FOutputDevice
{
public:
    virtual ~FReturnDevice() = default;
    void Serialize(const TCHAR* V, EName Event) override
    {
        returnValue.ui = 0;
        returnType = Event;
        auto string = QString::fromWCharArray(V);

        if (Event == EName::NAME_Int)
        {
            this->returnValue.ui = string.toUInt();
        }
        else if (Event == EName::NAME_Float)
        {
            this->returnValue.f = string.toFloat();
        }
        else if (Event == EName::NAME_Bool)
        {
            this->returnValue.b = string.toInt();
        }
    };

    EName returnType = EName::NAME_None;

    union
    {
        uint32_t ui;
        float f;
        bool b;
    } returnValue;

};

void Services::EditorAPI::FindSelected()
{
    m_selectionData.clear();

    if (!GEditor || !GEditor->Level)
    {
        return;
    }

    for (int i = 0; i < GEditor->Level->Actors.Num(); ++i)
    {
        auto actor = GEditor->Level->Actors(i);
        if (actor)
        {
            if (actor->bSelected)
            {
                m_selectionData.push_back({SelectedType::ST_ACTOR, i});
            }
        }
    }
    for (int i = 0; i < GEditor->Level->Model->Surfs.Num(); ++i)
    {
        auto surf = GEditor->Level->Model->Surfs(i);
        if (surf.PolyFlags & EPolyFlags::PF_Selected)
        {
            m_selectionData.push_back({SelectedType::ST_SURFACE, i});
        }
    }
}

bool Services::EditorAPI::HasBrowserClassSelected()
{
    return !!GEditor->CurrentClass;
}

void Services::EditorAPI::MakeTextureCurrent()
{
    API_IS_AVAILABLE;
    if (m_selectionData.empty())
    {
        return;
    }

    auto lastSelection = m_selectionData[m_selectionData.size()-1];

    if (lastSelection.type != SelectedType::ST_SURFACE)
    {
        return;
    }

    GEditor->CurrentTexture = GEditor->Level->Model->Surfs(lastSelection.index).Texture;
    if (GEditor->CurrentTexture)
    {
        QString texName = QString::fromWCharArray(GEditor->CurrentTexture->GetName());
        qInfo() << "Current Texture " << lastSelection.index << texName;
    }
}

QString Services::EditorAPI::GetCurrentTextureName()
{
    API_IS_AVAILABLE_VAL("");
    if (!GEditor->CurrentTexture)
    {
        return "";
    }
    return QString::fromWCharArray(GEditor->CurrentTexture->GetName());
}

void Services::EditorAPI::SetCurrentTexture()
{
    API_IS_AVAILABLE;
    if (!GEditor->CurrentTexture)
    {
        return;
    }

    this->ExecCommand("POLY SETTEXTURE");
}

void Services::EditorAPI::RegisterRightClickEvent()
{
    auto rd = FReturnDevice();

    // IK_RightMouse = 2;
    GEditor->Client->Exec(L"REGISTER_INPUTEVENT KEY=2", rd);

    if (rd.returnType == EName::NAME_Int)
    {
        m_rightClickEventId = rd.returnValue.ui;
    }
}

void Services::EditorAPI::SetMode(Helpers::EditorModes mode) {
    API_IS_AVAILABLE;

    GEditor->edcamSetMode(mode);
}
