//
// Created by melissaa on 20/04/24.
//
#include "ActorMenu.h"
using namespace Components;

void ActorMenu::Init() {
    auto pMenu = new QMenu("Actor");

    CreateAction(pMenu, "Hide Selected Actors", [=](){ this->NotImplemented(); }, "Ctrl+H");

    // Maybe don't have this here?
    auto pMenuBar = m_pWindow->menuBar();
    pMenuBar->addMenu(pMenu);
}