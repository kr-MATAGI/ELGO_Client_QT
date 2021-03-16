// QT
#include <QDataStream>

// EFC
#include "Common/Deifinition.h"

// Control
#include "Logger/ControlLogger.h"
#include "ControlEventState.h"
#include "ControlThread/ControlThread.h"

//========================================================
ControlEventState::ControlEventState()
//========================================================
{
    m_threadPool = new QThreadPool;
    m_threadPool->setMaxThreadCount(MAX_THREAD_COUNT);

    // proc Event
    m_state.RegisterEvent(CONTROL_EVENT::Event::RECV_SERVER_INFO_FROM_MAIN,
                          &ControlEventState::RecvServerInfoFromMain);

    m_state.RegisterEvent(CONTROL_EVENT::Event::UPDATE_DISPLAY_SLEEP_STATUS,
                          &ControlEventState::RecvUpdateDisplaySleepStatus);


}

//========================================================
ControlEventState::~ControlEventState()
//========================================================
{
    delete m_threadPool;
    m_threadPool = NULL;
}

//========================================================
void ControlEventState::ExecState(quint16 event, QByteArray &src)
//========================================================
{
    m_state.Exec(event, src);
}


//========================================================
void ControlEventState::RecvServerInfoFromMain(QByteArray &src)
//========================================================
{
    /**
     *  @brief  receive WIFI information from main process
     *  @param
     *          QString wasHost,
     *          int wasHostPort,
     *          QString remoteTCPHost
     */

    ControlThread *thread = new ControlThread;
    thread->SetControlEvent(CONTROL_EVENT::Event::RECV_SERVER_INFO_FROM_MAIN);
    thread->SetRecvBytes(src);
    m_threadPool->start(thread);
}

//========================================================
void ControlEventState::RecvUpdateDisplaySleepStatus(QByteArray& src)
//========================================================
{
    /**
     *  @brief  Update display sleep status
     *  @param  bool isDisplaySleep
     */

    QDataStream out(&src, QIODevice::ReadOnly);
    bool bIsDisplaySleep = false;
    out >> bIsDisplaySleep;

    const bool prevStatus = NetworkController::GetInstance()->GetNetworkCtrl().GetDisplaySleepStatus();
    if(prevStatus != bIsDisplaySleep)
    {
        NetworkController::GetInstance()->GetNetworkCtrl().SetDisplaySleepStatus(bIsDisplaySleep);
        ELGO_CONTROL_LOG("Update Display sleep status : %d -> %d", prevStatus, bIsDisplaySleep);
    }
    else
    {
        ELGO_CONTROL_LOG("NOT Updated Display sleep status : %d", prevStatus);
    }
}
