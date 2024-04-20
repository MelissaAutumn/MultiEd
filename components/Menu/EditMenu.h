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
        explicit EditMenu(QMainWindow* pWindow) : BaseMenu(pWindow) {
            m_sMapName = nullptr;
        }

        void Init() override;;

    private:
        QString m_sMapName;

    private slots:

        void Undo() {

        }
    };
}
#endif //MULTIED_EDITMENU_H
