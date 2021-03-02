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

static ViewerEventHandler *g_EventHandler = NULL;
static ViewerCtrl *g_ViewerCtrl = NULL;

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    g_EventHandler = new ViewerEventHandler(ELGO_PROC::Proc::ELGO_VIEWER);
    g_ViewerCtrl = new ViewerCtrl();

    // Send proccess ready status to MAIN
    g_ViewerCtrl->SendViewerIsReady();

    // Display Viewer
    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
            return -1;

    return app.exec();
}
