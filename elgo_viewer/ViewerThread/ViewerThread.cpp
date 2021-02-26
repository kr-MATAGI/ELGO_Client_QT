// QT
#include <QQuickView>

// EFC
#include "ShardMem/ShmCtrl.h"

// Viewer
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
    qDebug() << __FUNCTION__;
//    qmlRegisterType<QrMaker>("QRCode", 1, 0, "QRMaker");

//    QQuickView view;
//    view.setSource(QUrl("qrc:/contentPlayer.qml"));
//    view.show();

    // Read IP from shared memory
    QString ip;
    ShmCtrl shmCtrl;
    QByteArray shmBytes;
    QDataStream shmRead(&shmBytes, QIODevice::ReadWrite);
    shmRead.setVersion(QDataStream::Qt_5_12);
    shmCtrl.ShmRead(SHM_NAME::SHM_IP, shmBytes);
    shmRead >> ip;
    qDebug() << ip.toUtf8().constData();
}
