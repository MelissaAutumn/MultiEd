//
// Created by melissaa on 20/04/24.
//
#include "ClipBar.h"
using namespace Components;

void ClipBar::Init() {
    m_pLayout = new QGridLayout();
    m_pLayout->addWidget(this->CreateButton("BrushClip.bmp", "Clip Selected Brushes", []() { g_pEditorAPI->BrushClip(); }), 0, 0);
    m_pLayout->addWidget(this->CreateButton("BrushClipSplit.bmp", "Split Selected Brushes", []() { g_pEditorAPI->BrushSplit(); }), 0, 1);
    m_pLayout->addWidget(this->CreateButton("BrushClipFlip.bmp", "Flip Clipping Normal", []() { g_pEditorAPI->BrushFlip(); }), 1, 0);
    m_pLayout->addWidget(this->CreateButton("BrushClipDelete.bmp", "Delete Clipping Markers", []() { g_pEditorAPI->BrushDelete(); }), 1, 1);
}
