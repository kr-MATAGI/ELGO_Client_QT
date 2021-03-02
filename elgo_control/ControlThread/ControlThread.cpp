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
     *          QString mainSocket,
     *          int mainSocketPort
     */

    QByteArray recvBytes = m_bytes;
    QDataStream out(&recvBytes, QIODevice::ReadOnly);
    out.setVersion(QDataStream::Qt_5_12);
}
