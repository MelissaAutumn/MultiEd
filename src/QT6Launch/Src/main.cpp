//
// Created by melissaa on 26/04/24.
//

#include <Core.h>

#define LAUNCH_NAME TEXT("QT6Launch")
#define LAUNCH_NAME_LOG TEXT("QT6Launch.log")
#define LAUNCH_NAME_INI TEXT("QT6Launch.ini")

TCHAR GPackageInternal[64] = LAUNCH_NAME;
extern "C" { const TCHAR *GPackage = GPackageInternal; }

#include <ue/Launch.h>
#include <csignal>

int main(int argc, char* argv[])
{
    if (const auto engine = UnrealLaunch::Boot(argc, argv, "QT6Launch"))
    {
        // Start main engine loop.
        debugf( TEXT("Entering main loop.") );
        while ( !GIsRequestingExit ) {
            if(!UnrealLaunch::Loop( engine )) {
                GIsRequestingExit = TRUE;
                break;
            }
        }
    }

    return 0;
}
