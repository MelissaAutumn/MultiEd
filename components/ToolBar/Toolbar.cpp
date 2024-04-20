//
// Created by melissaa on 20/04/24.
//
#include "ToolBar.h"
using namespace Components;

void ToolBar::Init() {
    auto pToolbar = new QToolBar("Editor Options");
    pToolbar->addAction(CreateAction("UnrealEd_121.bmp", "New Map", [=]() { g_pEditorAPI->NewMap(); }));
    pToolbar->addAction(CreateAction("UnrealEd_414.bmp", "Open Map", [=]() { g_pHelperAPI->LoadMap(); }));
    pToolbar->addAction(CreateAction("UnrealEd_415.bmp", "Save Map", [=]() { this->NotImplemented(); }));
    pToolbar->addSeparator();
    pToolbar->addAction(CreateAction("UnrealEd_430.bmp", "Undo", [=]() { this->NotImplemented(); }));
    pToolbar->addAction(CreateAction("UnrealEd_431.bmp", "Redo", [=]() { this->NotImplemented(); }));
    pToolbar->addSeparator();
    pToolbar->addAction(CreateAction("UnrealEd_416.bmp", "Search for Actors", [=]() { this->NotImplemented(); }));
    pToolbar->addSeparator();
    pToolbar->addAction(CreateAction("UnrealEd_418.bmp", "Actor Class Browser", [=]() { this->NotImplemented(); }));
    pToolbar->addAction(CreateAction("UnrealEd_29757.bmp", "Group Browser", [=]() { this->NotImplemented(); }));
    pToolbar->addAction(CreateAction("UnrealEd_419.bmp", "Music Browser", [=]() { this->NotImplemented(); }));
    pToolbar->addAction(CreateAction("UnrealEd_420.bmp", "Sound Browser", [=]() { this->NotImplemented(); }));
    pToolbar->addAction(CreateAction("UnrealEd_417.bmp", "Texture Browser", [=]() { this->NotImplemented(); }));
    pToolbar->addAction(CreateAction("UnrealEd_421.bmp", "Mesh Browser", [=]() { this->NotImplemented(); }));
    pToolbar->addSeparator();
    pToolbar->addAction(CreateAction("UnrealEd_432.bmp", "2D Shape Editor", [=]() { this->NotImplemented(); }));
    pToolbar->addAction(CreateAction("UnrealEd_422.bmp", "UnrealScript Editor", [=]() { this->NotImplemented(); }));
    pToolbar->addSeparator();
    pToolbar->addAction(CreateAction("UnrealEd_423.bmp", "Actor Properties", [=]() { this->NotImplemented(); }));
    pToolbar->addAction(CreateAction("UnrealEd_424.bmp", "Surface Properties", [=]() { this->NotImplemented(); }));
    pToolbar->addSeparator();
    pToolbar->addAction(CreateAction("UnrealEd_425.bmp", "Build Geometry", [=]() { this->NotImplemented(); }));
    pToolbar->addAction(CreateAction("UnrealEd_426.bmp", "Build Lighting", [=]() { this->NotImplemented(); }));
    pToolbar->addAction(CreateAction("UnrealEd_427.bmp", "Build Paths", [=]() { this->NotImplemented(); }));
    pToolbar->addAction(CreateAction("UnrealEd_29764.bmp", "Build All (as per current settings)",
                                     [=]() { this->NotImplemented(); }));
    pToolbar->addAction(CreateAction("UnrealEd_428.bmp", "Build Options", [=]() { this->NotImplemented(); }));
    pToolbar->addSeparator();
    pToolbar->addAction(CreateAction("UnrealEd_429.bmp", "Play Map!", [=]() { this->NotImplemented(); }));

    m_pWindow->addToolBar(pToolbar);
}
