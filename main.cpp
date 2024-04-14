#include <QApplication>
#include <QMainWindow>
#include "ue/UnrealGlue.h"
// Components
#include "components/MultiEdWindow.h"
//#include <SDL2/SDL.h>

UnrealGlue* g_pUnreal = nullptr;


// Define to build the qt parts
// Mainly for debugging the unreal glue
#define COMPILE_WITH_QT

#include <csignal>

void sigHandler(int s)
{
    std::signal(s, SIG_DFL);
    qApp->quit();
}

int main(int argc, char *argv[]) {
#ifdef COMPILE_WITH_QT
    QApplication a(argc, argv);
#endif

    std::signal(SIGINT,  sigHandler);
    std::signal(SIGTERM, sigHandler);

    g_pUnreal = new UnrealGlue(nullptr);
    g_pUnreal->Boot(argc, argv);

#ifdef COMPILE_WITH_QT
    auto* pMainWindow = new Components::MultiEdWindow();
    pMainWindow->Init();
#endif

    // TODO: Move this in a place where we can re-init this from the UI
#ifdef COMPILE_WITH_QT
    pMainWindow->m_pSDLWindow = g_pUnreal->m_pWindow;

    int i = 0;
    for (auto pWinID : g_pUnreal->m_pWndIDs) {
        auto pWindow = QWindow::fromWinId((WId)pWinID);
        auto flags = pWindow->flags();
        QWidget *widget = QWidget::createWindowContainer(pWindow);
        widget->setFocusPolicy(Qt::NoFocus);
        pMainWindow->SetViewport(widget, (WId)pWinID, g_pUnreal->m_nViewportModes.at(i));

        i++;
    }

#endif

#ifdef COMPILE_WITH_QT
    pMainWindow->show();
#endif
    while (true) {
#ifdef COMPILE_WITH_QT
        // Run QT
        qApp->processEvents();
        if (!pMainWindow->isVisible()) {
            break;
        }
#endif
        // Unreal loop
        auto bOkay = g_pUnreal->Loop();
        if (!bOkay) {
            break;
        }
    }

#ifdef COMPILE_WITH_QT
    qApp->exit();
#endif

    return 0;//QApplication::exec();
}
