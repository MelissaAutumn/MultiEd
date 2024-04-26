//
// Created by melissaa on 2022-05-05.
//

#ifndef MULTIED_VIEWPORTBUTTON_H
#define MULTIED_VIEWPORTBUTTON_H

#include <QPushButton>
#include <QAction>
#include <QDebug>
#include <utility>
#include "../../helpers/Defines.h"

namespace Components {
    template<typename Setting>
    class ViewportButton : public QPushButton {
    public:
        explicit ViewportButton(QWidget *pParent = nullptr) : QPushButton(pParent) {
            setFlat(true);
            setCheckable(true);
        };

        explicit ViewportButton(
            const QString &toolTip,
            QIcon activeIcon = QIcon(),
            QIcon inactiveIcon = QIcon(),
            Setting setting = 0,
            bool isBitFlagBased = false,
            const std::function<void(Setting)> &onClickFn = [](Setting) { return 0; }
        ) : QPushButton(nullptr) {

            // We're an icon button
            setFlat(true);
            setCheckable(true);
            setChecked(false);

            setToolTip(toolTip);

            m_IsBitFlagBased = isBitFlagBased;

            m_ActiveIcon   = std::move(activeIcon);
            m_InactiveIcon = std::move(inactiveIcon);

            m_Setting = setting;

            QPushButton::connect(this, &QPushButton::clicked, [=](bool checked) {
                onClickFn(m_Setting);
            });

            SetNewIcon();

            // TODO: Will this work nicely with display scaling?
            auto size = QSize(28, 28);
            this->setFixedSize(size);
            this->setIconSize(size);
        }

        void UpdateState(Setting setting) {
            if (m_IsBitFlagBased) {
                setChecked(setting & m_Setting);
            } else {
                setChecked(m_Setting == setting);
            }
            SetNewIcon();
        }

        QIcon                  m_ActiveIcon;
        QIcon                  m_InactiveIcon;
        Setting                m_Setting;
        bool                   m_IsBitFlagBased;

    private:
        void SetNewIcon() {
            if (isChecked()) {
                setIcon(m_ActiveIcon);
            } else if (!isChecked()) {
                setIcon(m_InactiveIcon);
            }
        }

    };
}

#endif //MULTIED_VIEWPORTBUTTON_H
