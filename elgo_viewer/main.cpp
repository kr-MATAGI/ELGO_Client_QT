#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include <QrCode/QrMaker.h>
#include <QQuickView>
#include <QGuiApplication>

// EFC
#include "Event/ViewerEventHandler.h"
#include "LocalSocketEvent/EFCEvent.h"

static ViewerEventHandler *g_EventHandler = NULL;

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    g_EventHandler = new ViewerEventHandler(ELGO_PROC::Proc::ELGO_VIEWER);

    // Send proccess ready status to MAIN
    QByteArray readyBytes;
    QDataStream readyStream(&readyBytes, QIODevice::WriteOnly);
    readyStream.setVersion(QDataStream::Qt_5_12);
    readyStream << ELGO_PROC::Proc::ELGO_VIEWER;

    const bool bReadyEvent = EFCEvent::SendEvent(ELGO_PROC::Proc::ELGO_MAIN,
                                                 MAIN_EVENT::Event::PROCESS_IS_READY, readyBytes);
    if(false == bReadyEvent)
    {
        qDebug() << "Send Event Error - " << MAIN_EVENT::Event::PROCESS_IS_READY;
    }

    // Display Viewer
    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
            return -1;


        qmlRegisterType<QrMaker>("QRCode", 1, 0, "QRMaker");

        QQuickView view;
        view.setSource(QUrl("qrc:/contentPlayer.qml"));
        view.show();

    return app.exec();
}
