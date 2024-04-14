//
// Created by melissaa on 2022-04-24.
//

#ifndef MULTIED_CLIPBAR_H
#define MULTIED_CLIPBAR_H


#include "BaseBar.h"

namespace Components {
        class ClipBar : BaseBar {
        public:
            explicit ClipBar(QWidget *pParent) {
                m_pLayout = nullptr;
                m_pParent = pParent;
            };

            void Init() override {
                m_pLayout = new QGridLayout();
                m_pLayout->addWidget(this->CreateButton("BrushClip.bmp", "Clip Selected Brushes", []() { g_pEditorAPI->BrushClip(); }), 0, 0);
                m_pLayout->addWidget(this->CreateButton("BrushClipSplit.bmp", "Split Selected Brushes", []() { g_pEditorAPI->BrushSplit(); }), 0, 1);
                m_pLayout->addWidget(this->CreateButton("BrushClipFlip.bmp", "Flip Clipping Normal", []() { g_pEditorAPI->BrushFlip(); }), 1, 0);
                m_pLayout->addWidget(this->CreateButton("BrushClipDelete.bmp", "Delete Clipping Markers", []() { g_pEditorAPI->BrushDelete(); }), 1, 1);
            }

            QGridLayout *GetLayout() override { return m_pLayout; }
        };
}

#endif //MULTIED_CLIPBAR_H
