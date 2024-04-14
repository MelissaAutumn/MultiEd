//
// Created by melissaa on 2022-04-24.
//

#ifndef MULTIED_CSGBAR_H
#define MULTIED_CSGBAR_H


#include "BaseBar.h"
namespace Components {
        class CSGBar : public BaseBar {

        public:

            explicit CSGBar(QWidget *pParent) {
                m_pLayout = nullptr;
                m_pParent = pParent;
            };

            void Init() override {
                m_pLayout = new QGridLayout();
                m_pLayout->addWidget(this->CreateButton("ModeAdd.bmp", "Add", []() { g_pEditorAPI->BrushAdd(); }), 0, 0);
                m_pLayout->addWidget(this->CreateButton("ModeSubtract.bmp", "Subtract", []() { g_pEditorAPI->BrushSubtract(); }), 0, 1);
                m_pLayout->addWidget(this->CreateButton("ModeIntersect.bmp", "Intersect", []() { g_pEditorAPI->BrushIntersect(); }), 1, 0);
                m_pLayout->addWidget(this->CreateButton("ModeDeintersect.bmp", "Deintersect", []() { g_pEditorAPI->BrushDeintersect(); }), 1, 1);
                m_pLayout->addWidget(this->CreateButton("AddSpecial.bmp", "Add Special Brush", []() { g_pEditorAPI->BrushSpecial(); }), 2, 0);
                m_pLayout->addWidget(this->CreateButton("AddMover.bmp", "Add Mover Brush (right click for all mover types)", []() { g_pEditorAPI->BrushAddMover(); }), 2, 1);
            }

            QGridLayout *GetLayout() override { return m_pLayout; }
        };
}

#endif //MULTIED_CSGBAR_H
