//
// Created by melissaa on 2022-05-02.
//

#ifndef MULTIED_VIEWPORTSPLITTERCONTAINER_H
#define MULTIED_VIEWPORTSPLITTERCONTAINER_H

#include <QWidget>
#include <QSplitter>
#include "ViewportSplitter.h"
#include "../IComponent.h"


namespace Components {
    class ViewportSplitterContainer : IComponent, public QWidget {
    public:
        explicit ViewportSplitterContainer(QWidget* parent) : QWidget(parent) {
            m_pHSplitter1 = nullptr;
            m_pHSplitter2 = nullptr;
            m_pVSplitter = nullptr;
        }
        void Init() override;
        void AddWidget(QWidget* pWidget);

    private:

        ViewportSplitter* m_pHSplitter1{};
        ViewportSplitter* m_pHSplitter2{};
        QSplitter* m_pVSplitter{};

    private slots:
        void OnMove(ViewportSplitter* pSplitter, int pos, int index);

    };
}

#endif //MULTIED_VIEWPORTSPLITTERCONTAINER_H
