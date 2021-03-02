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
    *   @note
    *       VIEWER_EVENT::MAKE_QRCODE
    *   @param
    *       NONE
    */
    QDataStream out(&src, QIODevice::ReadOnly);
    out.setVersion(QDataStream::Qt_5_12);

    ViewerThread *thread = new ViewerThread;
    thread->SetViewerEvent(VIEWER_EVENT::Event::MAKE_QRCODE);
    thread->SetRecvBytes(src);
    m_threadPool->start(thread);
}
