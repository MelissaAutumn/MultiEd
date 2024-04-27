#include <QApplication>
#include <QMainWindow>
#include "ue/UnrealGlue.h"
// Components
#include "components/MultiEdWindow.h"
#include "ue/Launch.h"
//#include <SDL2/SDL.h>

// Editor functionality
UnrealGlue* g_pUnreal = nullptr;
// Game/Shared functionality
UnrealLaunch* g_UnrealLaunch = nullptr;

// Define to build the qt parts
// Mainly for debugging the unreal glue
#define COMPILE_WITH_QT

#include <csignal>

void sigHandler(int s)
{
    std::signal(s, SIG_DFL);
    qApp->quit();
}

int main(int argc, char* argv[])
{
#ifdef __linux__
    // Force XWayland/X11 mode if nothing is provided, until we have a better solution that works on wayland.
    if (!getenv("QT_QPA_PLATFORM"))
    {
        char x11[] = "QT_QPA_PLATFORM=xcb";
        putenv(x11);
    }
#endif

#ifdef COMPILE_WITH_QT
    QApplication a(argc, argv);
#endif

    std::signal(SIGINT, sigHandler);
    std::signal(SIGTERM, sigHandler);

    g_UnrealLaunch = new UnrealLaunch(true);
    g_pUnreal = new UnrealGlue();

    // Boot SharedLaunch, and some additional stuff for the editor
    auto engine = g_UnrealLaunch->Boot(argc, argv, "MultiEd");
    g_pUnreal->Boot(engine);

#ifdef COMPILE_WITH_QT
    auto windowIcon = QIcon("../Help/UnrealEd.ico");
    a.setWindowIcon(windowIcon);

    auto* pMainWindow = new Components::MultiEdWindow();;
    pMainWindow->Init();

    // centre on screen
    auto primaryScreen = a.primaryScreen();
    auto screenWidth = primaryScreen->geometry().width();
    auto screenHeight = primaryScreen->geometry().height();
    pMainWindow->move((screenWidth / 2) - (pMainWindow->width() / 2), (screenHeight / 2) - (pMainWindow->height() / 2));

#endif

    // TODO: Move this in a place where we can re-init this from the UI
#ifdef COMPILE_WITH_QT
    pMainWindow->m_pSDLWindow = g_pUnreal->m_pWindow;

    int i = 0;
    for (auto pWinID : g_pUnreal->m_pWndIDs)
    {
        auto pWindow = QWindow::fromWinId((WId)pWinID);
        QWidget* widget = QWidget::createWindowContainer(pWindow);
        widget->setFocusPolicy(Qt::NoFocus);
        pMainWindow->SetViewport(widget, (WId)pWinID, g_pUnreal->m_nViewportModes.at(i));
        i++;
    }

#endif

#ifdef COMPILE_WITH_QT
    pMainWindow->show();
#endif

    /*
     * We need to handle two different event loops here!
     */
    while (true)
    {
#ifdef COMPILE_WITH_QT
        // QT loop
        qApp->processEvents();
        if (!pMainWindow->isVisible())
        {
            break;
        }
#endif

        pMainWindow->Update();

        if (!g_pUnreal->HasViewports()) {
            continue;
        }

        // Unreal loop
        auto bOkay = g_UnrealLaunch->Loop(engine);
        if (!bOkay)
        {
            break;
        }
    }

    delete g_pUnreal;

#ifdef COMPILE_WITH_QT
    qApp->exit();
#endif

    delete g_UnrealLaunch;

    return 0;
}
