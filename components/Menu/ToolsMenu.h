//
// Created by melissaa on 2022-04-27.
//

#ifndef MULTIED_TOOLSMENU_H
#define MULTIED_TOOLSMENU_H

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
    class ToolsMenu : BaseMenu {
    public:
        explicit ToolsMenu(QMainWindow* pWindow) : BaseMenu(pWindow) {}

        void Init() override {
            auto pMenu = new QMenu("&Tools");

            CreateAction(pMenu, "Advanced Options", [=](){ this->NotImplemented(); }, "Ctrl+1");

            // Maybe don't have this here?
            auto pMenuBar = m_pWindow->menuBar();
            pMenuBar->addMenu(pMenu);
        };

    private:

    private slots:

    };
}
#endif //MULTIED_TOOLSMENU_H
