//
// Created by melissaa on 2022-04-28.
//

#ifndef MULTIED_EDITORAPI_H
#define MULTIED_EDITORAPI_H

#include <QApplication>
#include <QtDebug>

#include "../helpers/Defines.h"

class AActor;
//class TArray;
template< class T > class TArray;
class UBrushBuilder;
class UViewport;
class UObject;

struct ViewportData {
    UViewport* viewport;
    int64_t rightClickPressTime;
};

enum class SelectedType
{
    ST_NONE = 0,
    ST_ACTOR = 1,
    ST_SURFACE = 2,

    ST_MAX = 3,
};

struct SelectedData
{
    SelectedType type;
    int index;
};


/**
 * Communicates with the Editor API in UnrealEd
 * TODO: Could get biiiig! Maybe split it off into sub-sections?
 */
namespace Services {
    class EditorAPI {

    private:
        bool ExecCommand(const QString& sCommand);


    public:

        bool LogExec(const QString& sCommand);

        void RebuildBSP() {
            // Not now...
            //this->ExecCommand("BSP REBUILD");
        }
        void RedrawLevel();

        /*
         * Modes
         */

        void SetMode(Helpers::EditorModes mode);

        void MoveBrushHere();

        void ResetPoly();

        void TessellatePoly();

        void PasteAtClickPosition();

        void TextureAlignFloor();

        void TextureAlignWallDirection();

        void TextureAlignWallPanning();

        void TextureAlignWallX();

        void TextureAlignWallY();

        void TextureAlignWallXY();

        void TextureAlignCylinder();

        void TextureAlignAuto();

        void TextureAlignOneTile();

        void TextureAlignOneTileU();

        void TextureAlignOneTileV();

        void TextureUnalign();

        void PolySelectAll();

        void PolySelectMatchingZones();

        void PolySelectMatchingGroups();

        void PolySelectMatchingItems();

        void PolySelectMatchingBrush();

        void PolySelectMatchingTexture();

        void PolySelectMatchingPolyFlags();

        void PolySelectAllAdjacent();

        void PolySelectAdjacentCoplanars();

        void PolySelectAdjacentWalls();

        void PolySelectAdjacentFloors();

        void PolySelectAdjacentSlants();

        void PolySelectReverse();

        void PolyMemorizeSet();

        void PolyMemorizeRecall();

        void PolyMemorizeOr();

        void PolyMemorizeAnd();

        void PolyMemorizeXor();

        void SendToFirst();

        void SendToLast();

        void ActorToBrush();

        void ActorFromBrush();

        void ActorMerge();

        void ActorSeparate();

        void SnapToGrid();

        void ActorMakeSolid();

        void ActorMakeSemiSolid();

        void ActorMakeNonSolid();

        void ActorMakeAdditive();

        void ActorMakeSubtractive();

        void BrushSelectAdd();

        void BrushSelectSubtract();

        void BrushSelectSemiSolid();

        void BrushSelectNonSolids();

        QString GetCurrentClassName();

        void ActorReplaceClass();

        void ActorReplaceClassKeepValues();

        void ActorSelectAllOfClass();

        void ActorSelectMatching();

        void ActorDuplicate();

        void ActorDelete();

        void MakeActorCurrent();

        void EditCut();

        void EditCopy();

        void EditPaste();

        void SelectedProperties();

        void LevelProperties();


        SelectedData *GetLastSelected();

        UObject *GetLastSelectedObject();

        /*
         * Clip Commands
         */

        void BrushClip();
        void BrushSplit();
        void BrushFlip();
        void BrushDelete();

        /*
         * CSG Commands
         */

        void BrushAdd();
        void BrushSubtract();
        void BrushIntersect();
        void BrushDeintersect();
        void BrushSpecial();
        void BrushAddMover();

        /*
         * Misc Commands
         */
        void ActorShowSelected();
        void ActorHideSelected();

        void ActorHideInvert();

        void ActorSelectInside();
        void ActorSelectAll();
        void ActorSelectInvert();
        void ActorShowAll();

        void ActorAlign();

        void ActorShowPolys();

        void ActorKeyframeNum(int keyFrameNum);

        void ActorResetOrigin();

        void ActorResetPivot();

        void ActorResetRotation();

        void ActorResetScale();

        void ActorResetPolyFlags();

        void ActorResetAll();

        void ActorMirrorX();

        void ActorMirrorY();

        void ActorMirrorZ();

        void ActorApplyTransform();

        void SelectNone();

        void CameraSpeed(int nSpeed);
        void CameraAlign();

        void Undo();
        void Redo();

        /*
         * Build Commands
         */
        void BuildGeometry();
        void BuildLighting();
        void BuildPaths();
        void BuildAll();
        void PlayMap();


        int GetCameraSpeed();

        // Actor Commands

        void AddLightHere();

        void AddPlayerStart();

        void AddCurrentActor();

        void AddActorHere(const QString &actor);

        // Brushes

        TArray<UBrushBuilder *> GetBrushBuilders();
        void BuildBrush(UBrushBuilder* pBrush);

        void NewMap(bool bAdditive = false);
        void LoadMap(const wchar_t* sMap);

        bool SaveMap(const wchar_t *sMap);


        /*
         * Viewport
         */

        /*
         *
         */
        void SetViewportMode(WId pViewportID, Helpers::ViewportModes mode);

        void ToggleViewportFlag(WId pViewportID, Helpers::ViewportShowFlags flag);
        Helpers::ViewportShowFlags GetViewportFlags(WId pViewportID);

        void FindSelected();


        bool HasBrowserClassSelected();

        void MakeTextureCurrent();
        QString GetCurrentTextureName();
        void SetCurrentTexture();

        void RegisterClickEvents();
        ViewportData* FindViewport(WId pViewportID);
        uint32_t GetViewportSDLWindowId(WId pViewportID);

        // Selected Actor/Surface
        std::vector<SelectedData> m_selectionData;

    private:
        std::map<WId, ViewportData*> m_pCachedViewports;


        uint32_t m_rightClickEventId = 0;
        uint32_t m_leftClickEventId = 0;


    };
}

#endif //MULTIED_EDITORAPI_H
