//
// Created by melissaa on 2022-05-02.
//
#include <QFileDialog>

#include "Services.h"
#include "HelperAPI.h"



using namespace Services;


void Services::HelperAPI::LoadMap() {

        QStringList files = QFileDialog::getOpenFileNames(
                nullptr,
                "Load Map",
                "../Maps",
                "Map Files (*.unr)");

        if(files.isEmpty()) {
            return;
        }

        g_pEditorAPI->LoadMap(files.first().toStdWString().c_str());

}

