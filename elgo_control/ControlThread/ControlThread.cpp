// QT
#include <QDebug>
#include <QDataStream>

// EFC
#include "Common/Deifinition.h"
#include "LocalSocketEvent/EFCEvent.h"

// Control
#include "Logger/ControlLogger.h"
#include "ControlThread.h"

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
     *  @brief  receive WIFI information from main process
     *  @param
     *          QString wasHost,
     *          int wasHostPort,
     *          QString remoteTCPHost
     */

    QByteArray recvBytes = m_bytes;
    QDataStream out(&recvBytes, QIODevice::ReadOnly);
    QString wasHost;
    quint16 wasHostPort;
    QString remoteTCPHost;

    out >> wasHost;
    out >> wasHostPort;
    out >> remoteTCPHost;
    ELGO_CONTROL_LOG("WAS {Host : %s, port : %u }, remoteTCPHost : %s",
                     wasHost.toUtf8().constData(), wasHostPort, remoteTCPHost.toUtf8().constData());

    // Set Info into NetCtrl
    CONNECT_INFO connectInfo(wasHost, wasHostPort, remoteTCPHost);
    NetworkController::GetInstance()->GetNetworkCtrl().SetConnectInfo(connectInfo);

    // TCP Server Start
    RemoteControlServer::GetInstance()->TCPServerStartSignal();
}
