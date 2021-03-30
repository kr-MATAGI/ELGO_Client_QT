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
        ELGO_CONTROL_LOG("Unkwon Event %d", m_event);
    }
}

//========================================================
void ControlThread::ExecRecvServerInfoFromMain()
//========================================================
{
    /**
     *  @note
     *          ELGO_MAIN -> ELGO_CONTROL
     *          receive WIFI information from main process
     *  @param
     *          QString wasHost,
     *          quint16 wasHostPort,
     *          QString remoteTCPHost
     */

    QDataStream recvStream(&m_bytes, QIODevice::ReadOnly);
    QString wasHost;
    quint16 wasHostPort;
    QString remoteTCPHost;

    recvStream >> wasHost;
    recvStream >> wasHostPort;
    recvStream >> remoteTCPHost;
    ELGO_CONTROL_LOG("WAS {Host : %s, port : %u }, remoteHost : %s",
                     wasHost.toUtf8().constData(), wasHostPort, remoteTCPHost.toUtf8().constData());

    // Set Info into NetCtrl
    CONNECT_INFO connectInfo(wasHost, wasHostPort, remoteTCPHost);
    NetworkController::GetInstance()->GetNetworkCtrl().SetConnectInfo(connectInfo);

    // Remote Server Start
    emit RemoteControlServer::GetInstance()->RemoteControlServerStartSignal();

    // Connect Content Server
    NetworkController::GetInstance()->GetNetworkCtrl().ConnectContentWebSocketToServer();
}

//========================================================
void ControlThread::ExecResponseScreenCapture()
//========================================================
{
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

    if(true == bIsSuccessed)
    {
        QString uploadedPath;
        const bool bIsUploaded =
                NetworkController::GetInstance()->GetNetworkCtrl().UploadScreenCaptureImage(imgSavedPath, uploadedPath);
        if(true == bIsUploaded)
        {
            response.payload.path = uploadedPath;

            QString sendJson;
            JsonWriter::WriteContentServerScreenCaptureEvent(response, sendJson);
            NetworkController::GetInstance()->GetNetworkCtrl().GetContentWebSocket().SendTextMessageToServer(sendJson);
        }
        else
        {

        }
    }
    else
    {
        // TODO : Error Response
    }
}
