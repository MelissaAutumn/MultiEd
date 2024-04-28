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
        Preferences();
        ~Preferences() override;
        void LogTree(const TCHAR* szCaption, int depth);
        void AddItem(QTreeWidgetItem* parent, const TCHAR* caption, int depth);
        void InitTree();

    private:
        QTreeWidget* m_TreeWidget;
    };

} // Components

#endif //MULTIED_PREFERENCES_H
