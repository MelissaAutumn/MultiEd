//
// Created by melissaa on 20/04/24.
//
#include "CSGBar.h"
using namespace Components;

void CSGBar::Init() {
    m_pLayout = new QGridLayout();
    m_pLayout->addWidget(this->CreateButton("ModeAdd.bmp", "Add", []() { g_pEditorAPI->BrushAdd(); }), 0, 0);
    m_pLayout->addWidget(this->CreateButton("ModeSubtract.bmp", "Subtract", []() { g_pEditorAPI->BrushSubtract(); }), 0, 1);
    m_pLayout->addWidget(this->CreateButton("ModeIntersect.bmp", "Intersect", []() { g_pEditorAPI->BrushIntersect(); }), 1, 0);
    m_pLayout->addWidget(this->CreateButton("ModeDeintersect.bmp", "Deintersect", []() { g_pEditorAPI->BrushDeintersect(); }), 1, 1);
    m_pLayout->addWidget(this->CreateButton("AddSpecial.bmp", "Add Special Brush", []() { g_pEditorAPI->BrushSpecial(); }), 2, 0);
    m_pLayout->addWidget(this->CreateButton("AddMover.bmp", "Add Mover Brush (right click for all mover types)", []() { g_pEditorAPI->BrushAddMover(); }), 2, 1);
}
