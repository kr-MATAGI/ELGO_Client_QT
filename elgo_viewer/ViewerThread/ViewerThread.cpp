// QT
#include <QQuickView>

#include <QThread>

// EFC
#include "ShardMem/ShmCtrl.h"

// Viewer
#include "Logger/ViewerLogger.h"
#include "QrCode/QrMaker.h"
#include "ViewerThread.h"

//========================================================
ViewerThread::ViewerThread()
//========================================================
{

}

//========================================================
ViewerThread::~ViewerThread()
//========================================================
{

}

//========================================================
void ViewerThread::SetViewerEvent(VIEWER_EVENT::Event event)
//========================================================
{
    m_event = event;
}

//========================================================
void ViewerThread::SetRecvBytes(const QByteArray &src)
//========================================================
{
    m_bytes = src;
}

//========================================================
void ViewerThread::run()
//========================================================
{
    if(VIEWER_EVENT::Event::MAKE_QRCODE == m_event)
    {
        ExecMakeQrCodeThread();
    }
    else
    {
        qDebug() << __FUNCTION__ << "Unkwon Event" ;
    }
}

//========================================================
void ViewerThread::ExecMakeQrCodeThread()
//========================================================
{
    /**
    * @note
    *       ELGO_MAIN -> ELGO_VIEWER
    *       Viewer will make qr code image and display.
    * @param
    *       QString ip
    */

    QByteArray recvBytes = m_bytes;
    QDataStream recvStream(&recvBytes, QIODevice::ReadOnly);
    QString ip;
    recvStream >> ip;
    ELGO_VIEWER_LOG("IP : %s", ip.toUtf8().constData());

    // test
    qmlRegisterType<QrMaker>("QRCode", 1, 0, "QRMaker");

    QQuickView view;
    view.setSource(QUrl("qrc:/QrCodeDispay.qml"));
    view.show();
}
