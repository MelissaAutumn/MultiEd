//
// Created by melissaa on 20/04/24.
//
#include "BrushMenu.h"

using namespace Components;

void BrushMenu::Init() {
    auto pMenu = new QMenu("&Brush");

    auto pBrushClipMenu = pMenu->addMenu("Brush Clip");
    CreateAction(pBrushClipMenu, "&Clip", [=](){ this->NotImplemented(); });

    // Maybe don't have this here?
    auto pMenuBar = m_pWindow->menuBar();
    pMenuBar->addMenu(pMenu);
}
