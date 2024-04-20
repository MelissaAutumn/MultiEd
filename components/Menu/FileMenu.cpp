//
// Created by melissaa on 2022-05-01.
//
#include "FileMenu.h"
#include "../../services/Services.h"

using namespace Components;

void FileMenu::New() {
        // No dialog, just new map!
        m_sMapName = "";
        //m_pWindow->setWindowTitle("Multi Editor");
        g_pEditorAPI->NewMap();
}

void FileMenu::Open()  {
    g_pHelperAPI->LoadMap();
}

void FileMenu::Init() {
    auto pMenu = new QMenu("&File");

    CreateAction(pMenu, "&New", [=](){ this->New(); }, "Ctrl+N");
    CreateAction(pMenu, "&Open", [=](){ this->Open(); }, "Ctrl+O");

    CreateAction(pMenu, "&Save", [=](){ this->Save(); }, "Ctrl+S");
    CreateAction(pMenu, "Save &As", [=](){ this->SaveAs(); }, "Ctrl+Shift+S");

    CreateAction(pMenu, "&Import", [=](){ this->Import(); });
    CreateAction(pMenu, "&Export", [=](){ this->Export(); });

    CreateAction(pMenu, "E&xit", [=](){ this->Exit(); });

    // Maybe don't have this here?
    auto pMenuBar = m_pWindow->menuBar();
    pMenuBar->addMenu(pMenu);

}
