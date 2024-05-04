//
// Created by melissaa on 23/05/22.
//

#ifndef MULTIED_PREFERENCES_H
#define MULTIED_PREFERENCES_H

#include <Core.h>
#include <QDialog>
#include <QTreeWidget>
#include <QVBoxLayout>

namespace Components {
    class PreferencesTree;

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

    private:
        QVBoxLayout* m_Layout;
        PreferencesTree* m_TreeWidget;

    };

} // Components

#endif //MULTIED_PREFERENCES_H
