#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include <QRCode/QrMaker.h>
#include <QQuickView>
#include <QGuiApplication>

// EFC
#include "Event/ViewerEventHandler.h"

static EventHandler *g_EventHandler = NULL;

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    g_EventHandler = new EventHandler(ELGO_PROC::Proc::ELGO_VIEWER);

    qmlRegisterType<QrMaker>("QRCode", 1, 0, "QRMaker");

    QQuickView view;
    view.setSource(QUrl("qrc:/contentPlayer.qml"));
    view.show();

    return app.exec();
}
