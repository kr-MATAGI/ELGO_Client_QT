// QT
#include <QGuiApplication>
#include <QScreen>
#include <QWindow>
#include <QQuickView>
#include <QThread>

// EFC
#include "LocalSocketEvent/EFCEvent.h"

// Viewer
#include "Definition/ViewerDefinition.h"
#include "Logger/ViewerLogger.h"
#include "QrCode/QrMaker.h"
#include "ViewerThread.h"

// Common
#include "Common/Interface/ContentsPlayDataImpl.h"
#include "Common/Interface/ScheduleImpl.h"

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
    else if(VIEWER_EVENT::Event::PLAY_CUSTOM_PLAY_DATA == m_event)
    {
        ExecPlayCustomPlayData();
    }
    else if(VIEWER_EVENT::Event::PLAY_FIXED_PLAY_DATA == m_event)
    {
        ExecPlayFixedPlayData();
    }
    else if(VIEWER_EVENT::Event::REQUEST_SCREEN_CAPTURE == m_event)
    {
        ExecScreenCapture();
    }
    else
    {
        ELGO_VIEWER_LOG("Error - Unknwon Event : %d", m_event);
    }
}

//========================================================
void ViewerThread::ExecMakeQrCodeThread()
//========================================================
{
    /**
     * @note
     *       ELGO_CONTROL -> ELGO_VIEWER
     *       make new qr code and display it.
     * @param
     *       QString    ip
     *       QString    deviceName
     */
    QDataStream recvStream(&m_bytes, QIODevice::ReadOnly);
    QString url = "http://";
    QString ip;
    QString deviceName;
    recvStream >> ip;
    recvStream >> deviceName;

    url.append(ip);
    url.append(":3000/remote");

    emit MainWindow::GetInstance()->DrawQRCode(url, deviceName);
    ELGO_VIEWER_LOG("Emit SIGNAL to GUI - DRAW QRCODE {url: %s, deviceName: %s}",
                    url.toStdString().c_str(), deviceName.toStdString().c_str());

}

//========================================================
void ViewerThread::ExecPlayCustomPlayData()
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
    PlayJson::CustomPlayDataJson customPlayData;
    recvStream >> customPlayData;

    ELGO_VIEWER_LOG("custom - id : %d, name : %s",
                    customPlayData.playData.id,
                    customPlayData.playData.name.toStdString().c_str());

    // Add Play Data and Play
    emit ContentsPlayer::GetInstance()->AddPlayDataSignal(customPlayData);
    emit ContentsPlayer::GetInstance()->ExecPlayDataSignal(customPlayData.playData, true);

    /**
     *  @note
     *          ELGO_VIEWER -> ELGO_MAIN
     *          Save Current Playing Data to DB
     *  @param
     *          int playDataId
     *          PlayJson::PlayDataType type
     */
    // Save playing data to DB
    QByteArray sendBytes;
    QDataStream sendStream(&sendBytes, QIODevice::WriteOnly);
    sendStream << customPlayData.playData.id;
    sendStream << customPlayData.playData.playDataType;
    const bool bSendEvent = EFCEvent::SendEvent(ELGO_SYS::Proc::ELGO_MAIN,
                                                MAIN_EVENT::Event::SAVE_PLAYING_DATA_TO_DB,
                                                sendBytes);
    if(false == bSendEvent)
    {
        ELGO_VIEWER_LOG("ERROR - SendEvent: %d", MAIN_EVENT::Event::SAVE_PLAYING_DATA_TO_DB);
    }
}

