//
// Created by melissaa on 2022-05-05.
//

#ifndef MULTIED_BRUSHPROPERTIES_H
#define MULTIED_BRUSHPROPERTIES_H

#include <QApplication>

#include <QWindow>
#include <QDialog>
#include <QTreeWidget>
#include <QHBoxLayout>
#include <QPushButton>


class UBrushBuilder;

namespace Components {
    class BrushProperties : public QDialog {
    public:
        explicit BrushProperties(UBrushBuilder* pBrush);

        void InitTree();

    public:
        void OnBuild();
        void OnReset();
        void OnClose();

    private:
        UBrushBuilder* m_pBrush;
        QTreeWidget* m_pTreeWidget;
        QList<QTreeWidgetItem *> m_aItems;
    };
}
#endif //MULTIED_BRUSHPROPERTIES_H
