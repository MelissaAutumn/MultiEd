//
// Created by melissaa on 20/04/24.
//
#include "BuildMenu.h"
using namespace Components;

void BuildMenu::Init() {
    auto pMenu = new QMenu("B&uild");

    CreateAction(pMenu, "&Play Level", [=](){ this->NotImplemented(); }, "Ctrl+P");

    // Maybe don't have this here?
    auto pMenuBar = m_pWindow->menuBar();
    pMenuBar->addMenu(pMenu);
}
