//
// Created by melissaa on 26/04/24.
//

#ifndef LAUNCH_H
#define LAUNCH_H

class LogWindow;
class QApplication;
class UEngine;

#ifndef EDITOR_API
#define EDITOR_API DLL_IMPORT
#endif

// Keeping things a little organized
class UnrealLaunch {
public:
    UnrealLaunch(bool launchEditor);
    virtual ~UnrealLaunch();

    UEngine *Boot(int argc, char *argv[], const char* moduleName);
    bool Loop(UEngine* engine);

    LogWindow* m_LogWindow;

private:
    UEngine* InitEngine();

    bool m_LaunchEditor;
};

#endif //LAUNCH_H
