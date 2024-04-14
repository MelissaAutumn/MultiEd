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

    private:
        /**
         * Temp until we fill the viewport with something neat!
         * @param pViewport
         */
        static void LoadAndSetRandomSplash(QMainWindow* pViewport);

        QMainWindow* m_pViewport;
        Services::EditorAPI* m_pEditorAPI;
        ViewportSplitterContainer* m_pQuadView;

        void resizeEvent(QResizeEvent*);
        void moveEvent(QMoveEvent *event);

    public:
        MultiEdWindow() : QMainWindow() {

        }

        void Init();
        Services::EditorAPI* EditorAPI() { return m_pEditorAPI; }
        void SetViewport(QWidget* pWidget, WId nWindowID, Helpers::ViewportModes nMode);

        SDL_Window* m_pSDLWindow;

        //LogWindow m_LogWindow;

    };
}

#endif //MULTIED_MULTIEDWINDOW_H
