// QT
#include <QApplication>

// Viewer
#include "MainWindow.h"
#include "Event/ViewerEventHandler.h"
#include "ViewerCtrl/ViewerCtrl.h"
#include "ViewerCtrl/ViewerController.h"
#include "Widget/ContentsPlayer.h"

static ViewerController *g_ViewerController = NULL;
static ContentsPlayer *g_ContentsPlayer = NULL;
static MainWindow *g_MainWindow = NULL;
static ViewerEventHandler *g_EventHandler = NULL;

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    g_ViewerController = ViewerController::GetInstance();
    g_MainWindow = MainWindow::GetInstance();

    // Event Listenner
    g_EventHandler = new ViewerEventHandler(ELGO_SYS::Proc::ELGO_VIEWER);

    // Send proccess ready status to MAIN
    g_ViewerController->GetInstance()->GetViewerCtrl().SendViewerIsReady();

    // Contents Player Init
    g_ContentsPlayer = ContentsPlayer::GetInstance();
    g_ContentsPlayer->showFullScreen();

    g_MainWindow->showFullScreen();
    emit g_MainWindow->GetInstance()->CloseMainWindowByError();

    return app.exec();
}
