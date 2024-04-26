//
// Created by melissaa on 2022-04-26.
//

#ifndef MULTIED_BASEMENU_H
#define MULTIED_BASEMENU_H
#include <QApplication>
#include <QMenu>
#include <QMenuBar>
#include <QWidget>
#include <QMainWindow>
#include <iostream>
#include <QMessageBox>
#include <QFileDialog>
#include "../IComponent.h"



namespace Components {
    class BaseMenu : IComponent, public QObject {
    public:
        explicit BaseMenu(QMainWindow* pWindow) {
            m_pWindow = pWindow;
        }

    protected:
        template<typename ActionFunction>
        void CreateAction(QMenu* pMenu, const QString& sText, const ActionFunction f, const QString& sShortcut = "") {
            // Lol! This isn't great.
            // Instead of explicitly defining these, why not stuff them in a hashmap, keyed by name?
            auto pAction = new QAction(sText, m_pWindow);
            if (!sShortcut.isEmpty()) {
                pAction->setShortcut(QKeySequence::fromString(sShortcut));
            }
            QMainWindow::connect(pAction, &QAction::triggered, f);

            // Finally add it to our menu
            pMenu->addAction(pAction);

            m_mActionList.insert({sText, pAction});
        }

    protected:
        QMainWindow *m_pWindow;
        std::map<QString, QAction*> m_mActionList;

    protected slots:
        void NotImplemented() {
            QMessageBox::information(m_pWindow, "Go Away!", "Feature not implemented.");
        }
    };


}
#endif //MULTIED_BASEMENU_H
