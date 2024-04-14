//
// Created by melissaa on 2022-05-07.
//

#include "BrushProperties.h"
// Contains UE classes
#include "../../services/Services.h"
#include <Engine.h>
#include <EditorPrivate.h>

#include "../../helpers/UnrealHelper.h"

#include "TreeProperty.h"

Components::BrushProperties::BrushProperties(UBrushBuilder *pBrush)  {

    m_pBrush = pBrush;
    setWindowTitle(Helpers::GetQStringFromFString(pBrush->GetName()));

    m_pTreeWidget = new QTreeWidget();
    m_pTreeWidget->setColumnCount(2);
    m_pTreeWidget->setItemDelegateForColumn(0, new NoEditDelegate(this));
    m_pTreeWidget->setItemDelegateForColumn(1, new UPropertyDelegate(this));

    InitTree();

    auto pBuild = new QPushButton("&Build");
    auto pReset = new QPushButton("&Reset");
    auto pClose = new QPushButton("&Close");

    QDialog::connect(pBuild, &QPushButton::clicked, [=]() { OnBuild(); });
    QDialog::connect(pReset, &QPushButton::clicked, [=]() { OnReset(); });
    QDialog::connect(pClose, &QPushButton::clicked, [=]() { OnClose(); });

    auto pLayout = new QHBoxLayout();
    auto pInnerLayout = new QVBoxLayout();
    pInnerLayout->setAlignment(Qt::AlignmentFlag::AlignTop);
    pLayout->addWidget(m_pTreeWidget);
    pInnerLayout->addWidget(pBuild);
    pInnerLayout->addWidget(pReset);
    pInnerLayout->addWidget(pClose);
    pLayout->addLayout(pInnerLayout);

    setLayout(pLayout);

    this->show();
}

void Components::BrushProperties::OnBuild() {
    g_pEditorAPI->BuildBrush(m_pBrush);
}

void Components::BrushProperties::OnReset() {
    for(auto pItem : m_aItems) {
        auto pTreeProp = dynamic_cast<Components::TreeProperty*>(pItem);
        if (!pTreeProp) {
            continue;
        }
        pTreeProp->ResetValue();
    }
}

void Components::BrushProperties::OnClose() {
    this->close();
}

void Components::BrushProperties::InitTree() {
// At the current time, each TreeProperty has no unique pointers, so this is safe!
    m_pTreeWidget->clear();

    auto pClass = m_pBrush->GetClass();

    for (TFieldIterator<UProperty> It(m_pBrush->GetClass()); It; ++It) {
        if (It->Category == FName(pClass->GetName())) {
            QString name = QString::fromWCharArray(It->GetName());
            QStringList list(name);

            FString val = TEXT("");
            BYTE* pMainOffset = &pClass->Defaults(0);
            It->ExportText(0, val, pMainOffset, pMainOffset, PPF_Localized);
            QString qVal = Helpers::GetQStringFromFString(val);
            list.push_back(qVal);

            auto ptr = *It;
            auto pWidgetItem = new TreeProperty(m_pBrush, ptr, list);
            pWidgetItem->SetPropertyData();
            pWidgetItem->setFlags(pWidgetItem->flags() | Qt::ItemIsEditable);

            m_aItems.append(pWidgetItem);
        }
    }


    QStringList headerList("Property");
    headerList.push_back("Value");
    m_pTreeWidget->setHeaderLabels(headerList);
    m_pTreeWidget->insertTopLevelItems(0, m_aItems);
}
