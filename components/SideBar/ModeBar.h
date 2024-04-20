//
// Created by melissaa on 2022-04-24.
//

#ifndef MULTIED_MODEBAR_H
#define MULTIED_MODEBAR_H


#include <QPushButton>
#include <QGridLayout>
#include "../IComponent.h"
#include "BaseBar.h"
#include "SidebarButton.h"
#include "../../helpers/Defines.h"

namespace Components {
        class ModeBar : BaseBar {
        private:
            std::vector<SidebarButton<Helpers::EditorModes>*> m_vButtons;

        public:

            explicit ModeBar(QWidget *pParent) {
                m_pLayout = nullptr;
                m_pParent = pParent;
            };

            void ChangeEditorMode(Helpers::EditorModes mode);

            void Init() override;

            QGridLayout *GetLayout() override { return m_pLayout; }
        };
}

#endif //MULTIED_MODEBAR_H
