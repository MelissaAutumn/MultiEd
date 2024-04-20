//
// Created by melissaa on 20/04/24.
//
#include "MiscBar.h"
using namespace Components;

void MiscBar::ChangeCameraSpeed(QPushButton *pCameraSpeed) {
    auto currentSpeed = g_pEditorAPI->GetCameraSpeed();

    QIcon icon;

    // Select the "next" camera speed
    if (currentSpeed == 1) {
        g_pEditorAPI->CameraSpeed(4);
        icon.addFile("./MultiEd/exe/UnrealEd_436.bmp");
    }
    else if (currentSpeed == 4) {
        g_pEditorAPI->CameraSpeed(16);
        icon.addFile("./MultiEd/exe/UnrealEd_437.bmp");
    }
    else {
        g_pEditorAPI->CameraSpeed(1);
        icon.addFile("./MultiEd/exe/UnrealEd_435.bmp");
    }

    pCameraSpeed->setIcon(icon);
}

void MiscBar::Init() {
    m_pCameraSpeed = this->CreateButton("AlignCameras.bmp", "Change Camera Speed", [=]() { Components::MiscBar::ChangeCameraSpeed(m_pCameraSpeed); });
    // Haha this sucks
    QIcon icon("./MultiEd/exe/UnrealEd_436.bmp");
    m_pCameraSpeed->setIcon(icon);

    m_pLayout = new QGridLayout();
    m_pLayout->addWidget(this->CreateButton("ShowSelected.bmp", "Show Selected Actors Only", []() { g_pEditorAPI->ActorShowSelected(); }), 0, 0);
    m_pLayout->addWidget(this->CreateButton("HideSelected.bmp", "Hide Selected Actors", []() { g_pEditorAPI->ActorHideSelected(); }), 0, 1);
    m_pLayout->addWidget(this->CreateButton("SelectInside.bmp", "Show Inside Actors", []() { g_pEditorAPI->ActorSelectInside(); }), 1, 0);
    m_pLayout->addWidget(this->CreateButton("SelectAll.bmp", "Select All Actors", []() { g_pEditorAPI->ActorSelectAll(); }), 1, 1);
    m_pLayout->addWidget(this->CreateButton("InvertSelections.bmp", "Invert Selection", []() { g_pEditorAPI->ActorSelectInvert(); }), 2, 0);
    m_pLayout->addWidget(this->CreateButton("ShowAll.bmp", "Show All Actors", []() { g_pEditorAPI->ActorShowAll(); }), 2, 1);
    m_pLayout->addWidget(m_pCameraSpeed, 3, 0);
    m_pLayout->addWidget(this->CreateButton("AlignCameras.bmp", "Align Viewport Cameras to 3D Viewport Camera", []() { g_pEditorAPI->CameraAlign(); }),3, 1);
}
