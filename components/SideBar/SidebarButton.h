//
// Created by melissaa on 2022-05-05.
//

#ifndef MULTIED_SIDEBARBUTTON_H
#define MULTIED_SIDEBARBUTTON_H

#include <QPushButton>
#include <QAction>
#include <QDebug>
#include <utility>
#include <QMouseEvent>
#include "../../helpers/Defines.h"

namespace Components {
    template<typename Setting>
    class SidebarButton : public QPushButton {
    public:
        typedef std::function<void(Setting)> OnClickFn;
        explicit SidebarButton(QWidget *pParent = nullptr) : QPushButton(pParent) {
            setFlat(true);
            setCheckable(true);
        };

        explicit SidebarButton(
            const QString &toolTip,
            QIcon icon = QIcon(),
            Setting setting = 0,
            bool isBitFlagBased = false,
            OnClickFn onLeftClickFn = [](Setting) { return; },
            OnClickFn onRightClickFn = [](Setting) { return; }
        ) : QPushButton(nullptr) {

            // We're an icon button
            setFlat(true);
            setCheckable(true);
            setChecked(false);

            setToolTip(toolTip);

            m_IsBitFlagBased = isBitFlagBased;
            m_Icon           = std::move(icon);
            m_Setting        = setting;

            setIcon(m_Icon);

            // TODO: Make sure this is right
            m_OnLeftClick = onLeftClickFn;
            m_OnRightClick = onRightClickFn;

            // TODO: Will this work nicely with display scaling?
            auto size = QSize(40, 40);
            this->setFixedSize(size);
            this->setIconSize(size);
        }

        void mousePressEvent(QMouseEvent *e) override {

            if (e->button() == Qt::LeftButton) {
                m_OnLeftClick(m_Setting);
            } else if (e->button() == Qt::RightButton) {
                m_OnRightClick(m_Setting);
            }

        }

        void UpdateState(Setting setting) {
            if (m_IsBitFlagBased) {
                setChecked(setting & m_Setting);
            } else {
                setChecked(m_Setting == setting);
            }
        }

        QIcon   m_Icon;
        Setting m_Setting;
        bool    m_IsBitFlagBased;
        OnClickFn m_OnLeftClick;
        OnClickFn m_OnRightClick;
    private:
    };
}
#endif //MULTIED_SIDEBARBUTTON_H
