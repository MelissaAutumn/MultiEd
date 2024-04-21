//
// Created by melissaa on 2022-04-27.
//

#ifndef MULTIED_VIEWPORT_H
#define MULTIED_VIEWPORT_H
#include <QApplication>
#include <QAction>
#include <QIcon>
#include <QPixmap>
#include <QWidget>
#include <QMainWindow>
#include <QToolBar>
#include <QComboBox>
#include <QMenu>
#include <iostream>
#include "../IComponent.h"
#include "ViewportButton.h"
#include "ViewportAction.h"

#include <functional>

namespace Components {
    class Viewport : IComponent, public QObject {
    protected:

    private:
        QMainWindow *m_pWindow{};
        QMainWindow *m_pWidget;
        WId m_nViewportWindowID;

        QMenu *m_pDropdownMenu;
        QPushButton *m_pDropdownButton;

        Helpers::ViewportModes m_nCurrentViewportMode;
        std::map<QString, QAction *> m_mActionList;

        ViewportButton<Helpers::ViewportShowFlags> *m_pRealtimeButton;
        std::vector<ViewportButton<Helpers::ViewportModes> *> m_vViewportModeButtons;
        std::vector<ViewportAction<Helpers::ViewportModes> *> m_vViewportModeActions;
        std::vector<ViewportAction<Helpers::ViewportShowFlags> *> m_vViewportFlags;

        // Is set on RightClick Press, used to determine if the right click is a hold or click.
        uint32_t m_RightClickTimestamp;

    protected:
        template<typename ActionFunction>
        void CreateAction(QMenu *pModeMenu, const QString &sText, const ActionFunction f,
                          const QString &sShortcut = "") {
            // Lol! This isn't great.
            // Instead of explicitly defining these, why not stuff them in a hashmap, keyed by name?
            auto pAction = new QAction(sText, m_pWindow);
            if (!sShortcut.isEmpty()) {
                pAction->setShortcut(QKeySequence::fromString(sShortcut));
            }
            QMainWindow::connect(pAction, &QAction::triggered, f);

            // Finally add it to our menu
            pModeMenu->addAction(pAction);

            m_mActionList.insert({sText, pAction});
        }

        void ChangeViewportMode(Helpers::ViewportModes mode);

        void ChangeViewportFlag(Helpers::ViewportShowFlags flag);

    public slots:
        void handleRightClick(bool isReleased, uint32_t timestamp);
    public:
        explicit Viewport(QMainWindow *pWindow, WId nViewportWindowID, Helpers::ViewportModes nCurrentMode);


        void Init() override;;

        /**
         * Open a context menu for a selection of actor(s)
         */
        void OpenActorMenu();
        /**
         * Open a context menu for a selection of surface(s)
         */
        void OpenSurfaceMenu();
        /**
         * Open the default context menu when you right click on nothing with nothing selected!
         */
        void OpenDefaultMenu();


        QMainWindow *GetWidget() { return m_pWidget; }

    };
}

#endif //MULTIED_VIEWPORT_H
