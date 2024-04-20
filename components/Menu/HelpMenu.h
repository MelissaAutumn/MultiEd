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

        void Init() override;;

    private:

    private slots:

        void LaunchOldUnrealForum();

        void LaunchOldUnrealUEDForum();

        void LaunchEpicGames();

        void LaunchOldUnreal();

        void LaunchGitRepo();

        void LaunchGitWiki();

        void About();
    };
}
#endif //MULTIED_HELPMENU_H
