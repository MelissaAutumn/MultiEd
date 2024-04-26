//
// Created by melissaa on 20/04/24.
//
#include "OrderedSideBar.h"
using namespace Components;

OrderedSideBar::OrderedSideBar(): m_pWidget(nullptr) {
}

void OrderedSideBar::Init() {
    m_pWidget   = new QWidget;
    auto* pVBar = new QVBoxLayout(m_pWidget);
    pVBar->setAlignment(Qt::AlignmentFlag::AlignTop | Qt::AlignmentFlag::AlignHCenter);

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
    auto pSpacer   = new QSpacerItem(nBarWidth, 24);
    pVBar->addSpacerItem(pSpacer);

    // Add the rest
    pVBar->addLayout(pClipBar->GetLayout());
    pVBar->addSpacerItem(pSpacer);

    pVBar->addLayout(pBrushBar->GetLayout());
    pVBar->addSpacerItem(pSpacer);

    pVBar->addLayout(pCSGBar->GetLayout());
    pVBar->addSpacerItem(pSpacer);

    pVBar->addLayout(pMiscBar->GetLayout());

}

QWidget * OrderedSideBar::GetWidget() const { return m_pWidget; }
