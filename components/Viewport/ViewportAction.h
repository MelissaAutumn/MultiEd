//
// Created by melissaa on 2022-05-05.
//

#ifndef MULTIED_VIEWPORTACTION_H
#define MULTIED_VIEWPORTACTION_H

#include <QAction>
#include "../../helpers/Defines.h"

namespace Components {
    template<typename Setting>
    class ViewportAction : public QAction {
    public:
        ViewportAction(
            const QString &name,
            const QString &shortcut,
            Setting setting = 0,
            bool isBitFlagBased = false,
            const std::function<void(Setting)> &onClickFn = [](Setting) { return 0; }
        ) : QAction(nullptr) {
            setText(name);
            m_Setting = setting;

            m_IsBitFlagBased = isBitFlagBased;

            setChecked(false);
            setCheckable(true);

            QAction::connect(this, &QAction::triggered, [=](bool checked) {
                onClickFn(m_Setting);
            });
        }

        void UpdateState(Setting setting) {
            if (m_IsBitFlagBased) {
                setChecked(setting & m_Setting);
            } else {
                setChecked(m_Setting == setting);
            }
        }

        Setting                m_Setting;
        bool                   m_IsBitFlagBased;
    private:

    };
}
#endif //MULTIED_VIEWPORTACTION_H
