// QT
#include <QDebug>
#include <QDataStream>

// EFC
#include "Common/Deifinition.h"
#include "LocalSocketEvent/EFCEvent.h"

// Control
#include "ControlThread.h"
#include "Logger/ControlLogger.h"
#include "JSON/JsonWriter.h"


//========================================================
ControlThread::ControlThread()
    : m_event(CONTROL_EVENT::Event::NONE_CONTROL_EVENT)
//========================================================
{

}

//========================================================
ControlThread::~ControlThread()
//========================================================
{

}

//========================================================
void ControlThread::SetControlEvent(const CONTROL_EVENT::Event event)
//========================================================
{
    m_event = event;
}

//========================================================
void ControlThread::SetRecvBytes(const QByteArray &src)
//========================================================
{
    m_bytes = src;
}

//========================================================
void ControlThread::run()
//========================================================
{
    if(CONTROL_EVENT::Event::RECV_SERVER_INFO_FROM_MAIN == m_event)
    {
        ExecRecvServerInfoFromMain();
    }
    else if(CONTROL_EVENT::Event::RESPONSE_SCREEN_CAPTURE == m_event)
    {
        ExecResponseScreenCapture();
    }
    else
    {
        ELGO_CONTROL_LOG("Unknwon Event %d", m_event);
    }
}

//========================================================
void ControlThread::ExecRecvServerInfoFromMain()
//========================================================
{
    /**
     *  @note
     *          ELGO_MAIN -> ELGO_CONTROL
     *          receive server connection information.
     *  @param
     *          QString wasHost,
     *          quint16 wasHostPort,
     *          QString remoteTCPHost
     *          bool bIsDisplaySleep
     */

    QDataStream recvStream(&m_bytes, QIODevice::ReadOnly);
    QString wasHost;
    quint16 wasHostPort = 0;
    QString remoteTCPHost;
    bool bIsDisplaySleep = false;

    recvStream >> wasHost;
    recvStream >> wasHostPort;
    recvStream >> remoteTCPHost;
    recvStream >> bIsDisplaySleep;
    ELGO_CONTROL_LOG("WAS {Host : %s, port : %u }, remoteHost : %s",
                     wasHost.toUtf8().constData(), wasHostPort,
                     remoteTCPHost.toUtf8().constData());

    // Set Info into NetCtrl
    CONNECT_INFO connectInfo(wasHost, wasHostPort, remoteTCPHost);
    NetworkController::GetInstance()->GetNetworkCtrl().SetConnectInfo(connectInfo);

    // Remote Server Start
    emit RemoteControlServer::GetInstance()->RemoteControlServerStartSignal();

    // Connect Content Server
    NetworkController::GetInstance()->GetNetworkCtrl().ConnectContentWebSocketToServer();

    // Set Display Sleep Status
    NetworkController::GetInstance()->GetNetworkCtrl().SetDisplaySleepStatus(bIsDisplaySleep);
}

//========================================================
void ControlThread::ExecResponseScreenCapture()
//========================================================
{
    /**
    * @note
    *       ELGO_CONTROL -> ELGO_VIEWER
    *       Receive screen capture response.
    * @param
    *       QString payload.src
    *       QString payload.dest
    *       QString imagePath
    *       bool    bIsSuccessed
    */

    ContentSchema::Summary response;
    QString imgSavedPath;
    bool bIsSuccessed = false;

    QDataStream recvStream(&m_bytes, QIODevice::ReadOnly);
    response.event = ContentSchema::Event::SCREEN_CAPTURE;
    response.payload.type = ContentSchema::PayloadType::RESPONSE;
    recvStream >> response.payload.dest;
    recvStream >> response.payload.src;
    recvStream >> imgSavedPath;
    recvStream >> bIsSuccessed;

    bool bIsUploaded = false;
    if(true == bIsSuccessed)
    {
        QString uploadedPath;
        bIsUploaded = NetworkController::GetInstance()->GetNetworkCtrl().UploadScreenCaptureImage(imgSavedPath, uploadedPath);
        if(true == bIsUploaded)
        {
            response.payload.path = uploadedPath;

            QString sendJson;
            JsonWriter::WriteContentServerScreenCaptureEvent(response, sendJson);
            NetworkController::GetInstance()->GetNetworkCtrl().GetContentWebSocket().SendTextMessageToServer(sendJson);
        }
        else
        {
            ELGO_CONTROL_LOG("Error - Failed Upload : %s", imgSavedPath.toUtf8().constData());
        }
    }
    else
    {
        QString sendJson;
        QString errorStr;
        if(false == bIsSuccessed)
        {
            errorStr = "Failed Screen Capture";
        }
        else if(false == bIsUploaded)
        {
            errorStr = "Failed Upload File";
        }
        JsonWriter::WriteContentServerErrorResponse(response, sendJson, errorStr);
    }
}
