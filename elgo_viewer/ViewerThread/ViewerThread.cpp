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
    else if(VIEWER_EVENT::Event::ROTATE_DISPLAY == m_event)
    {
        ExecRotateDeviceDisplay();
    }
    else if(VIEWER_EVENT::Event::CUSTOM_PLAY_DATA == m_event)
    {
        ExecCustomPlayData();
    }
    else if(VIEWER_EVENT::Event::FIXED_PLAY_DATA == m_event)
    {
        ExecFixedPlayData();
    }
    else if(VIEWER_EVENT::Event::CUSTOM_PLAY_SCHEDULES == m_event)
    {
        ExecCustomPlaySchedules();
    }
    else if(VIEWER_EVENT::Event::FIXED_PLAY_SCHEDULES == m_event)
    {
        ExecFixedPlaySchedules();
    }
    else if(VIEWER_EVENT::Event::REQUEST_SCREEN_CAPTURE == m_event)
    {
        ExecScreenCapture();
    }
    else
    {
        ELGO_VIEWER_LOG("Error - Unkwon Event : %d", m_event);
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
void ViewerThread::ExecCustomPlayData()
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

    SchedulesTimer::GetInstance()->AddPlayDataList(customPlayData, true);
}

//========================================================
void ViewerThread::ExecFixedPlayData()
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

    SchedulesTimer::GetInstance()->AddPlayDataList(fixedPlayData, true);
}

//========================================================
void ViewerThread::ExecCustomPlaySchedules()
//========================================================
{
    /**
    * @note
    *       ELGO_CONTROL -> ELGO_VIEWER
    *       Send custom play data information
    *       with schedules
    * @param
    *       CustomPlayDataJson customPlayData
    *       QVector<PlaySchedules> schedules
    */

    PlayJson::CustomPlayDataJson customPlayData;
    QVector<ScheduleJson::PlaySchedules> scheduleList;

    QDataStream recvStream(&m_bytes, QIODevice::ReadOnly);
    recvStream >> customPlayData;
    recvStream >> scheduleList;
    for(int idx = 0; idx < scheduleList.size(); idx++)
    {
        SchedulesTimer::GetInstance()->AddPlaySchedule(scheduleList[idx]);
    }

    SchedulesTimer::GetInstance()->AddPlayDataList(customPlayData);
}

//========================================================
void ViewerThread::ExecFixedPlaySchedules()
//========================================================
{
    /**
    * @note
    *       ELGO_CONTROL -> ELGO_VIEWER
    *       Send fixed play data information
    *       with schedules
    * @param
    *       FixedPlayDataJson customPlayData
    *       QVector<PlaySchedules> schedules
    */

    PlayJson::FixedPlayDataJson fixedPlayData;
    QVector<ScheduleJson::PlaySchedules> scheduleList;

    QDataStream recvStream(&m_bytes, QIODevice::ReadOnly);
    recvStream >> fixedPlayData;
    recvStream >> scheduleList;

    for(int idx = 0; idx < scheduleList.size(); idx++)
    {
        SchedulesTimer::GetInstance()->AddPlaySchedule(scheduleList[idx]);
    }
    SchedulesTimer::GetInstance()->AddPlayDataList(fixedPlayData);
}

//========================================================
void ViewerThread::ExecScreenCapture()
//========================================================
{
    /**
    * @note
    *       ELGO_CONTROL -> ELGO_VIEWER
    *       current Screen capture on elgo_viewer
    *       if capture is successed, response to elgo_control
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
    *       ELGO_CONTROL -> ELGO_VIEWER
    *       Receive screen capture response from elgo_viewer
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
