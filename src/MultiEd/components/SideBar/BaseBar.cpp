//
// Created by melissaa on 20/04/24.
//
#include "BaseBar.h"
using namespace Components;

QPushButton *BaseBar::CreateButton(const QString &sIcon, const QString &sTooltip, const std::function<void(void)> &onClickFn) {
    auto *btn = new QPushButton("");

    QString iconName = "./editorres/";
    iconName += sIcon;

    QIcon icon(iconName);

    QSize btnSize(40, 40);

    btn->setFlat(true);
    btn->setIcon(icon);
    btn->setIconSize(btnSize);

    btn->setToolTip(sTooltip);
    btn->setFixedSize(btnSize);

    m_vButtons.push_back(btn);

    QObject::connect(btn, &QPushButton::clicked, onClickFn);

    return btn;
}
