//
// Created by melissaa on 2022-04-24.
//

#ifndef MULTIED_CLIPBAR_H
#define MULTIED_CLIPBAR_H


#include "BaseBar.h"

namespace Components {
        class ClipBar : BaseBar {
        public:
            explicit ClipBar(QWidget *pParent) {
                m_pLayout = nullptr;
                m_pParent = pParent;
            };

            void Init() override;

            QGridLayout *GetLayout() override { return m_pLayout; }
        };
}

#endif //MULTIED_CLIPBAR_H
