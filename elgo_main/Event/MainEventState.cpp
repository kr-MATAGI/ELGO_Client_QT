// QT
#include <QDataStream>

// Main
#include "MainEventState.h"
#include "MainThread/MainThread.h"

//========================================================
MainEventState::MainEventState()
//========================================================
{
    m_threadPool = new QThreadPool;
    m_threadPool->setMaxThreadCount(MAX_THREAD_COUNT);

    // enroll event
    m_state.RegisterEvent(MAIN_EVENT::Event::PROCESS_IS_READY,
                          &MainEventState::RecvProcecssReady);
    m_state.RegisterEvent(MAIN_EVENT::Event::CHANGE_DEVICE_OPTIONS,
                          &MainEventState::RecvChangeDeviceOptions);
}

//========================================================
MainEventState::~MainEventState()
//========================================================
{

}

//========================================================
void MainEventState::ExecState(quint16 event, QByteArray &src)
//========================================================
{
    m_state.Exec(event, src);;
}

//========================================================
void MainEventState::RecvProcecssReady(QByteArray &src)
//========================================================
{
    /**
     *  @brief  receive status of process started
     *  @param  ELGO_PROC::Proc proc
     */

    MainThread *thread = new MainThread;
    thread->SetMainEvent(MAIN_EVENT::Event::PROCESS_IS_READY);
    thread->SetRecvBytes(src);
    m_threadPool->start(thread);
}

//========================================================
void MainEventState::RecvChangeDeviceOptions(QByteArray& src)
//========================================================
{
    /**
     *  @brief  Change Device Options
     *  @param
     *          bool displayOnOff
     *          bool deviceMute
     *          bool contentPause
     */

    MainThread *thread = new MainThread;
    thread->SetMainEvent(MAIN_EVENT::Event::CHANGE_DEVICE_OPTIONS);
    thread->SetRecvBytes(src);
    m_threadPool->start(thread);
}
