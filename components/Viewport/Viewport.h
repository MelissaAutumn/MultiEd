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
        QMainWindow *m_pWindow;
        QMainWindow *m_pWidget;
        WId         m_nViewportWindowID;

        QMenu* m_pDropdownMenu;
        QPushButton* m_pDropdownButton;

        Helpers::ViewportModes m_nCurrentViewportMode;
        std::map<QString, QAction*> m_mActionList;

        ViewportButton<Helpers::ViewportShowFlags>* m_pRealtimeButton;
        std::vector<ViewportButton<Helpers::ViewportModes>*> m_vViewportModeButtons;
        std::vector<ViewportAction<Helpers::ViewportModes>*> m_vViewportModeActions;
        std::vector<ViewportAction<Helpers::ViewportShowFlags>*> m_vViewportFlags;


    protected:
        template<typename ActionFunction>
        void CreateAction(QMenu* pModeMenu, const QString& sText, const ActionFunction f, const QString& sShortcut = "") {
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

        void ChangeViewportMode(Helpers::ViewportModes mode) {
            m_nCurrentViewportMode = mode;
            g_pEditorAPI->SetViewportMode(this->m_nViewportWindowID, mode);
            m_pDropdownButton->setText(g_ViewportModeNameMap.at(mode));

            for(auto pButton : m_vViewportModeButtons) {
                pButton->UpdateState(mode);
            }
            for(auto pAction : m_vViewportModeActions) {
                pAction->UpdateState(mode);
            }


        }
        void ChangeViewportFlag(Helpers::ViewportShowFlags flag) {
            g_pEditorAPI->ToggleViewportFlag(this->m_nViewportWindowID, flag);

            auto flags = g_pEditorAPI->GetViewportFlags(m_nViewportWindowID);

            m_pRealtimeButton->UpdateState(flags);
            for(auto pAction : m_vViewportFlags) {
                pAction->UpdateState(flags);
            }
        }

    public:
        explicit Viewport(QMainWindow* pWindow, WId nViewportWindowID, Helpers::ViewportModes nCurrentMode) {
            m_pWindow = pWindow;
            m_nViewportWindowID = nViewportWindowID;
            m_nCurrentViewportMode = nCurrentMode;
            m_pWidget = nullptr;
        }


        void Init() override {
            m_pWidget = new QMainWindow();
            m_pWidget->show();
            m_pWidget->setWindowTitle("Viewport");
            m_pWidget->resize(640, 480);


            auto pAtlas = QPixmap("./MultiEd/exe/UnrealEd_118.bmp");
            auto pToolbar = new QToolBar("Viewport Toolbar");

            m_pDropdownButton = new QPushButton("Hello World :)");
            m_pDropdownButton->setFlat(true);
            m_pDropdownButton->setFixedWidth(128);
            // lol css
            m_pDropdownButton->setStyleSheet("text-align: left; margin-left: 2px;");
            Components::Viewport::connect(m_pDropdownButton, &QPushButton::clicked, [=]() { m_pDropdownMenu->exec(m_pDropdownButton->mapToGlobal(m_pDropdownButton->pos() ) + QPoint(-10, 30)); });

            pToolbar->addWidget(m_pDropdownButton);

            // Face buttons
            struct ViewportModeButtonCreateData {
                QString Tooltip;
                Helpers::ViewportModes Mode;
            };
            struct ViewportModeActionCreateData {
                QString sText;
                QString sShortcut;
                Helpers::ViewportModes Mode;
            };
            struct ViewportFlagActionCreateData {
                QString sText;
                QString sShortcut;
                Helpers::ViewportShowFlags Flag;
            };

            std::vector<ViewportModeButtonCreateData> ToolbarButtons = {
                // We're cheating by putting the shortcut in the tooltip, it's actually set for the toolbar mode action!
                { "Top", Helpers::ViewportModes::Top },
                { "Front", Helpers::ViewportModes::Front },
                { "Side", Helpers::ViewportModes::Side },

                { "Wireframe", Helpers::ViewportModes::Wireframe },
                { "Texture Usage", Helpers::ViewportModes::TextureUsage },
                { "BSP Cuts", Helpers::ViewportModes::BSPCuts },
                { "Dynamic Lighting", Helpers::ViewportModes::DynamicLighting },
                { "Textured", Helpers::ViewportModes::Textured },
                { "Zones/Portals", Helpers::ViewportModes::ZonesNPortals },
            };
            std::vector<ViewportModeActionCreateData> ToolbarModeActions = {
                { "Top", "Alt+7", Helpers::ViewportModes::Top},
                { "Front", "Alt+8", Helpers::ViewportModes::Front},
                { "Side", "Alt+9", Helpers::ViewportModes::Side},

                { "Wireframe", "Alt+1", Helpers::ViewportModes::Wireframe},
                { "Zones/Portals", "Alt+2", Helpers::ViewportModes::ZonesNPortals},
                { "Texture Usage", "Alt+3", Helpers::ViewportModes::TextureUsage},
                { "BSP Cuts", "Alt+4", Helpers::ViewportModes::BSPCuts},
                { "Dynamic Lighting", "Alt+5", Helpers::ViewportModes::DynamicLighting},
                { "Textured", "Alt+6", Helpers::ViewportModes::Textured},
            };
            std::vector<ViewportFlagActionCreateData> ViewActions = {
                { "Show Active B&rush", "B", Helpers::ViewportShowFlags::SHOW_Brush},
                { "Show &Moving Brush", "", Helpers::ViewportShowFlags::SHOW_MovingBrushes},

                { "Show Back&drop", "K", Helpers::ViewportShowFlags::SHOW_Backdrop},
                { "Show Coordinates", "", Helpers::ViewportShowFlags::SHOW_Coords},
                { "Show Paths", "", Helpers::ViewportShowFlags::SHOW_Paths},
            };
            std::vector<ViewportFlagActionCreateData> ActorsActions = {
                { "Full Actor View", "H", Helpers::ViewportShowFlags::SHOW_Actors},
                { "Icon View", "", Helpers::ViewportShowFlags::SHOW_ActorIcons},
                { "Radii View", "", Helpers::ViewportShowFlags::SHOW_ActorRadii},
                // No
                //{ "Hide Actors", "", ~(Helpers::ViewportShowFlags::SHOW_Actors | Helpers::ViewportShowFlags::SHOW_ActorIcons | Helpers::ViewportShowFlags::SHOW_ActorRadii)},
            };

            /*
             * REALTIME PREVIEW BUTTON
             */
            {
                auto InactiveIcon = QIcon(pAtlas.copy(0, 0, 22, 20));
                auto ActiveIcon   = QIcon(pAtlas.copy(0, 20, 22, 20));
                m_pRealtimeButton = new ViewportButton<Helpers::ViewportShowFlags>("Realtime Preview (P)",
                                                                                   ActiveIcon,
                                                                                   InactiveIcon,
                                                                                   Helpers::ViewportShowFlags::SHOW_PlayerCtrl,//Helpers::ViewportShowFlags::SHOW_RealTime, // Odd, but it seems to be like this in the UnrealEd code.
                                                                                   true,
                                                                                   [=](Helpers::ViewportShowFlags flag) {
                                                                                       ChangeViewportFlag(flag);
                                                                                   });
                m_pRealtimeButton->UpdateState(g_pEditorAPI->GetViewportFlags(m_nViewportWindowID));
                pToolbar->addWidget(m_pRealtimeButton);
                pToolbar->addSeparator();
            }

            /*
             * VIEWPORT MODE FACE BUTTONS
             */
            for (int i = 0; i < ToolbarButtons.size(); i++) {
                auto data = ToolbarButtons[i];

                // Hacky solution for adding separators!
                if ( i == 3 ) {
                    pToolbar->addSeparator();
                }

                auto InactiveIcon = QIcon(pAtlas.copy(22*(i+1), 0, 22, 20));
                auto ActiveIcon = QIcon(pAtlas.copy(22*(i+1), 20, 22, 20));

                auto pButton = new ViewportButton<Helpers::ViewportModes>(data.Tooltip, ActiveIcon, InactiveIcon, data.Mode, false, [=](Helpers::ViewportModes mode) { ChangeViewportMode(mode); });
                pButton->UpdateState(m_nCurrentViewportMode);

                pToolbar->addWidget(pButton);
                m_vViewportModeButtons.push_back(pButton);
            }

            m_pDropdownMenu = new QMenu("Viewport Options", m_pDropdownButton);

            /*
             * MODE MENU
             */
            auto pModeMenu = new QMenu("&Mode");
            for (int i = 0; i < ToolbarModeActions.size(); i++) {
                auto data = ToolbarModeActions[i];

                // Hacky solution for adding separators!
                if ( i == 3 ) {
                    pModeMenu->addSeparator();
                }

                auto pAction = new ViewportAction<Helpers::ViewportModes>(data.sText, data.sShortcut, data.Mode, false, [=](Helpers::ViewportModes mode) { ChangeViewportMode(mode); });
                pAction->UpdateState(m_nCurrentViewportMode);

                pModeMenu->addAction(pAction);
                m_vViewportModeActions.push_back(pAction);
            }
            m_pDropdownMenu->addMenu(pModeMenu);

            /*
             *  VIEW MENU
             */
            auto pViewMenu = new QMenu("&View");
            for (int i = 0; i < ViewActions.size(); i++) {
                auto data = ViewActions[i];

                // Hacky solution for adding separators!
                if ( i == 2 ) {
                    pViewMenu->addSeparator();
                }

                auto pAction = new ViewportAction<Helpers::ViewportShowFlags>(data.sText, data.sShortcut, data.Flag, true, [=](Helpers::ViewportShowFlags flag) { ChangeViewportFlag(flag); });

                pAction->UpdateState(g_pEditorAPI->GetViewportFlags(m_nViewportWindowID));
                pViewMenu->addAction(pAction);
                m_vViewportFlags.push_back(pAction);
            }
            m_pDropdownMenu->addMenu(pViewMenu);

            /*
             *  ACTORS MENU
             */
            auto pActorsMenu = new QMenu("&Actors");
            for (int i = 0; i < ActorsActions.size(); i++) {
                auto data = ActorsActions[i];

                auto pAction = new ViewportAction<Helpers::ViewportShowFlags>(data.sText, data.sShortcut, data.Flag, true, [=](Helpers::ViewportShowFlags flag) { ChangeViewportFlag(flag); });

                pAction->UpdateState(g_pEditorAPI->GetViewportFlags(m_nViewportWindowID));
                pActorsMenu->addAction(pAction);
                m_vViewportFlags.push_back(pAction);
            }
            m_pDropdownMenu->addMenu(pActorsMenu);


            // Fin~
            ChangeViewportMode(m_nCurrentViewportMode);
            m_pWidget->addToolBar(pToolbar);
        };


        QMainWindow* GetWidget() { return m_pWidget; }
    };
}

#endif //MULTIED_VIEWPORT_H
