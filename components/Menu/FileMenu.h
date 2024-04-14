//
// Created by melissaa on 2022-04-26.
//

#ifndef MULTIED_FILEMENU_H
#define MULTIED_FILEMENU_H

#include <QApplication>
#include <QMenu>
#include <QMenuBar>
#include <QWidget>
#include <QMainWindow>
#include <QMessageBox>
#include <QFileDialog>
#include <iostream>
#include "../IComponent.h"
#include "BaseMenu.h"

namespace Components {
    // Need QObject for connecting, I may make components entirely based off of this in the future.
    class FileMenu : public BaseMenu {
    public:
        FileMenu(QMainWindow* pWindow) : BaseMenu(pWindow) {
            m_sMapName = nullptr;
        }

        void Init() override {
            auto pMenu = new QMenu("&File");

            CreateAction(pMenu, "&New", [=](){ this->New(); }, "Ctrl+N");
            CreateAction(pMenu, "&Open", [=](){ this->Open(); }, "Ctrl+O");

            CreateAction(pMenu, "&Save", [=](){ this->Save(); }, "Ctrl+S");
            CreateAction(pMenu, "Save &As", [=](){ this->SaveAs(); }, "Ctrl+Shift+S");

            CreateAction(pMenu, "&Import", [=](){ this->Import(); });
            CreateAction(pMenu, "&Export", [=](){ this->Export(); });

            CreateAction(pMenu, "E&xit", [=](){ this->Exit(); });

            // Maybe don't have this here?
            auto pMenuBar = m_pWindow->menuBar();
            pMenuBar->addMenu(pMenu);

        };

    private:

        QString m_sMapName;

    private slots:
        void New();
        void Open();
        void Save() {
            if (m_sMapName == nullptr) {
                SaveAs();
                return;
            }
        }
        void SaveAs() {
            QString file = QFileDialog::getSaveFileName(
                    m_pWindow,
                    "Save Map",
#ifdef _DEBUG
                    "/mnt/nvme/Games/UT99/Maps/",
#else
                    "../Maps",
#endif
                    "Map Files (*.unr)");

            // Do something with those files!
            // Otherwise save!
            m_sMapName = file;
            m_pWindow->setWindowTitle("Multi Editor - ["+m_sMapName+"]");
        }
        void Import() {
            NotImplemented();
        }
        void Export() {
            NotImplemented();
        }
        void Exit() {
            QApplication::exit(0);
        }
    };
}


#endif //MULTIED_FILEMENU_H
