// QT
#include <QDebug>

// EFC
#include "Common/Deifinition.h"
#include "LocalSocketEvent/EFCEvent.h"

// Control
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
    if(CONTROL_EVENT::Event::RECV_WIFI_INFO_FROM_MAIN == m_event)
    {
        ExecRecvWifiInfoFromMain();
    }
    else
    {
        qDebug() << __FUNCTION__ << "Unkwon Event";
    }
}

//========================================================
void ControlThread::ExecRecvWifiInfoFromMain()
//========================================================
{

}
