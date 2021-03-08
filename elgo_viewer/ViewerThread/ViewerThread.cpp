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
    QString url = "http://";
    QString ip;
    recvStream >> ip;
    url.append(ip);
    url.append(":3000/remote");
    ViewerController::GetInstance()->GetViewerCtrl().SetQRCodeURL(url);
    emit MainWindow::GetInstance()->DrawQRCode();
    ELGO_VIEWER_LOG("Emit SIGNAL to GUI - DRAW QRCODE, url : %s", url.toUtf8().constData());
}
