#include "ViewerThread.h"

// QT
#include <QQuickView>

#include "QrCode/QrMaker.h"

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
}

//========================================================
void ViewerThread::ExecMakeQrCodeThread()
//========================================================
{
    qmlRegisterType<QrMaker>("QRCode", 1, 0, "QRMaker");

    QQuickView view;
    view.setSource(QUrl("qrc:/contentPlayer.qml"));
    view.show();
}
