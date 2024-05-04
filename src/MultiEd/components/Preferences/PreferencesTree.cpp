//
// Created by melissaa on 04/05/24.
//

#include <utility>

#include "PreferencesTree.h"

#include <queue>

#include "../TreeDelegates.h"
#include <Core.h>

Components::PreferencesTree::PreferencesTree() {
    Setup();
}

Components::PreferencesTree::PreferencesTree(UObject *Object) {
    m_Objects.push_back(Object);
    Setup();
    Generate();
}

Components::PreferencesTree::PreferencesTree(std::vector<UObject *> Objects) {
    m_Objects = std::move(Objects);
    Setup();
    Generate();
}

Components::PreferencesTree::~PreferencesTree() {

}

void Components::PreferencesTree::Reinitialize(UObject * Object) {
    // Remove any existing children
    this->clear();
    m_Objects.clear();
    m_Objects.push_back(Object);
    this->Generate();
}

void Components::PreferencesTree::Setup() {
    this->setColumnCount(2);
    this->setItemDelegateForColumn(0, new NoEditDelegate(this));
    this->setItemDelegateForColumn(1, new UPropertyDelegate(this));
    QStringList headerList("Property");
    headerList.push_back("Value");
    this->setHeaderLabels(headerList);
    this->setMinimumSize(480, 640);
}

void Components::PreferencesTree::Generate(UClass *ClassOverride, QTreeWidgetItem* Parent) {
    // FIXME: Only deal with one right now
    UObject* object = nullptr;
    UClass* uClass = ClassOverride;

    if (!uClass) {
        object = m_Objects.at(0);
        uClass = object->GetClass();
    }

    std::map<QString, std::vector<QTreeWidgetItem *> > treeMap;
    for (TFieldIterator<UProperty> It(uClass); It; ++It) {
        // This seems to be correct?
        if (It->Category == FName(NAME_None)) {
            continue;
        }

        QString name = QString::fromWCharArray(It->GetName());
        QStringList list(name);

        FString val = TEXT("");

        BYTE *mainOffset = nullptr;
        if (!object) {
            mainOffset = &uClass->Defaults(0);
        } else {
            mainOffset = reinterpret_cast<BYTE*>(object);
            //mainOffset -= It->Offset;
        }

        It->ExportText(0, val, mainOffset, mainOffset, PPF_Localized);
        QString qVal = QString::fromWCharArray(*val);
        list.push_back(qVal);

        auto childItem = new QTreeWidgetItem(Parent ? Parent : nullptr);
        childItem->setFlags(Qt::ItemIsEditable | Qt::ItemIsEnabled);

        childItem->setText(0, list[0]);
        childItem->setText(1, list[1]);

        if (object) {
            auto category = QString::fromWCharArray(*It->Category);
            treeMap[category].push_back(childItem);
        }
    }

    for (const auto &[categoryName, children]: treeMap) {
        auto categoryItem = new QTreeWidgetItem(this);
        categoryItem->setText(0, categoryName);

        for (auto child: children) {
            categoryItem->addChild(child);
        }
    }
}

void Components::PreferencesTree::Generate() {
    this->Generate(nullptr, nullptr);
}

Components::PreferencesTree * Components::PreferencesTree::FromConfigProperty(const QString &Key,
                                                                              const QString &Package) {
    TCHAR key[256];
    TCHAR package[256];
    memset(key, 0, sizeof(key));
    memset(package, 0, sizeof(package));

    Key.toWCharArray(key);
    Package.toWCharArray(package);
    auto category = LocalizeGeneral(key, package);

    auto tree = new Components::PreferencesTree();

    struct PrefQueueInfo {
        TArray<struct FPreferencesInfo> prefs;
        QTreeWidgetItem *parent;
    };

    TArray<struct FPreferencesInfo> firstPrefs;
    UObject::GetPreferences(firstPrefs, category, 1);

    std::queue<PrefQueueInfo> queue;
    queue.push(PrefQueueInfo({firstPrefs, nullptr}));

    // breath-first search
    while (!queue.empty()) {
        auto pop = queue.front();

        auto parent = pop.parent;
        TArray<struct FPreferencesInfo> prefs = pop.prefs;

        queue.pop();

        for (auto i = 0; i < prefs.Num(); i++) {
            auto pref = prefs(i);

            auto caption = QString::fromWCharArray(*pref.Caption);

            const auto item = new QTreeWidgetItem(!parent ? tree : nullptr);
            item->setText(0, caption);

            if (parent) {
                parent->addChild(item);
            }

            // Loop through properties if they have any!
            if (const auto uClass = UObject::StaticLoadClass(UObject::StaticClass(), nullptr, *pref.Class, nullptr,
                                                             LOAD_NoWarn, nullptr)) {
                uClass->GetDefaultObject()->LoadConfig(1); //!!
                tree->Generate(uClass, item);
                                                             }

            TArray<struct FPreferencesInfo> nextPrefs;
            UObject::GetPreferences(nextPrefs, *pref.Caption, 0);
            queue.push({nextPrefs, item});
        }
    }

    return tree;
}
