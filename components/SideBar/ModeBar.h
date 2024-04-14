//
// Created by melissaa on 2022-04-24.
//

#ifndef MULTIED_MODEBAR_H
#define MULTIED_MODEBAR_H


#include <QPushButton>
#include <QGridLayout>
#include "../IComponent.h"
#include "BaseBar.h"
#include "SidebarButton.h"
#include "../../helpers/Defines.h"

namespace Components {
        class ModeBar : BaseBar {
        private:
            std::vector<SidebarButton<Helpers::EditorModes>*> m_vButtons;

        public:

            explicit ModeBar(QWidget *pParent) {
                m_pLayout = nullptr;
                m_pParent = pParent;
            };

            void ChangeEditorMode(Helpers::EditorModes mode) {
                g_pEditorAPI->SetMode(mode);
                for (auto pButton : m_vButtons) {
                    pButton->UpdateState(mode);
                }
            }

            void Init() override {
                m_pLayout = new QGridLayout();

                struct CreateData {
                    QString toolTip;
                    QIcon icon;
                    Helpers::EditorModes mode;
                };

                std::vector<CreateData> vData = {
                    { "Camera Movement", QIcon("./editorres/ModeCamera.bmp"), Helpers::EditorModes::EM_ViewportMove },
                    { "Vertex Editing", QIcon("./editorres/ModeVertex.bmp"), Helpers::EditorModes::EM_VertexEdit },
                    { "Brush Sheering", QIcon("./editorres/ModeSheer.bmp"), Helpers::EditorModes::EM_BrushSheer },
                    { "Actor Scaling", QIcon("./editorres/ModeScale.bmp"), Helpers::EditorModes::EM_BrushScale },
                    { "Actor Stretching", QIcon("./editorres/ModeStretch.bmp"), Helpers::EditorModes::EM_BrushStretch },
                    { "Snapped Brush Scaling", QIcon("./editorres/ModeSnapScale.bmp"), Helpers::EditorModes::EM_BrushSnap },
                    { "Brush Rotate", QIcon("./editorres/ModeRotate.bmp"), Helpers::EditorModes::EM_BrushRotate },
                    { "Texture Pan", QIcon("./editorres/TexturePan.bmp"), Helpers::EditorModes::EM_TexturePan },
                    { "Texture Rotate", QIcon("./editorres/TextureRotate.bmp"), Helpers::EditorModes::EM_TextureRotate },
                    { "Brush Clipping", QIcon("./editorres/ModeBrushClip.bmp"), Helpers::EditorModes::EM_BrushClip },
                };

                int row = 0;
                int column = 0;
                for (const auto& pData : vData) {
                    auto pButton = new SidebarButton<Helpers::EditorModes>(pData.toolTip, pData.icon, pData.mode, false, [=](Helpers::EditorModes mode) { ChangeEditorMode(mode); });
                    m_vButtons.push_back(pButton);
                    m_pLayout->addWidget(pButton, row, column);

                    column++;

                    if (column >= 2) {
                        column = 0;
                        row++;
                    }
                }

                ChangeEditorMode(Helpers::EditorModes::EM_ViewportMove);
            }

            QGridLayout *GetLayout() override { return m_pLayout; }
        };
}

#endif //MULTIED_MODEBAR_H
