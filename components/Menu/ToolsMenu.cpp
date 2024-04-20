//
// Created by melissaa on 20/04/24.
//
#include "ToolsMenu.h"
using namespace Components;

void ToolsMenu::Init() {
    auto pMenu = new QMenu("&Tools");

    CreateAction(pMenu, "Advanced Options", [=](){ this->NotImplemented(); }, "Ctrl+1");

    // Maybe don't have this here?
    auto pMenuBar = m_pWindow->menuBar();
    pMenuBar->addMenu(pMenu);
}
