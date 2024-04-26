//
// Created by melissaa on 26/04/24.
//

#include <QApplication>
#include <QIcon>
#include <QMainWindow>

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
    QApplication a(argc, argv);
    auto windowIcon = QIcon("../Help/Unreal.ico");
    a.setWindowIcon(windowIcon);

    auto unrealLaunch = new UnrealLaunch();

    if (const auto engine = unrealLaunch->Boot(argc, argv, "QT6Launch"))
    {
        // Start main engine loop.
        debugf( TEXT("Entering main loop.") );
        while ( !GIsRequestingExit ) {
            // Process QT Events
            qApp->processEvents();

            // Process UE Events
            if(!unrealLaunch->Loop( engine )) {
                GIsRequestingExit = TRUE;
                break;
            }
        }
    }

    delete unrealLaunch;

    return 0;
}
