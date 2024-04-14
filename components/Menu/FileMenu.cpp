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
