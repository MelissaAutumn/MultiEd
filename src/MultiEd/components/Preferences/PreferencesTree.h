//
// Created by melissaa on 04/05/24.
//

#ifndef PREFERENCESTREE_H
#define PREFERENCESTREE_H
#include <QTreeWidget>

class UObject;
class UClass;

namespace Components {
    class PreferencesTree : public QTreeWidget {
    public:
        /**
         * Generates an empty PreferencesTree
         * You probably don't want this!
         */
        explicit PreferencesTree();
        /**
         * Generate a preferences tree from a class and its properties
         * @param Object
         */
        explicit PreferencesTree(UObject* Object);

        /**
         * Generate a preferences tree from multiple classes and its properties
         * @param Objects
         */
        explicit PreferencesTree(std::vector<UObject*> Objects);

        ~PreferencesTree() override;

        void Reinitialize(UObject*);
        void Setup();
        void Generate();
        void Generate(UClass* ClassOverride, QTreeWidgetItem* Parent);


        /**
         * Generate a preferences tree from a given package and its properties
         * @param Key
         * @param Package
         * @return
         */
        static PreferencesTree* FromConfigProperty(const QString &Key, const QString &Package);

    private:
        QString Key;
        QString Package;
        std::vector<UObject*> m_Objects;


    };
}

#endif //PREFERENCESTREE_H
