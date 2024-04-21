//
// Created by melissaa on 2022-04-28.
//

#ifndef MULTIED_MULTIEDWINDOW_H
#define MULTIED_MULTIEDWINDOW_H

#include <QApplication>
#include <QPushButton>
#include <QHBoxLayout>
#include <QDockWidget>
#include <QLabel>
#include <csignal>

// Components
#include "./Menu/FileMenu.h"
#include "./Menu/EditMenu.h"
#include "./Menu/ViewMenu.h"
#include "./Menu/BrushMenu.h"
#include "./Menu/ActorMenu.h"
#include "./Menu/BuildMenu.h"
#include "./Menu/ToolsMenu.h"
#include "./Menu/HelpMenu.h"
#include "./ToolBar/ToolBar.h"
#include "./Viewport/Viewport.h"
#include "./SideBar/OrderedSideBar.h"
#include "../services/EditorAPI.h"
#include "Viewport/ViewportSplitterContainer.h"

#include <SDL2/SDL.h>
#include "../helpers/Defines.h"

// TODO: Should this be in a different namespace?
namespace Components {
    class MultiEdWindow : public QMainWindow {
    Q_OBJECT;
    private:
        QMainWindow* m_pViewport{};
        ViewportSplitterContainer* m_pQuadView{};

        std::vector<Viewport*> m_viewports;

        // Components
        QDockWidget* m_pDockWidget{};
        OrderedSideBar* m_pOrderedSideBar{};
        FileMenu* m_pFileMenu{};
        EditMenu* m_pEditMenu{};
        ViewMenu* m_pViewMenu{};
        BrushMenu* m_pBrushMenu{};
        ActorMenu* m_pActorMenu{};
        BuildMenu* m_pBuildMenu{};
        ToolsMenu* m_pToolsMenu{};
        HelpMenu* m_pHelpMenu{};
        ToolBar* m_pToolbar{};

    public:
        MultiEdWindow() : QMainWindow() {

        }

        ~MultiEdWindow() override;

        void Init();
        void InitInputs();
        void SetViewport(QWidget* pWidget, WId nWindowID, Helpers::ViewportModes nMode);
        void Update();

        SDL_Window* m_pSDLWindow{};
    signals:
        void rightClick(WId windowId, bool isReleased, uint32_t timestamp);
    };
}

#endif //MULTIED_MULTIEDWINDOW_H
