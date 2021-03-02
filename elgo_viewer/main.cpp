#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include <QrCode/QrMaker.h>
#include <QQuickView>
#include <QGuiApplication>

// EFC
#include "LocalSocketEvent/EFCEvent.h"

// Viewer
#include "Event/ViewerEventHandler.h"
#include "ViewerCtrl/ViewerCtrl.h"
#include "ViewerCtrl/ViewerController.h"

static ViewerController *g_ViewerController = ViewerController::GetInstance();
static ViewerEventHandler *g_EventHandler = NULL;

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    g_EventHandler = new ViewerEventHandler(ELGO_PROC::Proc::ELGO_VIEWER);

    // Send proccess ready status to MAIN
    g_ViewerController->GetInstance()->GetViewerCtrl().SendViewerIsReady();

    // Display Viewer
    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
            return -1;

    return app.exec();
}
