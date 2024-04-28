//
// Created by melissaa on 23/05/22.
//
#include <QTreeWidget>
#include <QDebug>
#include <QString>
//#include <Core.h>
#include "Preferences.h"

#include <QHBoxLayout>

#include "../TreeDelegates.h"

#include "../../helpers/UnrealHelper.h"



namespace Components {
    Preferences::Preferences() {

        m_TreeWidget = nullptr;
        auto caption = LocalizeGeneral("AdvancedOptionsTitle",TEXT("Window"));

        this->setWindowTitle(QString::fromWCharArray(caption));

        LogTree(caption, 0);
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

    void Preferences::AddItem(QTreeWidgetItem *parent, const TCHAR *caption, int depth) {
        TArray<struct FPreferencesInfo> aPrefs;
        UObject::GetPreferences( aPrefs, caption, 0 );
        
        for(int i = 0; i < aPrefs.Num(); i++) {
            auto qCaption = QString::fromWCharArray(*aPrefs(i).Caption);
            const auto item = new QTreeWidgetItem(!parent ? m_TreeWidget : nullptr);
            item->setText(0, qCaption);
            if (parent) {
                parent->addChild(item);
            }

            item->setText(1, QString::fromWCharArray(*aPrefs(i).Class));

            if (const auto uClass = UObject::StaticLoadClass( UObject::StaticClass(), nullptr, *aPrefs(i).Class, nullptr, LOAD_NoWarn, nullptr )) {
                for (TFieldIterator<UProperty> It(uClass); It; ++It) {
                    if (It->Category == FName(uClass->GetName())) {
                        QString name = QString::fromWCharArray(It->GetName());
                        QStringList list(name);

                        FString val = TEXT("");
                        BYTE* pMainOffset = &uClass->Defaults(0);
                        It->ExportText(0, val, pMainOffset, pMainOffset, PPF_Localized);
                        QString qVal = Helpers::GetQStringFromFString(val);
                        list.push_back(qVal);

                        auto childItem = new QTreeWidgetItem(item);
                        childItem->setFlags(Qt::ItemIsEditable|Qt::ItemIsEnabled);

                        childItem->setText(0, list[0]);
                        childItem->setText(1, list[1]);
                    }
                }
            }

            AddItem(item, *aPrefs(i).Caption, depth + 1);
        }
    }

    void Preferences::InitTree() {
        auto category = LocalizeGeneral("AdvancedOptionsTitle",TEXT("Window"));

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

        this->AddItem(nullptr, category, 0);

        m_TreeWidget->expandToDepth(1);
    }
} // Components