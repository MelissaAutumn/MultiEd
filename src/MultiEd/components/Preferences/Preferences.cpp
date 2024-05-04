//
// Created by melissaa on 23/05/22.
//
#include <QTreeWidget>
#include <QDebug>
#include <QString>
//#include <Core.h>
#include "Preferences.h"

#include <QHBoxLayout>
#include <queue>
#include <map>
#include <vector>

#include "PreferencesTree.h"
#include "../TreeDelegates.h"

#include "../../helpers/UnrealHelper.h"


namespace Components {
    Preferences::Preferences(UObject *UObj) {
        this->setAttribute(Qt::WA_DeleteOnClose);

        m_TreeWidget = new PreferencesTree(UObj);
        auto layout = new QVBoxLayout();
        layout->addWidget(m_TreeWidget);
        m_Layout = layout;
        this->setLayout(layout);

        this->setWindowTitle(QString("%1 Properties").arg(QString::fromWCharArray(UObj->GetClass()->GetName())));

        this->setBaseSize(320, 640);
        this->show();
    }

    Preferences::Preferences(const QString &Key, const QString &Package) {
        this->setAttribute(Qt::WA_DeleteOnClose);

        m_TreeWidget = PreferencesTree::FromConfigProperty(Key, Package);
        auto layout = new QVBoxLayout();
        layout->addWidget(m_TreeWidget);
        m_Layout = layout;
        this->setLayout(layout);

        this->setWindowTitle("Properties Window");


        this->setBaseSize(320, 640);
        this->show();
    }


    Preferences::~Preferences() {
        delete m_TreeWidget;
        delete m_Layout;
    }

} // Components
