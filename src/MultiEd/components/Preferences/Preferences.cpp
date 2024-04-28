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

#include "../TreeDelegates.h"

#include "../../helpers/UnrealHelper.h"



namespace Components {
    Preferences::Preferences(const QString &Key, const QString &Package) {
        this->Key = Key;
        this->Package = Package;

        m_TreeWidget = nullptr;

        this->setWindowTitle("Properties Window");

        InitTree();

        this->setBaseSize(320, 640);
        this->show();
    }

    Preferences::~Preferences() {
        delete m_TreeWidget;
    }

    void Preferences::LogTree(const TCHAR* szCaption, int depth) {
        TArray<struct FPreferencesInfo> aPrefs;
        UObject::GetPreferences( aPrefs, szCaption, 0 );

        for(int i = 0; i < aPrefs.Num(); i++) {
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

        qInfo() << QString("Properties for LocalizeGeneral(%1, %2) = %3").arg(QString::fromWCharArray(key), QString::fromWCharArray(package), QString::fromWCharArray(category));
        this->setWindowTitle(QString::fromWCharArray(category));

        struct PrefQueueInfo {
            TArray<struct FPreferencesInfo> prefs;
            QTreeWidgetItem* parent;
        };

        TArray<struct FPreferencesInfo> firstPrefs;
        UObject::GetPreferences( firstPrefs, category, 1 );

        std::queue<PrefQueueInfo> queue;
        queue.push(PrefQueueInfo({firstPrefs, nullptr}));

        // breath-first search
        while(!queue.empty()) {
            auto pop = queue.front();

            auto parent = pop.parent;
            TArray<struct FPreferencesInfo> prefs = pop.prefs;

            queue.pop();

            for(auto i = 0; i < prefs.Num(); i++) {
                auto pref = prefs(i);

                auto caption = QString::fromWCharArray(*pref.Caption);

                const auto item = new QTreeWidgetItem(!parent ? m_TreeWidget : nullptr);
                item->setText(0, caption);

                if (parent) {
                    parent->addChild(item);
                }

                // Loop through properties if they have any!
                if (const auto uClass = UObject::StaticLoadClass( UObject::StaticClass(), nullptr, *pref.Class, nullptr, LOAD_NoWarn, nullptr )) {
                    uClass->GetDefaultObject()->LoadConfig(1);//!!

                    for (TFieldIterator<UProperty> It(uClass); It; ++It) {
                        //if (It->Category == FName(uClass->GetName()))
                        {
                            QString name = QString::fromWCharArray(It->GetName());
                            QStringList list(name);

                            FString val = TEXT("");
                            BYTE* mainOffset = &uClass->Defaults(0);
                            It->ExportText(0, val, mainOffset, mainOffset, PPF_Localized);
                            QString qVal = QString::fromWCharArray(*val);
                            list.push_back(qVal);

                            auto childItem = new QTreeWidgetItem(item);
                            childItem->setFlags(Qt::ItemIsEditable|Qt::ItemIsEnabled);

                            childItem->setText(0, list[0]);
                            childItem->setText(1, list[1]);
                        }
                    }
                }

                TArray<struct FPreferencesInfo> nextPrefs;
                UObject::GetPreferences( nextPrefs, *pref.Caption, 0 );
                queue.push({nextPrefs, item});
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

        this->AddItems();
    }
} // Components