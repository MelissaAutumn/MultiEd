//
// Created by melissaa on 2022-04-24.
//

#ifndef MULTIED_BASEBAR_H
#define MULTIED_BASEBAR_H


#include <QtWidgets>
#include <QPushButton>
#include <QGridLayout>
#include "../IComponent.h"
#include "../../services/Services.h"

namespace Components {
        class BaseBar : IComponent {
        protected:
            QPushButton *CreateButton(const QString& sIcon, const QString& sTooltip = "", const std::function<void(void)>& onClickFn = []() {return 0;}) {
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

            QWidget *m_pParent = nullptr;
            QGridLayout *m_pLayout = nullptr;
            std::vector<QPushButton *> m_vButtons;
        private:
        public:
            virtual ~BaseBar() {
                for (auto pBtn: m_vButtons) {
                    pBtn->deleteLater();
                }
                m_vButtons.clear();

                if (m_pLayout) {
                    m_pLayout->deleteLater();
                    m_pLayout = nullptr;
                }
            }

            std::vector<QPushButton *> GetButtons() { return m_vButtons; };

            virtual QGridLayout *GetLayout() = 0;
        };
}

#endif //MULTIED_BASEBAR_H
