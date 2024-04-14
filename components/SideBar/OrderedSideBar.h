//
// Created by melissaa on 2022-04-26.
//

#ifndef MULTIED_ORDEREDSIDEBAR_H
#define MULTIED_ORDEREDSIDEBAR_H

#include <QWidget>
#include <QVBoxLayout>
#include "../IComponent.h"

#include "ModeBar.h"
#include "ClipBar.h"
#include "BrushBar.h"
#include "CSGBar.h"
#include "MiscBar.h"

#include "../../services/Services.h"


namespace Components {

    class OrderedSideBar : IComponent, public QObject {
    public:

        OrderedSideBar() {}

        void Init() override {
            m_pWidget = new QWidget;
            auto* pVBar = new QVBoxLayout(m_pWidget);


            // Create and init our subcomponents
            auto* pModeBar = new Components::ModeBar(nullptr);
            pModeBar->Init();
            auto* pClipBar = new Components::ClipBar(nullptr);
            pClipBar->Init();
            auto* pBrushBar = new Components::BrushBar(nullptr);
            pBrushBar->Init();
            auto* pCSGBar = new Components::CSGBar(nullptr);
            pCSGBar->Init();
            auto *pMiscBar = new Components::MiscBar(nullptr);
            pMiscBar->Init();

            // Add the first subcomponent
            pVBar->addLayout(pModeBar->GetLayout());

            // Calculate the width and add a spacer
            auto nBarWidth = pVBar->sizeHint().width();
            auto pSpacer = new QSpacerItem(nBarWidth, 24);
            pVBar->addSpacerItem(pSpacer);

            // Add the rest
            pVBar->addLayout(pClipBar->GetLayout());
            pVBar->addSpacerItem(pSpacer);

            pVBar->addLayout(pBrushBar->GetLayout());
            pVBar->addSpacerItem(pSpacer);

            pVBar->addLayout(pCSGBar->GetLayout());
            pVBar->addSpacerItem(pSpacer);

            pVBar->addLayout(pMiscBar->GetLayout());

        };

        QWidget *GetWidget() { return m_pWidget; }
    private:
        QWidget* m_pWidget;
    };
}

#endif //MULTIED_ORDEREDSIDEBAR_H
