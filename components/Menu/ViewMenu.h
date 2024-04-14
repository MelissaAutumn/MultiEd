//
// Created by melissaa on 2022-04-27.
//

#ifndef MULTIED_VIEWMENU_H
#define MULTIED_VIEWMENU_H

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
    class ViewMenu : BaseMenu {
    public:
        explicit ViewMenu(QMainWindow* pWindow) : BaseMenu(pWindow) {}

        void Init() override {
            auto pMenu = new QMenu("&View");

            CreateAction(pMenu, "Align Viewport Cameras to 3D", [=](){ this->NotImplemented(); }, "Ctrl+Space");
            CreateAction(pMenu, "Camera Speed +", [=](){ this->NotImplemented(); }, "Ctrl+Num+");
            CreateAction(pMenu, "Camera Speed -", [=](){ this->NotImplemented(); }, "Ctrl+Num-");



            // Maybe don't have this here?
            auto pMenuBar = m_pWindow->menuBar();
            pMenuBar->addMenu(pMenu);
        };

    private:

    private slots:

    };
}
#endif //MULTIED_VIEWMENU_H
