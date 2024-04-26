//
// Created by melissaa on 26/04/24.
//

#ifndef LAUNCH_H
#define LAUNCH_H

class UEngine;

#ifndef EDITOR_API
#define EDITOR_API DLL_IMPORT
#endif

// Keeping things a little organized
class UnrealLaunch {
public:

    static UEngine *Boot(int argc, char *argv[], const char* moduleName);
    static bool Loop(UEngine* engine);

private:
    static UEngine* InitEngine();
};

#endif //LAUNCH_H
