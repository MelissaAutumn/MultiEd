//
// Created by melissaa on 2022-05-02.
//
#include <QFileDialog>

#include "Services.h"
#include "HelperAPI.h"



using namespace Services;


void Services::HelperAPI::LoadMap() {

        auto file = QFileDialog::getOpenFileName(
                nullptr,
                "Load Map",
                "../Maps",
                "Map Files (*.unr)", nullptr, QFileDialog::DontUseNativeDialog);

        if(file.isEmpty()) {
            return;
        }

        g_pEditorAPI->LoadMap(file.toStdWString().c_str());

}

