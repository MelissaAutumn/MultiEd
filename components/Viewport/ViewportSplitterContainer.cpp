//
// Created by melissaa on 2022-05-02.
//

#include <QVBoxLayout>
#include "ViewportSplitterContainer.h"


using namespace Components;

void ViewportSplitterContainer::Init() {

    m_pHSplitter1 = new ViewportSplitter(this);
    m_pHSplitter2 = new ViewportSplitter(this);
    m_pVSplitter = new QSplitter(Qt::Vertical, this);

    m_pHSplitter1->setHandleWidth(4);
    m_pHSplitter2->setHandleWidth(4);
    m_pVSplitter->setHandleWidth(4);

    ViewportSplitterContainer::connect(m_pHSplitter1, &ViewportSplitter::splitterMoved, this, [=](int pos, int index) { this->OnMove(m_pHSplitter1, pos, index); });
    ViewportSplitterContainer::connect(m_pHSplitter2, &ViewportSplitter::splitterMoved, this, [=](int pos, int index) { this->OnMove(m_pHSplitter2, pos, index); });

    m_pVSplitter->addWidget(m_pHSplitter1);
    m_pVSplitter->addWidget(m_pHSplitter2);

    auto pLayout = new QVBoxLayout();
    pLayout->addWidget(m_pVSplitter);
    this->setLayout(pLayout);
}


void ViewportSplitterContainer::OnMove(ViewportSplitter *pSplitter, int pos, int index) {
    ViewportSplitter* pThis = pSplitter == m_pHSplitter1 ? m_pHSplitter2 : m_pHSplitter1;
    if (!pThis) {
        return;
    }
    pThis->moveSplitterProxy(pos, index);
}

void ViewportSplitterContainer::AddWidget(QWidget *pWidget) {
    if (!m_pHSplitter1 || !m_pHSplitter2) {
        return;
    }

    if (m_pHSplitter1->count() < 2) {
        m_pHSplitter1->addWidget(pWidget);
    }
    else if (m_pHSplitter2->count() < 2) {
        m_pHSplitter2->addWidget(pWidget);
    }
}
