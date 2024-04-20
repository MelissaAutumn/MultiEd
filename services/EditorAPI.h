//
// Created by melissaa on 2022-04-28.
//

#ifndef MULTIED_EDITORAPI_H
#define MULTIED_EDITORAPI_H

#include <QApplication>
#include <QtDebug>
#include "../helpers/Defines.h"

//class TArray;
template< class T > class TArray;
class UBrushBuilder;
class UViewport;
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

#if 0
        void ModeCamera() {
            this->SetMode( EM_ViewportMove );
        }
        void ModeSheer() {
            this->SetMode( EM_BrushSheer );
        }
        void ModeScale() {
            this->SetMode( EM_BrushScale );
        }
        void ModeStretch() {
            this->SetMode( EM_BrushStretch );
        }
        void ModeSnapScale() {
            this->SetMode( EM_BrushSnap );
        }
        void ModeRotate() {
            this->SetMode( EM_BrushRotate );
        }
        void ModeVertexEdit() {
            this->SetMode( EM_VertexEdit );
        }
        void ModeBrushClip() {
            this->SetMode( EM_BrushClip );
        }
        void ModeTexturePan() {
            this->SetMode( EM_TexturePan );
        }
        void ModeTextureRotate() {
            this->SetMode( EM_TextureRotate );
        }
#endif
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
        void ActorSelectInside();
        void ActorSelectAll();
        void ActorSelectInvert();
        void ActorShowAll();
        void CameraSpeed(int nSpeed);
        void CameraAlign();

        int GetCameraSpeed();

        // Actor Commands

        void AddLightHere();

        // Brushes

        TArray<UBrushBuilder *> GetBrushBuilders();
        void BuildBrush(UBrushBuilder* pBrush);

        void NewMap(bool bAdditive = false);
        void LoadMap(const wchar_t* sMap);
        void SaveMap() {

        }


        /*
         * Viewport
         */

        /*
         *
         */
        void SetViewportMode(WId pViewportID, Helpers::ViewportModes mode);

        void ToggleViewportFlag(WId pViewportID, Helpers::ViewportShowFlags flag);
        Helpers::ViewportShowFlags GetViewportFlags(WId pViewportID);

        bool DoesViewportHaveRightClick(WId pViewportID);

    private:
        std::map<WId, UViewport*> m_pCachedViewports;

        UViewport* FindViewport(WId pViewportID);
    };
}

#endif //MULTIED_EDITORAPI_H
