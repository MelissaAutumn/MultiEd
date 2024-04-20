//
// Created by melissaa on 20/04/24.
//
#include "ViewMenu.h"
using namespace Components;

void ViewMenu::Init() {
    auto pMenu = new QMenu("&View");

    CreateAction(pMenu, "Align Viewport Cameras to 3D", [=](){ this->NotImplemented(); }, "Ctrl+Space");
    CreateAction(pMenu, "Camera Speed +", [=](){ this->NotImplemented(); }, "Ctrl+Num+");
    CreateAction(pMenu, "Camera Speed -", [=](){ this->NotImplemented(); }, "Ctrl+Num-");



    // Maybe don't have this here?
    auto pMenuBar = m_pWindow->menuBar();
    pMenuBar->addMenu(pMenu);
}