//========================================================
void ViewerThread::ExecPlayFixedPlayData()
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
    PlayJson::FixedPlayDataJson fixedPlayData;
    recvStream >> fixedPlayData;

    ELGO_VIEWER_LOG("fixed - id : %d, name : %s",
                    fixedPlayData.playData.id,
                    fixedPlayData.playData.name.toStdString().c_str());

    // Add Play Data
    emit ContentsPlayer::GetInstance()->AddPlayDataSignal(fixedPlayData);
    emit ContentsPlayer::GetInstance()->ExecPlayDataSignal(fixedPlayData.playData, true);

    /**
     *  @note
     *          ELGO_VIEWER -> ELGO_MAIN
     *          Save Current Playing Data to DB
     *  @param
     *          int playDataId
     *          PlayJson::PlayDataType type
     */
    // Save playing data to DB
    QByteArray sendBytes;
    QDataStream sendStream(&sendBytes, QIODevice::WriteOnly);
    sendStream << fixedPlayData.playData.id;
    sendStream << fixedPlayData.playData.playDataType;
    const bool bSendEvent = EFCEvent::SendEvent(ELGO_SYS::Proc::ELGO_MAIN,
                                                MAIN_EVENT::Event::SAVE_PLAYING_DATA_TO_DB,
                                                sendBytes);
    if(false == bSendEvent)
    {
        ELGO_VIEWER_LOG("ERROR - SendEvent: %d", MAIN_EVENT::Event::SAVE_PLAYING_DATA_TO_DB);
    }
}

//========================================================
void ViewerThread::ExecScreenCapture()
//========================================================
{
    /**
    * @note
    *       ELGO_CONTROL -> ELGO_VIEWER
    *       capture current screen.
    *       if capture is successed, response to elgo_control.
    *       Response Event : RESPONSE_SCREEN_CAPTURE
    * @param
    *       QString payload.src
    *       QString payload.dest
    */

    QDataStream recvStream(&m_bytes, QIODevice::ReadOnly);
    QString payloadSrc;
    QString payloadDest;
    recvStream >> payloadSrc;
    recvStream >> payloadDest;
    ELGO_VIEWER_LOG("Recv src - %s, dest - %s",
                    payloadSrc.toUtf8().constData(), payloadDest.toUtf8().constData());

    // Screen capture
    bool bIsSuccessed = false;
    const QString& savedPath = CAPATURE_SAVE_PATH;
    const bool bIsCaptured = CaptureCurrentScreen(savedPath);
    if(true == bIsCaptured)
    {
        bIsSuccessed = true;
    }

    // Send response
    /**
     * @note
     *       ELGO_VIEWER -> ELGO_CONTORL
     *       Receive screen capture response.
     * @param
     *       QString payload.src
     *       QString payload.dest
     *       QString imagePath
     *       bool    bIsSuccessed
     */

    QByteArray sendBytes;
    QDataStream sendStream(&sendBytes, QIODevice::WriteOnly);
    sendStream << payloadSrc;
    sendStream << payloadDest;
    sendStream << savedPath;
    sendStream << bIsSuccessed;

    const bool bSendEvent = EFCEvent::SendEvent(ELGO_SYS::Proc::ELGO_CONTROL,
                                                CONTROL_EVENT::Event::RESPONSE_SCREEN_CAPTURE, sendBytes);
    if(false == bSendEvent)
    {
        ELGO_VIEWER_LOG("Error - Send Event : %d", CONTROL_EVENT::Event::RESPONSE_SCREEN_CAPTURE);
    }
}

//========================================================
bool ViewerThread::CaptureCurrentScreen(const QString& savedPath)
//========================================================
{
    bool retValue = true;

    QScreen *screen = QGuiApplication::primaryScreen();

    if(NULL == screen)
    {
        ELGO_VIEWER_LOG("ERROR - NULL == screen");
    }
    else
    {
        QPixmap pixmap = screen->grabWindow(0);
        if(false == pixmap.save(savedPath))
        {
            retValue = false;
            ELGO_VIEWER_LOG("ERROR - Not saved pixmap { path : %s }", savedPath.toStdString().c_str());
        }
        else
        {
            ELGO_VIEWER_LOG("Saved Pixmap { path : %s }", savedPath.toStdString().c_str());
        }
    }

    return retValue;
}
