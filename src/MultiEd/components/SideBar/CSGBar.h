//
// Created by melissaa on 2022-04-24.
//

#ifndef MULTIED_CSGBAR_H
#define MULTIED_CSGBAR_H


#include "BaseBar.h"
namespace Components {
        class CSGBar : public BaseBar {

        public:

            explicit CSGBar(QWidget *pParent) {
                m_pLayout = nullptr;
                m_pParent = pParent;
            };

            void Init() override;

            QGridLayout *GetLayout() override { return m_pLayout; }
        };
}

#endif //MULTIED_CSGBAR_H
