//
// Created by melissaa on 2022-04-24.
//

#ifndef MULTIED_MISCBAR_H
#define MULTIED_MISCBAR_H


#include "BaseBar.h"
namespace Components {
    class MiscBar : BaseBar {

    public:

        explicit MiscBar(QWidget *pParent) {
            m_pLayout = nullptr;
            m_pParent = pParent;
        };

        static void ChangeCameraSpeed(QPushButton* pCameraSpeed);

        void Init() override;

        QGridLayout *GetLayout() override { return m_pLayout; }
        QPushButton* m_pCameraSpeed{};
    };
}

#endif //MULTIED_MISCBAR_H
