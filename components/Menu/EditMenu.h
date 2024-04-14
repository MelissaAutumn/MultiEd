//
// Created by melissaa on 2022-04-26.
//

#ifndef MULTIED_EDITMENU_H
#define MULTIED_EDITMENU_H

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
    class EditMenu : BaseMenu {
    public:
        EditMenu(QMainWindow* pWindow) : BaseMenu(pWindow) {
            m_sMapName = nullptr;
        }

        void Init() override {
            auto pMenu = new QMenu("&Edit");

            CreateAction(pMenu, "&Undo", [=](){ this->NotImplemented(); }, "Ctrl+Z");
            CreateAction(pMenu, "&Redo", [=](){ this->NotImplemented(); }, "Ctrl+Y");
            pMenu->addSeparator();
            CreateAction(pMenu, "&Search for Actors...", [=](){ this->NotImplemented(); }, "Ctrl+F");
            pMenu->addSeparator();
            CreateAction(pMenu, "Cu&t", [=](){ this->NotImplemented(); }, "Ctrl+X");
            CreateAction(pMenu, "&Copy", [=](){ this->NotImplemented(); }, "Ctrl+C");
            CreateAction(pMenu, "&Paste", [=](){ this->NotImplemented(); }, "Ctrl+V");
            pMenu->addSeparator();
            CreateAction(pMenu, "D&uplicate", [=](){ this->NotImplemented(); }, "Ctrl+D");
            CreateAction(pMenu, "&Delete", [=](){ this->NotImplemented(); }, "Del");
            pMenu->addSeparator();
            CreateAction(pMenu, "Select &None", [=](){ this->NotImplemented(); }, "Ctrl+Q");
            CreateAction(pMenu, "Select Al&l Actors", [=](){ this->NotImplemented(); }, "Ctrl+A");

            auto pSurfaceMenu = pMenu->addMenu("&Select Surfaces");
            CreateAction(pSurfaceMenu, "Select Al&l Surfaces", [=](){ this->NotImplemented(); }, "Ctrl+Shift+A");
            CreateAction(pSurfaceMenu, "Matching G&roups", [=](){ this->NotImplemented(); }, "Ctrl+G");
            CreateAction(pSurfaceMenu, "Matching I&tems", [=](){ this->NotImplemented(); }, "Ctrl+I");
            CreateAction(pSurfaceMenu, "Matching B&rush", [=](){ this->NotImplemented(); }, "Ctrl+B");
            CreateAction(pSurfaceMenu, "Matching T&exture", [=](){ this->NotImplemented(); }, "Ctrl+T");
            CreateAction(pSurfaceMenu, "Matching P&olyFlags", [=](){ this->NotImplemented(); }, "Ctrl+P");
            pSurfaceMenu->addSeparator();
            CreateAction(pSurfaceMenu, "All Ad&jacents", [=](){ this->NotImplemented(); }, "Ctrl+J");
            CreateAction(pSurfaceMenu, "Adjacent C&oplanars", [=](){ this->NotImplemented(); }, "Ctrl+C");
            CreateAction(pSurfaceMenu, "Adjacent W&alls", [=](){ this->NotImplemented(); }, "Ctrl+W");
            CreateAction(pSurfaceMenu, "Adjacent F&loors/Ceilings", [=](){ this->NotImplemented(); }, "Ctrl+F");
            CreateAction(pSurfaceMenu, "Adjacent S&lants", [=](){ this->NotImplemented(); }, "Ctrl+S");
            pSurfaceMenu->addSeparator();
            CreateAction(pSurfaceMenu, "Reverse", [=](){ this->NotImplemented(); }, "Ctrl+Q");
            pSurfaceMenu->addSeparator();
            CreateAction(pSurfaceMenu, "&Memory Set", [=](){ this->NotImplemented(); }, "Ctrl+M");
            CreateAction(pSurfaceMenu, "&Recall Memory", [=](){ this->NotImplemented(); }, "Ctrl+R");
            CreateAction(pSurfaceMenu, "&OR With Memory", [=](){ this->NotImplemented(); }, "Ctrl+O");
            CreateAction(pSurfaceMenu, "&AND With Memory", [=](){ this->NotImplemented(); }, "Ctrl+U");
            CreateAction(pSurfaceMenu, "&XOR With Memory", [=](){ this->NotImplemented(); }, "Ctrl+X");
            
            // Maybe don't have this here?
            auto pMenuBar = m_pWindow->menuBar();
            pMenuBar->addMenu(pMenu);
        };

    private:
        QString m_sMapName;

    private slots:

        void Undo() {

        }
    };
}
#endif //MULTIED_EDITMENU_H
