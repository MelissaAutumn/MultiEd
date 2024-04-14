//
// Created by melissaa on 2022-04-27.
//

#ifndef MULTIED_BUILDMENU_H
#define MULTIED_BUILDMENU_H

#include <QApplication>
#include <QMenu>
#include <QMenuBar>
#include <QWidget>
#include <QMainWindow>
#include <iostream>
#include <QMessageBox>
#include <QFileDialog>
#include "BaseMenu.h"
#include "../IComponent.h"

namespace Components {
    class BuildMenu : BaseMenu {
    public:
        explicit BuildMenu(QMainWindow* pWindow) : BaseMenu(pWindow) {}

        void Init() override {
            auto pMenu = new QMenu("B&uild");

            CreateAction(pMenu, "&Play Level", [=](){ this->NotImplemented(); }, "Ctrl+P");

            // Maybe don't have this here?
            auto pMenuBar = m_pWindow->menuBar();
            pMenuBar->addMenu(pMenu);
        };

    private:

    private slots:

    };
}
#endif //MULTIED_BUILDMENU_H
