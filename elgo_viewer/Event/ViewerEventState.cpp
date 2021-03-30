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
    m_state.RegisterEvent(VIEWER_EVENT::Event::CUSTOM_PLAY_DATA_SCHEDULES,
                          &ViewerEventState::RecvCustomPlaySchedules);
    m_state.RegisterEvent(VIEWER_EVENT::Event::FIXED_PLAY_DATA_SCHEDULES,
                          &ViewerEventState::RecvFixedPlaySchedules);
    m_state.RegisterEvent(VIEWER_EVENT::Event::REQUEST_SCREEN_CAPTURE,
                          &ViewerEventState::RecvRequestScreenCapture);
}

//========================================================
ViewerEventState::~ViewerEventState()
//========================================================
{
    delete m_threadPool;
    m_threadPool = NULL;
}

//========================================================
void ViewerEventState::ExecState(const quint16 event, const QByteArray &src)
//========================================================
{
    m_state.Exec(event, src);;
}

//========================================================
void ViewerEventState::MakeQrCodeAndDisplay(const QByteArray &src)
//========================================================
{
    /**
    * @note
    *       ELGO_CONTROL -> ELGO_VIEWER
    *       Viewer will make qr code image and display.
    * @param
    *       QString ip
    */
    ViewerThread *thread = new ViewerThread;
    thread->SetViewerEvent(VIEWER_EVENT::Event::MAKE_QRCODE);
    thread->SetRecvBytes(src);

    m_threadPool->start(thread);
}

//========================================================
void ViewerEventState::RotateDeviceDisplay(const QByteArray& src)
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
void ViewerEventState::RecvCustomPlayData(const QByteArray& src)
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
void ViewerEventState::RecvFixedPlayData(const QByteArray& src)
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

//========================================================
void ViewerEventState::RecvCustomPlaySchedules(const QByteArray& src)
//========================================================
{
    /**
    * @note
    *       ELGO_CONTROL -> ELGO_VIEWER
    *       Send fixed play data information
    *       with schedules
    * @param
    *       FixedPlayDataJson customPlayData
    *       int scheduleCount
    *       PlaySchedules schedules
    */
    ViewerThread *thread = new ViewerThread;
    thread->SetViewerEvent(VIEWER_EVENT::Event::CUSTOM_PLAY_DATA_SCHEDULES);
    thread->SetRecvBytes(src);

    m_threadPool->start(thread);
}

//========================================================
void ViewerEventState::RecvFixedPlaySchedules(const QByteArray& src)
//========================================================
{
    /**
    * @note
    *       ELGO_CONTROL -> ELGO_VIEWER
    *       Send fixed play data information
    *       with schedules
    * @param
    *       FixedPlayDataJson customPlayData
    *       int scheduleCount
    *       PlaySchedules schedules
    */
    ViewerThread *thread = new ViewerThread;
    thread->SetViewerEvent(VIEWER_EVENT::Event::FIXED_PLAY_DATA_SCHEDULES);
    thread->SetRecvBytes(src);

    m_threadPool->start(thread);
}

//========================================================
void ViewerEventState::RecvRequestScreenCapture(const QByteArray& src)
//========================================================
{
    /**
    * @note
    *       ELGO_CONTROL -> ELGO_VIEWER
    *       current Screen capture on elgo_viewer
    *       if capture is successed, response to elgo_control
    * @param
    *       QString payload.src
    *       QString payload.dest
    */
    ViewerThread *thread = new ViewerThread;
    thread->SetViewerEvent(VIEWER_EVENT::Event::REQUEST_SCREEN_CAPTURE);
    thread->SetRecvBytes(src);

    m_threadPool->start(thread);
}
