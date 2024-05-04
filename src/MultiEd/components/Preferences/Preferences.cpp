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

#include "../TreeDelegates.h"

#include "../../helpers/UnrealHelper.h"


namespace Components {
    Preferences::Preferences(UObject *UObj) {
        m_TreeWidget = nullptr;

        this->setAttribute(Qt::WA_DeleteOnClose);

        InitTree();

        m_UObj = UObj;
        this->AddClassProperties(UObj->GetClass(), m_TreeWidget, true);
        this->setWindowTitle(QString("%1 Properties").arg(QString::fromWCharArray(UObj->GetClass()->GetName())));

        this->setBaseSize(320, 640);
        this->show();
    }

    Preferences::Preferences(const QString &Key, const QString &Package) {
        this->Key = Key;
        this->Package = Package;
        this->setAttribute(Qt::WA_DeleteOnClose);
        m_TreeWidget = nullptr;

        m_UObj = nullptr;
        this->setWindowTitle("Properties Window");

        InitTree();
        this->AddItems();

        this->setBaseSize(320, 640);
        this->show();
    }


    Preferences::~Preferences() {
        delete m_TreeWidget;
    }

    void Preferences::LogTree(const TCHAR *szCaption, int depth) {
        TArray<struct FPreferencesInfo> aPrefs;
        UObject::GetPreferences(aPrefs, szCaption, 0);

        for (int i = 0; i < aPrefs.Num(); i++) {
            auto caption = Helpers::GetQStringFromFString(aPrefs(i).Caption);
            qDebug() << depth << QString::fromWCharArray(*aPrefs(i).ParentCaption) << "->" << caption;
            LogTree(*aPrefs(i).Caption, depth + 1);
        }
    }

    void Preferences::AddItems() {
        TCHAR key[256];
        TCHAR package[256];
        memset(key, 0, sizeof(key));
        memset(package, 0, sizeof(package));

        this->Key.toWCharArray(key);
        this->Package.toWCharArray(package);

        auto category = LocalizeGeneral(key, package);
        this->setWindowTitle(QString::fromWCharArray(category));

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

                const auto item = new QTreeWidgetItem(!parent ? m_TreeWidget : nullptr);
                item->setText(0, caption);

                if (parent) {
                    parent->addChild(item);
                }

                // Loop through properties if they have any!
                if (const auto uClass = UObject::StaticLoadClass(UObject::StaticClass(), nullptr, *pref.Class, nullptr,
                                                                 LOAD_NoWarn, nullptr)) {
                    uClass->GetDefaultObject()->LoadConfig(1); //!!
                    this->AddClassProperties(uClass, item, false);
                }

                TArray<struct FPreferencesInfo> nextPrefs;
                UObject::GetPreferences(nextPrefs, *pref.Caption, 0);
                queue.push({nextPrefs, item});
            }
        }
    }

    template<typename TreeWidgetLike>
    void Preferences::AddClassProperties(UClass *UClass, TreeWidgetLike *Parent, bool BuildCategories) {
        if (!UClass) {
            return;
        }

        std::map<QString, std::vector<QTreeWidgetItem *> > treeMap;
        for (TFieldIterator<UProperty> It(UClass); It; ++It) {
            // This seems to be correct?
            if (It->Category == FName(NAME_None)) {
                continue;
            }

            QString name = QString::fromWCharArray(It->GetName());
            QStringList list(name);

            FString val = TEXT("");

            BYTE *mainOffset = nullptr;
            if (!m_UObj) {
                mainOffset = &UClass->Defaults(0);
            } else {
                mainOffset = reinterpret_cast<BYTE*>(m_UObj);
                //mainOffset -= It->Offset;
            }

            It->ExportText(0, val, mainOffset, mainOffset, PPF_Localized);
            QString qVal = QString::fromWCharArray(*val);
            list.push_back(qVal);

            auto childItem = new QTreeWidgetItem(!BuildCategories ? Parent : nullptr);
            childItem->setFlags(Qt::ItemIsEditable | Qt::ItemIsEnabled);

            childItem->setText(0, list[0]);
            childItem->setText(1, list[1]);

            if (BuildCategories) {
                auto category = QString::fromWCharArray(*It->Category);
                treeMap[category].push_back(childItem);
            }
        }

        for (const auto &[categoryName, children]: treeMap) {
            auto categoryItem = new QTreeWidgetItem(Parent);
            categoryItem->setText(0, categoryName);

            for (auto child: children) {
                categoryItem->addChild(child);
            }
        }
    }

    void Preferences::InitTree() {
        auto layout = new QVBoxLayout();

        m_TreeWidget = new QTreeWidget(this);
        m_TreeWidget->setColumnCount(2);
        m_TreeWidget->setItemDelegateForColumn(0, new NoEditDelegate(m_TreeWidget));
        m_TreeWidget->setItemDelegateForColumn(1, new UPropertyDelegate(m_TreeWidget));
        QStringList headerList("Property");
        headerList.push_back("Value");
        m_TreeWidget->setHeaderLabels(headerList);

        m_TreeWidget->setMinimumSize(480, 640);

        layout->addWidget(m_TreeWidget);
        this->setLayout(layout);
    }
} // Components
