//
// Created by melissaa on 2022-04-27.
//

#ifndef MULTIED_ACTORMENU_H
#define MULTIED_ACTORMENU_H

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
    class ActorMenu : BaseMenu {
    public:
        explicit ActorMenu(QMainWindow* pWindow) : BaseMenu(pWindow) {}

        void Init() override {
            auto pMenu = new QMenu("Actor");

            CreateAction(pMenu, "Hide Selected Actors", [=](){ this->NotImplemented(); }, "Ctrl+H");

            // Maybe don't have this here?
            auto pMenuBar = m_pWindow->menuBar();
            pMenuBar->addMenu(pMenu);
        };

    private:

    private slots:

    };
}
#endif //MULTIED_ACTORMENU_H
