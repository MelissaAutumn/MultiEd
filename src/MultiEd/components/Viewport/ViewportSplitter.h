//
// Created by melissaa on 2022-05-02.
//

#ifndef MULTIED_VIEWPORTSPLITTER_H
#define MULTIED_VIEWPORTSPLITTER_H

#include <QSplitter>

namespace Components {
    /**
     * Exists for one function proxy!
     */
    class ViewportSplitter : public QSplitter {
    public:
        explicit ViewportSplitter(QWidget* parent = nullptr) : QSplitter(parent) {}
        explicit ViewportSplitter(Qt::Orientation orientation, QWidget* parent = nullptr) : QSplitter(orientation, parent) {}

        void moveSplitterProxy(int pos, int index);;
    };
}

#endif //MULTIED_VIEWPORTSPLITTER_H
