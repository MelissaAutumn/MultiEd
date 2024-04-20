//
// Created by melissaa on 2022-04-27.
//

#ifndef MULTIED_BRUSHMENU_H
#define MULTIED_BRUSHMENU_H

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
    class BrushMenu : BaseMenu {
    public:
        explicit BrushMenu(QMainWindow* pWindow) : BaseMenu(pWindow) {}

        void Init() override;;

    private:
    private slots:
    };
}
#endif //MULTIED_BRUSHMENU_H
