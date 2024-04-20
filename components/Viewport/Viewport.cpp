//
// Created by melissaa on 20/04/24.
//
#include "Viewport.h"
#include "../../services/Services.h"
using namespace Components;

void Viewport::ChangeViewportMode(Helpers::ViewportModes mode) {
    m_nCurrentViewportMode = mode;
    g_pEditorAPI->SetViewportMode(this->m_nViewportWindowID, mode);
    m_pDropdownButton->setText(g_ViewportModeNameMap.at(mode));

    for (auto pButton: m_vViewportModeButtons) {
        pButton->UpdateState(mode);
    }
    for (auto pAction: m_vViewportModeActions) {
        pAction->UpdateState(mode);
    }
}

void Viewport::ChangeViewportFlag(Helpers::ViewportShowFlags flag) {
    g_pEditorAPI->ToggleViewportFlag(this->m_nViewportWindowID, flag);

    auto flags = g_pEditorAPI->GetViewportFlags(m_nViewportWindowID);

    m_pRealtimeButton->UpdateState(flags);
    for (auto pAction: m_vViewportFlags) {
        pAction->UpdateState(flags);
    }
}

void Viewport::Init() {
    m_pWidget = new QMainWindow();
    m_pWidget->show();
    m_pWidget->setWindowTitle("Viewport");
    m_pWidget->resize(640, 480);


    auto pAtlas   = QPixmap("./MultiEd/exe/UnrealEd_118.bmp");
    auto pToolbar = new QToolBar("Viewport Toolbar");

    m_pDropdownButton = new QPushButton("Hello World :)");
    m_pDropdownButton->setFlat(true);
    // lol css
    m_pDropdownButton->setStyleSheet(
        "text-align: left; margin-left: 2px; margin-right: 16px; min-width: 128px;");
    Components::Viewport::connect(m_pDropdownButton, &QPushButton::clicked, [=]() {
        m_pDropdownMenu->exec(m_pDropdownButton->mapToGlobal(m_pDropdownButton->pos()) + QPoint(-10, 30));
    });

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
        {"Top", Helpers::ViewportModes::Top},
        {"Front", Helpers::ViewportModes::Front},
        {"Side", Helpers::ViewportModes::Side},

        {"Wireframe", Helpers::ViewportModes::Wireframe},
        {"Texture Usage", Helpers::ViewportModes::TextureUsage},
        {"BSP Cuts", Helpers::ViewportModes::BSPCuts},
        {"Dynamic Lighting", Helpers::ViewportModes::DynamicLighting},
        {"Textured", Helpers::ViewportModes::Textured},
        {"Zones/Portals", Helpers::ViewportModes::ZonesNPortals},
    };
    std::vector<ViewportModeActionCreateData> ToolbarModeActions = {
        {"Top", "Alt+7", Helpers::ViewportModes::Top},
        {"Front", "Alt+8", Helpers::ViewportModes::Front},
        {"Side", "Alt+9", Helpers::ViewportModes::Side},

        {"Wireframe", "Alt+1", Helpers::ViewportModes::Wireframe},
        {"Zones/Portals", "Alt+2", Helpers::ViewportModes::ZonesNPortals},
        {"Texture Usage", "Alt+3", Helpers::ViewportModes::TextureUsage},
        {"BSP Cuts", "Alt+4", Helpers::ViewportModes::BSPCuts},
        {"Dynamic Lighting", "Alt+5", Helpers::ViewportModes::DynamicLighting},
        {"Textured", "Alt+6", Helpers::ViewportModes::Textured},
    };
    std::vector<ViewportFlagActionCreateData> ViewActions = {
        {"Show Active B&rush", "B", Helpers::ViewportShowFlags::SHOW_Brush},
        {"Show &Moving Brush", "", Helpers::ViewportShowFlags::SHOW_MovingBrushes},

        {"Show Back&drop", "K", Helpers::ViewportShowFlags::SHOW_Backdrop},
        {"Show Coordinates", "", Helpers::ViewportShowFlags::SHOW_Coords},
        {"Show Paths", "", Helpers::ViewportShowFlags::SHOW_Paths},
    };
    std::vector<ViewportFlagActionCreateData> ActorsActions = {
        {"Full Actor View", "H", Helpers::ViewportShowFlags::SHOW_Actors},
        {"Icon View", "", Helpers::ViewportShowFlags::SHOW_ActorIcons},
        {"Radii View", "", Helpers::ViewportShowFlags::SHOW_ActorRadii},
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
                                                                           Helpers::ViewportShowFlags::SHOW_PlayerCtrl,
                                                                           //Helpers::ViewportShowFlags::SHOW_RealTime, // Odd, but it seems to be like this in the UnrealEd code.
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
        if (i == 3) {
            pToolbar->addSeparator();
        }

        auto InactiveIcon = QIcon(pAtlas.copy(22 * (i + 1), 0, 22, 20));
        auto ActiveIcon   = QIcon(pAtlas.copy(22 * (i + 1), 20, 22, 20));

        auto pButton = new ViewportButton<Helpers::ViewportModes>(
            data.Tooltip, ActiveIcon, InactiveIcon, data.Mode, false, [=](Helpers::ViewportModes mode) {
                ChangeViewportMode(mode);
            });
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
        if (i == 3) {
            pModeMenu->addSeparator();
        }

        auto pAction = new ViewportAction<Helpers::ViewportModes>(
            data.sText, data.sShortcut, data.Mode, false, [=](Helpers::ViewportModes mode) {
                ChangeViewportMode(mode);
            });
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
        if (i == 2) {
            pViewMenu->addSeparator();
        }

        auto pAction = new ViewportAction<Helpers::ViewportShowFlags>(
            data.sText, data.sShortcut, data.Flag, true, [=](Helpers::ViewportShowFlags flag) {
                ChangeViewportFlag(flag);
            });

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

        auto pAction = new ViewportAction<Helpers::ViewportShowFlags>(
            data.sText, data.sShortcut, data.Flag, true, [=](Helpers::ViewportShowFlags flag) {
                ChangeViewportFlag(flag);
            });

        pAction->UpdateState(g_pEditorAPI->GetViewportFlags(m_nViewportWindowID));
        pActorsMenu->addAction(pAction);
        m_vViewportFlags.push_back(pAction);
    }
    m_pDropdownMenu->addMenu(pActorsMenu);


    // Fin~
    ChangeViewportMode(m_nCurrentViewportMode);
    m_pWidget->addToolBar(pToolbar);
}

void Viewport::Update() const {
    if (g_pEditorAPI->DoesViewportHaveRightClick(this->m_nViewportWindowID)) {
        QMenu menu("PopUp Menu");
        menu.addAction("Properties");
        menu.addSeparator();
        menu.addAction("Add light here", [=]() {
            g_pEditorAPI->AddLightHere();
        });
        menu.addSeparator();
        menu.addAction("Etc...");
        menu.exec(QCursor::pos());
    }
}