//
// Created by melissaa on 23/05/22.
//

#ifndef MULTIED_PREFERENCES_H
#define MULTIED_PREFERENCES_H

#include <Core.h>
#include <QDialog>

namespace Components {

    class Preferences : QDialog {


    public:
        Preferences(const QString &Key, const QString &Package);
        ~Preferences() override;
        void LogTree(const TCHAR* szCaption, int depth);
        void AddItems();
        void InitTree();

    private:
        QTreeWidget* m_TreeWidget;
        QString Key;
        QString Package;
    };

} // Components

#endif //MULTIED_PREFERENCES_H
