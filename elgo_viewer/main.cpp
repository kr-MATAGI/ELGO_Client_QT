#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include <QRCode/QrMaker.h>
#include <QQuickView>
#include <QGuiApplication>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    qmlRegisterType<QrMaker>("QRCode", 1, 0, "QRMaker");

    QQuickView view;
    view.setSource(QUrl("qrc:/contentPlayer.qml"));
    view.show();

    return app.exec();
}
