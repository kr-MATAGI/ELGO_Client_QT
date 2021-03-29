// QT
#include <QQuickView>
#include <QThread>

// EFC
#include "ShardMem/ShmCtrl.h"

// Viewer
#include "Definition/ViewerDefinition.h"
#include "Logger/ViewerLogger.h"
#include "QrCode/QrMaker.h"
#include "ViewerThread.h"

// Common
#include "Common/Interface/ContentsPlayDataImpl.h"

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
    else if(VIEWER_EVENT::Event::ROTATE_DISPLAY == m_event)
    {
        ExecRotateDeviceDisplay();
    }
    else if(VIEWER_EVENT::Event::CUSTOM_PLAY_DATA == m_event)
    {
        ExecRecvCustomPlayData();
    }
    else if(VIEWER_EVENT::Event::FIXED_PLAY_DATA == m_event)
    {
        ExecRecvFixedPlayData();
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
    *       ELGO_CONTROL -> ELGO_VIEWER
    *       Viewer will make qr code image and display.
    * @param
    *       QString ip
    */

    const bool bIsDisplayQr = ViewerController::GetInstance()->GetViewerCtrl().GetIsDisplayQr();
    if(true == bIsDisplayQr)
    {
        ELGO_VIEWER_LOG("Already Displayed Remote QR Code");
    }
    else
    {
        ViewerController::GetInstance()->GetViewerCtrl().SetIsDisplayQr(true);
        QDataStream recvStream(&m_bytes, QIODevice::ReadOnly);
        QString url = "http://";
        QString ip;
        recvStream >> ip;
        url.append(ip);
        url.append(":3000/remote");
        ViewerController::GetInstance()->GetViewerCtrl().SetQRCodeURL(url);
        emit MainWindow::GetInstance()->DrawQRCode();
        ELGO_VIEWER_LOG("Emit SIGNAL to GUI - DRAW QRCODE, url : %s", url.toUtf8().constData());
    }
}

//========================================================
void ViewerThread::ExecRotateDeviceDisplay()
//========================================================
{
    /**
    * @note
    *       ELGO_CONTROL -> ELGO_VIEWER
    *       Rotate Display accroding to heading enum value.
    * @param
    *      quint8   heading (top : 1, right : 2, bottom : 3, left : 4)
    */

    QDataStream recvStream(&m_bytes, QIODevice::ReadOnly);
    VIEWER_DEF::HEADING heading;
    quint8 data;
    recvStream >> data;
    heading = static_cast<VIEWER_DEF::HEADING>(data);
    ELGO_VIEWER_LOG("Rotate device display : %d", heading);

    // TODO : rotate content display window accroding to heading degree.



}

//========================================================
void ViewerThread::ExecRecvCustomPlayData()
//========================================================
{
    /**
    * @note
    *       ELGO_CONTROL -> ELGO_VIEWER
    *       Send custom play data information
    * @param
    *       CustomPlayDataJson customPlayData
    */

    QDataStream recvStream(&m_bytes, QIODevice::ReadOnly);
    ObjectJson::CustomPlayDataJson customPlayData;
    ObjectJsonDataStream::operator>>(recvStream, customPlayData);

    ELGO_VIEWER_LOG("custom name : %s", customPlayData.playData.name.toStdString().c_str());
}

//========================================================
void ViewerThread::ExecRecvFixedPlayData()
//========================================================
{
    /**
    * @note
    *       ELGO_CONTROL -> ELGO_VIEWER
    *       Send fixed play data information
    * @param
    *       FixedPlayDataJson customPlayData
    */

    QDataStream recvStream(&m_bytes, QIODevice::ReadOnly);
    ObjectJson::FixedPlayDataJson fixedPlayData;
    ObjectJsonDataStream::operator>>(recvStream, fixedPlayData);

    ELGO_VIEWER_LOG("fixed name : %s", fixedPlayData.playData.name.toStdString().c_str());
}

