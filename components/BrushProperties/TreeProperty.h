//
// Created by melissaa on 2022-05-07.
//

#ifndef MULTIED_TREEPROPERTY_H
#define MULTIED_TREEPROPERTY_H

#include <QTreeWidgetItem>
#include <QStyledItemDelegate>
#include <QComboBox>
#include <QLineEdit>

#define QT_PROP_TYPE Qt::UserRole+1
#define QT_PROP_CHOICES Qt::UserRole+2
#define QT_PROP_VALUE Qt::UserRole+3

namespace Components {
    class TreeProperty : public QTreeWidgetItem {
    public:
        TreeProperty() {
            m_pProp = nullptr;
        };
        TreeProperty(UBrushBuilder* pBrush, UProperty* pProp, const QStringList &strings, int type = Type) : m_pBrush(pBrush), m_pProp(pProp), m_defaultsSet(false), QTreeWidgetItem(strings, type) {};

        void Test() {
            qDebug() << "Hello world";
        }

        void ResetValue() {
            FString val = TEXT("");
            BYTE* pData = &m_pBrush->GetClass()->Defaults(0);
            m_pProp->ExportText(0, val, pData, pData, PPF_Localized);
            QString qVal = Helpers::GetQStringFromFString(val);

            // Set the text and value
            setText(1, qVal);
            setData(1, QT_PROP_VALUE, qVal);
        }

        void SetPropertyData() {
            if (m_pProp == nullptr) {
                return;
            }

            QString type = "text";
            QStringList choices;

            if (m_pProp->IsA(UBoolProperty::StaticClass())) {
                type = "bool";
                choices.append("True");
                choices.append("False");
            }
            if (m_pProp->IsA(UByteProperty::StaticClass())) {
                type = "byte";
                auto* pByte = dynamic_cast<UByteProperty*>(m_pProp);
                if (pByte) {
                    // If we have options use those
                    if (pByte->Enum->Names.Num() > 0) {
                        for (int i = 0; i < pByte->Enum->Names.Num(); i++) {
                            choices.append( QString::fromWCharArray(*pByte->Enum->Names(i)) );
                        }
                    }
                    // Otherwise it's a slider
                    else {
                        type = "byte-slider";
                    }
                }
            }
            if (m_pProp->IsA(UNameProperty::StaticClass())) {
                type = "name";
                auto* pName = dynamic_cast<UNameProperty*>(m_pProp);
                if (pName) {
                    // ????
                }
            }
            if (m_pProp->IsA(UFloatProperty::StaticClass())) {
                type = "float";
            }
            if (m_pProp->IsA(UIntProperty::StaticClass())) {
                type = "int";
            }

            m_type = type;
            m_choices = choices;
            m_value = 0;

            // Maybe doing this incorrectly, but we need to set a bunch of ugly user data here
            setData(1, QT_PROP_TYPE, type);
            setData(1, QT_PROP_CHOICES, choices);
            setData(1, QT_PROP_VALUE, 0);

            m_defaultsSet = true;
        }

        /**
         * Sets user data
         * @param column
         * @param role
         * @param value
         */
        void setData(int column, int role, const QVariant &value) override {
            if (role != QT_PROP_VALUE) {
                QTreeWidgetItem::setData(column, role, value);
                return;
            }
            if (!m_defaultsSet) {
                return;
            }

            QTreeWidgetItem::setData(column, role, value);
            qDebug() << "Value set" << m_type << value << value.typeName();

            // The usual .stdwstring doesn't seem to work great here.
            // And I'm a little scared, but here's a quick fix. (I'm sure this is fine...right?)

            auto valueStr = value.toString();
            TCHAR val[valueStr.length() + 1];
            memset(val, 0, sizeof(val));
            valueStr.toWCharArray(val);

            // Few requirements:
            // 1. Booleans need to be sent as 0, 1
            // 2. Numbers need to be sent as ints, despite UnrealEd showing them as floats. (SO WEIRD.) Maybe type specific??
            // 3. I don't know if any other type needs a special value, lol.
            m_pProp->ImportText(val, (BYTE*)m_pBrush + m_pProp->Offset, PPF_Localized);
        }

        UProperty* GetProperty() { return m_pProp; }
        UBrushBuilder* GetBrushBuilder() { return m_pBrush; }



    private:
        UProperty* m_pProp;
        UBrushBuilder* m_pBrush;

        bool m_defaultsSet;

        QString m_type;
        QStringList m_choices;
        QVariant m_value;
    };
}

class UPropertyDelegate: public QStyledItemDelegate {
public:
    explicit UPropertyDelegate(QObject* parent=nullptr): QStyledItemDelegate(parent) {}

    /**
     * Sets the model (TreeProperty) data
     * @param editor
     * @param model
     * @param index
     */
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override {
        auto type = index.model()->data(index, QT_PROP_TYPE);
        auto choices = index.model()->data(index, QT_PROP_CHOICES).toStringList();

        QStyledItemDelegate::setModelData(editor, model, index);

        if (!choices.empty() && (type == "bool" || type == "byte")) {
            auto pBox = qobject_cast<QComboBox*>(editor);
            qDebug() << "Setting data!" << pBox->currentText();

            if (type == "bool") {
                QString val = "1";

                if (pBox->currentIndex() == 1) {
                    val = "0";
                }

                model->setData(index, val, QT_PROP_VALUE);
            } else {
                model->setData(index, pBox->currentIndex(), QT_PROP_VALUE);
            }

            return;
        }
        else if (type == "byte-slider") {
            qDebug() << "TODO!";
            return;
        }

        // Else

        auto pText = qobject_cast<QLineEdit*>(editor);

        auto val = pText->text();
        auto ok = true;

        // Real dumb, but ints need to ints, and floats need to be floats
        if (type == "int") {
            val = QString::number(val.toInt(&ok));
        } else if (type == "float") {
            val = QString::number(val.toFloat(&ok));
        }

        if (!ok) {
            val = pText->text();
        }
        qDebug() << "Setting data!" << val << ok;

        model->setData(index, val, QT_PROP_VALUE);

    }

    QWidget* createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override {

        auto type = index.model()->data(index, QT_PROP_TYPE).toString();
        auto choices = index.model()->data(index, QT_PROP_CHOICES).toStringList();

        if (!choices.empty() && (type == "bool" || type == "byte")) {
            auto pBox = new QComboBox(parent);
            pBox->addItems(choices);
            return pBox;
        }
        else if (type == "byte-slider") {
            qDebug() << "TODO!";
        }

        return QStyledItemDelegate::createEditor(parent, option, index);
    }
};
// Thanks Stackoverflow
class NoEditDelegate: public QStyledItemDelegate {
public:
    NoEditDelegate(QObject* parent=nullptr): QStyledItemDelegate(parent) {}
    virtual QWidget* createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const {
        return nullptr;
    }
};

#endif //MULTIED_TREEPROPERTY_H
