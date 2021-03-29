#include "ViewerEventState.h"
#include "ViewerThread/ViewerThread.h"

#include <QDataStream>

//========================================================
ViewerEventState::ViewerEventState()
//========================================================
{
    m_threadPool = new QThreadPool;
    m_threadPool->setMaxThreadCount(MAX_THREAD_COUNT);

    // enorll Event
    m_state.RegisterEvent(VIEWER_EVENT::Event::MAKE_QRCODE,
                          &ViewerEventState::MakeQrCodeAndDisplay);
    m_state.RegisterEvent(VIEWER_EVENT::Event::ROTATE_DISPLAY,
                          &ViewerEventState::RotateDeviceDisplay);
    m_state.RegisterEvent(VIEWER_EVENT::Event::CUSTOM_PLAY_DATA,
                          &ViewerEventState::RecvCustomPlayData);
    m_state.RegisterEvent(VIEWER_EVENT::Event::FIXED_PLAY_DATA,
                          &ViewerEventState::RecvFixedPlayData);
}

//========================================================
ViewerEventState::~ViewerEventState()
//========================================================
{
    delete m_threadPool;
    m_threadPool = NULL;
}

//========================================================
void ViewerEventState::ExecState(quint16 event, QByteArray &src)
//========================================================
{
    m_state.Exec(event, src);;
}

//========================================================
void ViewerEventState::MakeQrCodeAndDisplay(QByteArray &src)
//========================================================
{
    /**
    * @note
    *       ELGO_CONTROL -> ELGO_VIEWER
    *       Viewer will make qr code image and display.
    * @param
    *       QString ip
    */
    QDataStream out(&src, QIODevice::ReadOnly);
    out.setVersion(QDataStream::Qt_5_12);

    ViewerThread *thread = new ViewerThread;
    thread->SetViewerEvent(VIEWER_EVENT::Event::MAKE_QRCODE);
    thread->SetRecvBytes(src);
    m_threadPool->start(thread);
}

//========================================================
void ViewerEventState::RotateDeviceDisplay(QByteArray& src)
//========================================================
{
    /**
    * @note
    *       ELGO_CONTROL -> ELGO_VIEWER
    *       Rotate Display accroding to heading enum value.
    * @param
    *      quint8   heading (top : 1, right : 2, bottom : 3, left : 4)
    */
    ViewerThread *thread = new ViewerThread;
    thread->SetViewerEvent(VIEWER_EVENT::Event::ROTATE_DISPLAY);
    thread->SetRecvBytes(src);
    m_threadPool->start(thread);
}

//========================================================
void ViewerEventState::RecvCustomPlayData(QByteArray& src)
//========================================================
{
    /**
    * @note
    *       ELGO_CONTROL -> ELGO_VIEWER
    *       Send custom play data information
    * @param
    *       CustomPlayDataJson customPlayData
    */
    ViewerThread *thread = new ViewerThread;
    thread->SetViewerEvent(VIEWER_EVENT::Event::CUSTOM_PLAY_DATA);
    thread->SetRecvBytes(src);
    m_threadPool->start(thread);
}

//========================================================
void ViewerEventState::RecvFixedPlayData(QByteArray& src)
//========================================================
{
    /**
    * @note
    *       ELGO_CONTROL -> ELGO_VIEWER
    *       Send fixed play data information
    * @param
    *       FixedPlayDataJson customPlayData
    */
    ViewerThread *thread = new ViewerThread;
    thread->SetViewerEvent(VIEWER_EVENT::Event::FIXED_PLAY_DATA);
    thread->SetRecvBytes(src);
    m_threadPool->start(thread);
}
