//
// Created by melissaa on 20/04/24.
//
#include "Viewport.h"

#include "../MultiEdWindow.h"
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
    Components::Viewport::connect(m_pDropdownButton, &QPushButton::clicked, [=] {
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


void Viewport::OpenActorMenu() {
    QMenu menu("PopUp Menu");
    menu.addAction(QString("Actor Properties"));
    menu.addSeparator();
    menu.addAction("Add light here", [=] {
        g_pEditorAPI->AddLightHere();
    });
    menu.addSeparator();
    menu.addAction("Etc...");
    menu.exec(QCursor::pos());
}

void Viewport::OpenSurfaceMenu() {
    QMenu alignMenu("&Align Selected");
    alignMenu.addAction("Align as &Floor/Ceiling", [=] { g_pEditorAPI->TextureAlignFloor(); });
    alignMenu.addAction("Align Wall &Direction", [=] { g_pEditorAPI->TextureAlignWallDirection(); });
    alignMenu.addAction("Align Wall &Panning", [=] { g_pEditorAPI->TextureAlignWallPanning(); });
    alignMenu.addAction("Align to Wall Around &X Axis", [=] { g_pEditorAPI->TextureAlignWallX(); });
    alignMenu.addAction("Align to Wall Around &Y Axis", [=] { g_pEditorAPI->TextureAlignWallY(); });
    alignMenu.addAction("Align to Wall Around XY Axi&s", [=] { g_pEditorAPI->TextureAlignWallXY(); });
    alignMenu.addAction("Align to Wall for &Cylinder", [=] { g_pEditorAPI->TextureAlignCylinder(); });
    alignMenu.addAction("Align as &Auto", [=] { g_pEditorAPI->TextureAlignAuto(); });
    alignMenu.addAction("Align as One &Tile", [=] { g_pEditorAPI->TextureAlignOneTile(); });
    alignMenu.addAction("Align as One Tile (&U)", [=] { g_pEditorAPI->TextureAlignOneTileU(); });
    alignMenu.addAction("Align as One Tile (&V)", [=] { g_pEditorAPI->TextureAlignOneTileV(); });
    alignMenu.addSeparator();
    alignMenu.addAction("&Unalign Back To Default", [=] { g_pEditorAPI->TextureUnalign(); });  // Gasp plot twist!

    QMenu selectMenu("&Select Surfaces");
    selectMenu.addAction("Select &All Surfaces", [=] { g_pEditorAPI->PolySelectAll(); });
    selectMenu.addAction("Select &None", [=] { g_pEditorAPI->SelectNone(); });
    selectMenu.addAction("Matching &Zones", [=] { g_pEditorAPI->PolySelectMatchingZones(); });
    selectMenu.addAction("Matching &Groups", [=] { g_pEditorAPI->PolySelectMatchingGroups(); });
    selectMenu.addAction("Matching &Items", [=] { g_pEditorAPI->PolySelectMatchingItems(); });
    selectMenu.addAction("Matching &Brush", [=] { g_pEditorAPI->PolySelectMatchingBrush(); });
    selectMenu.addAction("Matching &Texture", [=] { g_pEditorAPI->PolySelectMatchingTexture(); });
    selectMenu.addAction("Matching &PolyFlags", [=] { g_pEditorAPI->PolySelectMatchingPolyFlags(); });
    selectMenu.addSeparator();
    selectMenu.addAction("All Ad&jacents", [=] { g_pEditorAPI->PolySelectAllAdjacent(); });
    selectMenu.addAction("Adjacent &Coplanars", [=] { g_pEditorAPI->PolySelectAdjacentCoplanars(); });
    selectMenu.addAction("Adjacent &Walls", [=] { g_pEditorAPI->PolySelectAdjacentWalls(); });
    selectMenu.addAction("Adjacent &Floors/Ceilings", [=] { g_pEditorAPI->PolySelectAdjacentFloors(); });
    selectMenu.addAction("Adjacent &Slants", [=] { g_pEditorAPI->PolySelectAdjacentSlants(); });
    selectMenu.addSeparator();
    selectMenu.addAction("Reverse", [=] { g_pEditorAPI->PolySelectReverse(); });
    selectMenu.addSeparator();
    selectMenu.addAction("&Memorize Set", [=] { g_pEditorAPI->PolyMemorizeSet(); });
    selectMenu.addAction("&Recall Memory", [=] { g_pEditorAPI->PolyMemorizeRecall(); });
    selectMenu.addAction("&Or With Memory", [=] { g_pEditorAPI->PolyMemorizeOr(); });
    selectMenu.addAction("&And With Memory", [=] { g_pEditorAPI->PolyMemorizeAnd(); });
    selectMenu.addAction("&Xor With Memory", [=] { g_pEditorAPI->PolyMemorizeXor(); });


    QMenu menu("Surface Menu");
    menu.addAction(QString("Surface &Properties (%1 Selected)"));
    menu.addSeparator();
    if (g_pEditorAPI->HasBrowserClassSelected())
    {
        menu.addAction("Add Class Here", [=] {
            g_pEditorAPI->AddCurrentActor();
        });
    }
    menu.addAction("Add &Light Here", [=] {
        g_pEditorAPI->AddLightHere();
    });
    menu.addAction("&Move Brush Here", [=] {
        g_pEditorAPI->MoveBrushHere();
    });
    menu.addAction("Add PlayerStart Here", [=] {
        g_pEditorAPI->AddPlayerStart();
    });
    menu.addSeparator();
    menu.addMenu(&alignMenu);
    menu.addAction("&Reset", [=] {
        g_pEditorAPI->ResetPoly();
    });
    menu.addAction("&Tessellate Surface", [=] {
        g_pEditorAPI->TessellatePoly();
    });
    menu.addAction("Make Texture Current", [=] { g_pEditorAPI->MakeTextureCurrent(); });
    menu.addSeparator();
    menu.addMenu(&selectMenu);
    menu.addAction(QString("Apply &Texture %1").arg(g_pEditorAPI->GetCurrentTextureName()), [=] { g_pEditorAPI->SetCurrentTexture(); });
    menu.addSeparator();
    menu.addAction("Align Viewport Cameras to 3D", [=] {
        g_pEditorAPI->CameraAlign();
    });
    menu.addSeparator();
    menu.addAction("Paste Actor &Click Location", [=] {
        g_pEditorAPI->PasteAtClickPosition();
    });

    menu.exec(QCursor::pos());
}

void Viewport::OpenDefaultMenu() {
    QMenu menu("PopUp Menu");
    menu.addAction(QString("Level Properties"));
    menu.addSeparator();
    menu.addAction("Add light here", [=] {
        g_pEditorAPI->AddLightHere();
    });
    menu.addSeparator();
    menu.addAction("Etc...");
    menu.exec(QCursor::pos());
}

void Viewport::handleRightClick(bool isReleased, uint32_t timestamp)
{
    if (!isReleased)
    {
        this->m_RightClickTimestamp = timestamp;
        return;
    }

    // If the Press to Release was greater than 100ms treat it as a hold and ignore it!
    if (SDL_GetTicks() - this->m_RightClickTimestamp >= 100/*ms*/)
    {
        this->m_RightClickTimestamp = 0;
        return;
    }

    g_pEditorAPI->FindSelected();
    if (g_pEditorAPI->m_selectionData.empty())
    {
        this->OpenDefaultMenu();
    }
    else if (g_pEditorAPI->m_selectionData[0].type == SelectedType::ST_ACTOR)
    {
        this->OpenActorMenu();
    } else
    {
        this->OpenSurfaceMenu();
    }

}

Viewport::Viewport(QMainWindow* pWindow, WId nViewportWindowID, Helpers::ViewportModes nCurrentMode): m_pDropdownMenu(
    nullptr), m_pDropdownButton(
    nullptr), m_pRealtimeButton(
    nullptr)
{
    m_pWindow              = pWindow;
    m_nViewportWindowID    = nViewportWindowID;
    m_nCurrentViewportMode = nCurrentMode;
    m_pWidget              = nullptr;
    m_RightClickTimestamp  = 0;


    connect(reinterpret_cast<MultiEdWindow*>(pWindow), &MultiEdWindow::rightClick, [=](WId windowId, bool isReleased, uint32_t timestamp)
    {
        // This is dumb but I plumbed it wrong so it will be fixed in the future
        // The lookups should mostly be cached...
        if (g_pEditorAPI->GetViewportSDLWindowId(this->m_nViewportWindowID) == windowId)
        {
            this->handleRightClick(isReleased, timestamp);
        }
    });
}
