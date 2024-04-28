//
// Created by melissaa on 2022-04-29.
//

#include "MultiEdWindow.h"

#include "Preferences/Preferences.h"

using namespace Components;

// Timer resolution for updates
constexpr int updateTimerMs = 50;

MultiEdWindow::~MultiEdWindow() {
    delete m_pDockWidget;
    delete m_pOrderedSideBar;
    delete m_pFileMenu;
    delete m_pEditMenu;
    delete m_pViewMenu;
    delete m_pBrushMenu;
    delete m_pActorMenu;
    delete m_pBuildMenu;
    delete m_pToolsMenu;
    delete m_pHelpMenu;
    delete m_pToolbar;
    delete m_pQuadView;

    // Clean up any viewports
    for(auto viewport : m_viewports) {
        delete viewport;
    }
}


/**
 * Set a window to a given viewport mode and parent it to a widget
 * @param pWidget - Widget to parent the viewport to
 * @param nWindowID - Foreign Window Handler
 * @param nMode - Viewport mode
 */
void MultiEdWindow::SetViewport(QWidget *pWidget, WId nWindowID, Helpers::ViewportModes nMode) {
    auto pViewportWidget = new Components::Viewport(this, nWindowID, nMode);
    pViewportWidget->Init();

    m_pViewport = pViewportWidget->GetWidget();
    m_pViewport->setCentralWidget(pWidget);
    m_pQuadView->AddWidget(pViewportWidget->GetWidget());

    m_viewports.push_back(pViewportWidget);
}

void MultiEdWindow::Update() {
    SDL_Event event;
    SDL_PumpEvents();

    while(SDL_PeepEvents(&event, 1, SDL_GETEVENT, SDL_USEREVENT, SDL_LASTEVENT))
    {
        // IK_RightMouse from EnginePrivate.h
        if (event.user.code == 2)
        {
            auto action = reinterpret_cast<int64_t>(event.user.data1);

            // IST_Release = 3
            this->rightClick(event.user.windowID, action == 3, event.user.timestamp);
        }
    }

}

void MultiEdWindow::Init() {
    this->setWindowTitle("MultiEd");
    this->resize(1280, 720);


    // Temp until I make a button for it :)
    auto pref = new Components::Preferences();

    g_pEditorAPI->RegisterRightClickEvent();

    // Side Bar

    // Create a dock, and set some zones / features
    m_pDockWidget = new QDockWidget();
    m_pDockWidget->setFeatures(QDockWidget::DockWidgetMovable);
    m_pDockWidget->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    // Create our sidebar
    m_pOrderedSideBar = new Components::OrderedSideBar();
    m_pOrderedSideBar->Init();

    // Add our sidebar / dock
    m_pDockWidget->setWidget(m_pOrderedSideBar->GetWidget());
    m_pDockWidget->setMaximumWidth(m_pOrderedSideBar->GetWidget()->sizeHint().width());

    this->addDockWidget(Qt::LeftDockWidgetArea, m_pDockWidget);

    m_pFileMenu = new Components::FileMenu(this);
    m_pFileMenu->Init();

    m_pEditMenu = new Components::EditMenu(this);
    m_pEditMenu->Init();

    // Not fully implemented
    m_pViewMenu = new Components::ViewMenu(this);
    m_pViewMenu->Init();

    // Not fully implemented
    m_pBrushMenu = new Components::BrushMenu(this);
    m_pBrushMenu->Init();

    // Not fully implemented
    m_pActorMenu = new Components::ActorMenu(this);
    m_pActorMenu->Init();

    // Not fully implemented
    m_pBuildMenu = new Components::BuildMenu(this);
    m_pBuildMenu->Init();

    // Not fully implemented
    m_pToolsMenu = new Components::ToolsMenu(this);
    m_pToolsMenu->Init();

    // Not fully implemented
    m_pHelpMenu = new Components::HelpMenu(this);
    m_pHelpMenu->Init();

    m_pToolbar = new Components::ToolBar(this);
    m_pToolbar->Init();

    // Viewports!
    m_pQuadView = new ViewportSplitterContainer(this);
    m_pQuadView->Init();
    setCentralWidget(m_pQuadView);


    this->show();

}

#include "moc_MultiEdWindow.cpp"