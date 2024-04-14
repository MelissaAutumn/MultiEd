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
#ifdef _DEBUG
                "/mnt/nvme/Games/UT99/Maps/",
#else
                "../Maps",
#endif
                "Map Files (*.unr)");

        if(files.isEmpty()) {
            return;
        }

        g_pEditorAPI->LoadMap(files.first().toStdWString().c_str());

}

