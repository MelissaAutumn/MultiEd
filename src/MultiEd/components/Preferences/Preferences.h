//
// Created by melissaa on 23/05/22.
//

#ifndef MULTIED_PREFERENCES_H
#define MULTIED_PREFERENCES_H

#include <Core.h>
#include <QDialog>
#include <QTreeWidget>

namespace Components {

    class Preferences : QDialog {


    public:
        /**
         * Initialize preferences for a single class and its properties
         * @param UClass
         */
        explicit Preferences(UObject* UObj);

        /**
         * Initialize preferences for config values of a given package and its properties.
         * @param Key
         * @param Package
         */
        Preferences(const QString &Key, const QString &Package);
        ~Preferences() override;
        void LogTree(const TCHAR* szCaption, int depth);
        void AddItems();

        template<typename TreeWidgetLike>
        void AddClassProperties(UClass* UClass, TreeWidgetLike* Parent, bool BuildCategories);
        void InitTree();

    private:
        QTreeWidget* m_TreeWidget;
        QString Key;
        QString Package;
        UObject* m_UObj;
    };

} // Components

#endif //MULTIED_PREFERENCES_H
