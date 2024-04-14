//
// Created by melissaa on 2022-04-29.
//

#include "MultiEdWindow.h"
#include "BrushProperties/BrushProperties.h"
#include "Preferences/Preferences.h"

using namespace Components;

void MultiEdWindow::LoadAndSetRandomSplash(QMainWindow *pViewport) {
    auto splashes = QStringList();
    splashes.push_back("./MultiEd/splash/UnrealTournamentLogo.bmp");
    splashes.push_back("./MultiEd/splash/Splash0.bmp");
    splashes.push_back("./MultiEd/splash/Splash1.bmp");
    splashes.push_back("./MultiEd/splash/Splash2.bmp");
    splashes.push_back("./MultiEd/splash/Splash3.bmp");
    splashes.push_back("./MultiEd/splash/Splash4.bmp");

    srand(time(nullptr));
    auto index = 0 + rand() % (splashes.count() - 1);

    auto image      = QPixmap(splashes[index]);
    auto imageLabel = new QLabel();
    // Lol no aspect ratio preserving scale.
    imageLabel->setScaledContents(true);
    imageLabel->setAlignment(Qt::AlignCenter);
    imageLabel->setPixmap(image);


    pViewport->setCentralWidget(imageLabel);
}

void MultiEdWindow::SetViewport(QWidget* pWidget, WId nWindowID, Helpers::ViewportModes nMode) {

    auto pViewportWidget = new Components::Viewport(this, nWindowID, nMode);
    pViewportWidget->Init();

    m_pViewport = pViewportWidget->GetWidget();
    m_pViewport->setCentralWidget(pWidget);
    m_pQuadView->AddWidget(pViewportWidget->GetWidget());
}

void MultiEdWindow::Init() {
    this->setWindowTitle("MultiEd");
    this->resize(1280, 720);


    // Temp until I make a button for it :)
    auto pref = new Components::Preferences();


    // Side Bar

    // Create a dock, and set some zones / features
    auto pDock = new QDockWidget();
    pDock->setFeatures(QDockWidget::DockWidgetMovable);
    pDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    // Create our sidebar
    auto pOrderedSideBar = new Components::OrderedSideBar();
    pOrderedSideBar->Init();

    // Add our sidebar / dock
    pDock->setWidget(pOrderedSideBar->GetWidget());
    pDock->setMaximumWidth(pOrderedSideBar->GetWidget()->sizeHint().width());

    this->addDockWidget(Qt::LeftDockWidgetArea, pDock);

    auto pFileMenu = new Components::FileMenu(this);
    pFileMenu->Init();

    auto pEditMenu = new Components::EditMenu(this);
    pEditMenu->Init();

    // Not fully implemented
    auto pViewMenu = new Components::ViewMenu(this);
    pViewMenu->Init();

    // Not fully implemented
    auto pBrushMenu = new Components::BrushMenu(this);
    pBrushMenu->Init();

    // Not fully implemented
    auto pActorMenu = new Components::ActorMenu(this);
    pActorMenu->Init();

    // Not fully implemented
    auto pBuildMenu = new Components::BuildMenu(this);
    pBuildMenu->Init();

    // Not fully implemented
    auto pToolsMenu = new Components::ToolsMenu(this);
    pToolsMenu->Init();

    // Not fully implemented
    auto pHelpMenu = new Components::HelpMenu(this);
    pHelpMenu->Init();

    auto pToolbar = new Components::ToolBar(this);
    pToolbar->Init();

    // Viewports!
    m_pQuadView = new ViewportSplitterContainer(this);
    m_pQuadView->Init();
    setCentralWidget(m_pQuadView);


    this->show();

}

void MultiEdWindow::resizeEvent(QResizeEvent * pResize) {
#if 0
    if (!m_pSDLWindow) { return; }
    SDL_SetWindowSize(m_pSDLWindow, m_pViewport->size().width(), m_pViewport->size().height()-42);
    SDL_RaiseWindow(m_pSDLWindow);
#endif

}

void MultiEdWindow::moveEvent(QMoveEvent *event) {
#if 0
    if (!m_pSDLWindow) { return; }
    auto pos = this->pos() + m_pViewport->pos();

    pos.setY(pos.y() + 72);

    SDL_SetWindowPosition(m_pSDLWindow, pos.x(), pos.y());
    SDL_SetWindowSize(m_pSDLWindow, m_pViewport->size().width(), m_pViewport->size().height()-42);
    SDL_RaiseWindow(m_pSDLWindow);
#endif
}


