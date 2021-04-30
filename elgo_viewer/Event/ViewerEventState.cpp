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
                          &ViewerEventState::RecvMakeQrCodeAndDisplay);
    m_state.RegisterEvent(VIEWER_EVENT::Event::ROTATE_DISPLAY,
                          &ViewerEventState::RecvRotateDeviceDisplay);
    m_state.RegisterEvent(VIEWER_EVENT::Event::PLAY_CUSTOM_PLAY_DATA,
                          &ViewerEventState::RecvPlayCustomPlayData);
    m_state.RegisterEvent(VIEWER_EVENT::Event::PLAY_FIXED_PLAY_DATA,
                          &ViewerEventState::RecvPlayFixedPlayData);
    m_state.RegisterEvent(VIEWER_EVENT::Event::ADD_CUSTOM_PLAY_DATA,
                          &ViewerEventState::RecvAddCustomPlayData);
    m_state.RegisterEvent(VIEWER_EVENT::Event::ADD_FIXED_PLAY_DATA,
                          &ViewerEventState::RecvAddFixedPlayData);
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
void ViewerEventState::RecvMakeQrCodeAndDisplay(const QByteArray &src)
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
void ViewerEventState::RecvRotateDeviceDisplay(const QByteArray& src)
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
void ViewerEventState::RecvPlayCustomPlayData(const QByteArray& src)
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
    thread->SetViewerEvent(VIEWER_EVENT::Event::PLAY_CUSTOM_PLAY_DATA);
    thread->SetRecvBytes(src);

    m_threadPool->start(thread);
}

//========================================================
void ViewerEventState::RecvPlayFixedPlayData(const QByteArray& src)
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
    thread->SetViewerEvent(VIEWER_EVENT::Event::PLAY_FIXED_PLAY_DATA);
    thread->SetRecvBytes(src);

    m_threadPool->start(thread);
}

//========================================================
void ViewerEventState::RecvAddCustomPlayData(const QByteArray& src)
//========================================================
{
    /**
     * @note
     *       ELGO_CONTROL -> ELGO_VIEWER
     *       Add custom play data
     * @param
     *       CustomPlayDataJson customPlayData
     */

    ViewerThread *thread = new ViewerThread;
    thread->SetViewerEvent(VIEWER_EVENT::Event::ADD_CUSTOM_PLAY_DATA);
    thread->SetRecvBytes(src);

    m_threadPool->start(thread);
}

//========================================================
void ViewerEventState::RecvAddFixedPlayData(const QByteArray& src)
//========================================================
{
    /**
     * @note
     *       ELGO_CONTROL -> ELGO_VIEWER
     *       Add fixed play data
     * @param
     *       FixedPlayDataJson fixedPlayData
     */

    ViewerThread *thread = new ViewerThread;
    thread->SetViewerEvent(VIEWER_EVENT::Event::ADD_FIXED_PLAY_DATA);
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
