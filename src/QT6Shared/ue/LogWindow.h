//
// Created by melissaa on 26/04/24.
//

#ifndef LOG_WINDOW_H
#define LOG_WINDOW_H

#include "../qt/LogWindow/LogWindow.h"

class LogWindow : public FOutputDevice {
public:
    FOutputDevice           *AuxOut;
    Components::qtLogWindow *LogWin;
    UBOOL ShowLog;

    LogWindow()
        : AuxOut(nullptr), LogWin(nullptr)
        , ShowLog(FALSE) {}

    void Serialize(const TCHAR *V, EName Event) {
        guard(LogWindow::Serialize) ;
        if (LogWin) {
            LogWin->Log(V);
        }
        if (AuxOut) {
            AuxOut->Serialize(V, Event);
        }
        unguard;
    }
};
#endif //LOG_WINDOW_H
