//
// Created by melissaa on 2022-04-27.
//

#ifndef MULTIED_TOOLBAR_H
#define MULTIED_TOOLBAR_H

#include <QtWidgets>
#include <QPushButton>
#include <QGridLayout>
#include "../IComponent.h"
#include "../../services/Services.h"

namespace Components {
    class ToolBar : IComponent, public QObject {
    protected:
        template<typename ActionFunction>
        QAction *CreateAction(const char *szIcon, const char *szTooltip, const ActionFunction f) {
            auto *pAction = new QAction();
            QString iconName = "./MultiEd/exe/";
            iconName += szIcon;
            QIcon icon(iconName);

            pAction->setIcon(icon);
            pAction->setToolTip(szTooltip);
            QMainWindow::connect(pAction, &QAction::triggered, f);


            m_mActionList.insert({iconName, pAction});

            return pAction;
        }

        std::map<QString, QAction*> m_mActionList;
    private:
        QMainWindow *m_pWindow;

    private slots:
        void NotImplemented() {
            QMessageBox::information(m_pWindow, "Go Away!", "Feature not implemented.");
        }

    public:
        explicit ToolBar(QMainWindow* pWindow) {
            m_pWindow = pWindow;
        }


        void Init() override;

    };
}

#endif //MULTIED_TOOLBAR_H
