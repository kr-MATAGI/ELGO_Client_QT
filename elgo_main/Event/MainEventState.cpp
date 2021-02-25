#include "MainEventState.h"
#include "MainThread/MainThread.h"

#include <QDataStream>

//========================================================
MainEventState::MainEventState()
//========================================================
{
    m_threadPool = new QThreadPool;
    m_threadPool->setMaxThreadCount(MAX_THREAD_COUNT);

    // enroll event
    m_state.ReigsterEvent(VIEWER_EVENT::Event::MAKE_QRCODE, &MainEventState::RecvProcecssReady);
}

//========================================================
MainEventState::~MainEventState()
//========================================================
{

}

//========================================================
void MainEventState::ExecState(MAIN_EVENT::Event event, QByteArray &src)
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

    QDataStream out(&src, QIODevice::ReadOnly);
    out.setVersion(QDataStream::Qt_5_12);
    MAIN_EVENT::Event event = MAIN_EVENT::Event::NONE_MAIN_EVENT;
    QByteArray recv;
    out >> event;
    out >> recv;

    MainThread *thread = new MainThread;
    thread->SetMainEvent(event);
    thread->SetRecvBytes(recv);
    m_threadPool->start(thread);
}
