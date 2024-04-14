//
// Created by melissaa on 2022-04-27.
//

#ifndef MULTIED_HELPMENU_H
#define MULTIED_HELPMENU_H

#include <QApplication>
#include <QMenu>
#include <QMenuBar>
#include <QWidget>
#include <QMainWindow>
#include <iostream>
#include <QMessageBox>
#include <QFileDialog>
#include <QDesktopServices>
#include "BaseMenu.h"
#include "../IComponent.h"

namespace Components {
    class HelpMenu : BaseMenu {
    public:
        explicit HelpMenu(QMainWindow* pWindow) : BaseMenu(pWindow) {}

        void Init() override {
            auto pMenu = new QMenu("&Help");

            CreateAction(pMenu, "&MultiEd Help", [=](){ this->NotImplemented(); }, "F1");
            pMenu->addSeparator();
            CreateAction(pMenu, "&Technical Support", [=](){ this->LaunchOldUnrealForum(); });
            CreateAction(pMenu, "Old&unreal UEd2.2 Forum", [=](){ this->LaunchOldUnrealUEDForum(); }, "F2");
            CreateAction(pMenu, "&Epic Games Web Site", [=](){ this->LaunchEpicGames(); });
            CreateAction(pMenu, "&Oldunreal Website", [=](){ this->LaunchOldUnreal(); });
            pMenu->addSeparator();
            CreateAction(pMenu, "&About MultiEd", [=](){ this->About(); });

            // Maybe don't have this here?
            auto pMenuBar = m_pWindow->menuBar();
            pMenuBar->addMenu(pMenu);
        };

    private:

    private slots:

        void LaunchOldUnrealForum() {
            const QString sURL = "https://www.oldunreal.com/cgi-bin/yabb2/YaBB.pl";
            QDesktopServices::openUrl(sURL);
        }

        void LaunchOldUnrealUEDForum() {
            const QString sURL = "https://www.oldunreal.com/cgi-bin/yabb2/YaBB.pl?board=UnrealEDTwoPointOne";
            QDesktopServices::openUrl(sURL);
        }

        void LaunchEpicGames() {
            const QString sURL = "www.epicgames.com";
            QDesktopServices::openUrl(sURL);
        }

        void LaunchOldUnreal() {
            const QString sURL = "https://www.oldunreal.com/";
            QDesktopServices::openUrl(sURL);
        }

        void About() {
            QMessageBox::about(m_pWindow, "About MultiEd", "MultiEd is based off of UnrealEd 2.2.\nBuilt by Melissa A.\n\nOriginal UnrealEd version copyright 2000, Epic Games Inc.\nUpdated to UnrealEd 2.2 by www.oldunreal.com.");
        }

    };
}
#endif //MULTIED_HELPMENU_H
