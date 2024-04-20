//
// Created by melissaa on 2022-05-07.
//

#include "BrushBar.h"
#include "SidebarButton.h"

#include <Engine.h>
#include <EditorPrivate.h>


void Components::BrushBar::Init() {
    m_pLayout = new QGridLayout();

    auto aBuilders = g_pEditorAPI->GetBrushBuilders();

    auto nCurrentRow = 0;
    auto nCurrentColumn = 0;
    for (int i = 0; i < aBuilders.Num(); i++) {
        UBrushBuilder* pBrush = aBuilders(i);
        m_Brushes.push_back(pBrush);

        QString sBitmapName = QString::fromWCharArray(*pBrush->BitmapFilename);

        auto bSmallBMP = QFile::exists("./editorres/" + sBitmapName + ".bmp");
        auto bLargeBMP = QFile::exists("./editorres/" + sBitmapName + ".BMP");

        if (!bSmallBMP && !bLargeBMP) {
            debugf(TEXT("[BrushBar::Init] Could not retrieve .bmp of %s"), sBitmapName.toStdWString().c_str());
        } else if (bSmallBMP) {
            sBitmapName += ".bmp";
        } else {
            sBitmapName += ".BMP";
        }

        QString sTooltip = QString::fromWCharArray(*pBrush->ToolTip);

        auto icon = QIcon("./editorres/" + sBitmapName);
        auto pButton = new SidebarButton<UBrushBuilder*>(sTooltip, icon, pBrush, false,
                                                         [](UBrushBuilder* pBrush) { g_pEditorAPI->BuildBrush(pBrush); },
                                                         [](UBrushBuilder* pBrush) {
            new BrushProperties(pBrush);
        }


                                                         );//[=](UBrushBuilder* pBrush) { OpenProperties(pBrush); });

        m_pLayout->addWidget(pButton, nCurrentRow, nCurrentColumn);

        nCurrentColumn++;

        if (nCurrentColumn >= 2) {
            nCurrentColumn = 0;
            nCurrentRow++;
        }
    }
}

void Components::BrushBar::OpenProperties(UBrushBuilder *pBrush) {
    auto pBrushProps = new BrushProperties(pBrush);
}

void Components::BrushBar::OpenProps(int index) {
    auto pBrush = m_Brushes.at(index);
    auto pBrushProps = new BrushProperties(pBrush);
}
