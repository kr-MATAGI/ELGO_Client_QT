#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include <QRCode/QrMaker.h>
#include <QQuickView>
#include <QGuiApplication>

// EFC
#include "Event/ViewerEventHandler.h"

static ViewerEventHandler *g_EventHandler = NULL;

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    g_EventHandler = new ViewerEventHandler(ELGO_PROC::Proc::ELGO_VIEWER);

    return app.exec();
}
