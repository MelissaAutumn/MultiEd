//
// Created by melissaa on 27/04/24.
//

#ifndef TREEDELEGATES_H
#define TREEDELEGATES_H
#include <QComboBox>
#include <QStyledItemDelegate>
#include <QLineEdit>

#define QT_PROP_TYPE Qt::UserRole+1
#define QT_PROP_CHOICES Qt::UserRole+2
#define QT_PROP_VALUE Qt::UserRole+3


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

#endif //TREEDELEGATES_H
