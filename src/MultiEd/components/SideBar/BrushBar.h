//
// Created by melissaa on 2022-04-24.
//

#ifndef MULTIED_BRUSHBAR_H
#define MULTIED_BRUSHBAR_H


#include "BaseBar.h"
#include "../BrushProperties/BrushProperties.h"

/**
 * Brush Bar
 * It's (hopefully) only class that touches UE internals aside from Services.
 */
namespace Components {
        class BrushBar : BaseBar {

        public:

            explicit BrushBar(QWidget *pParent) {
                m_pLayout = nullptr;
                m_pParent = pParent;
            };

            void OpenProperties(UBrushBuilder* pBrush);

            void Init() override;

            QGridLayout *GetLayout() override { return m_pLayout; }
        private:
            std::vector<UBrushBuilder*> m_Brushes;

        private slots:
            void OpenProps(int index);
        };



}

#endif //MULTIED_BRUSHBAR_H
