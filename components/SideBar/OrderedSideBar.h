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

        OrderedSideBar();

        void Init() override;;

        QWidget *GetWidget() const;

    private:
        QWidget* m_pWidget;
    };
}

#endif //MULTIED_ORDEREDSIDEBAR_H
