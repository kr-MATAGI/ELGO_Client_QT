// QT
#include <QDataStream>

// EFC
#include "Common/Deifinition.h"

// Control
#include "Logger/ControlLogger.h"
#include "ControlEventState.h"
#include "ControlThread/ControlThread.h"
#include "DownloadThread/DownloadThread.h"

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
    m_state.RegisterEvent(CONTROL_EVENT::Event::RESPONSE_SCREEN_CAPTURE,
                          &ControlEventState::RecvResponseScreenCapture);
}

//========================================================
ControlEventState::~ControlEventState()
//========================================================
{
    delete m_threadPool;
    m_threadPool = NULL;
}

//========================================================
void ControlEventState::ExecState(const quint16 event, const QByteArray &src)
//========================================================
{
    m_state.Exec(event, src);
}

//========================================================
void ControlEventState::RecvServerInfoFromMain(const QByteArray &src)
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

    ControlThread *thread = new ControlThread;
    thread->SetControlEvent(CONTROL_EVENT::Event::RECV_SERVER_INFO_FROM_MAIN);
    thread->SetRecvBytes(src);

    m_threadPool->start(thread);
}

//========================================================
void ControlEventState::RecvUpdateDisplaySleepStatus(const QByteArray& src)
//========================================================
{
    /**
     *  @note
     *          ELGO_MAIN -> ELGO_CONTROL
     *          Update display sleep status
     *  @param
     *          bool isDisplaySleep
     */

    QByteArray recvBytes = src;
    QDataStream out(&recvBytes, QIODevice::ReadOnly);
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

//========================================================
void ControlEventState::RecvResponseScreenCapture(const QByteArray& src)
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

    ControlThread *thread = new ControlThread;
    thread->SetControlEvent(CONTROL_EVENT::Event::RESPONSE_SCREEN_CAPTURE);
    thread->SetRecvBytes(src);

    m_threadPool->start(thread);
}
