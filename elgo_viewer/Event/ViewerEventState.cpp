// QT
#include <QDataStream>

// Common
#include "Common/Interface/ContentsPlayDataImpl.h"
#include "LocalSocketEvent/EFCEvent.h"

// Viwer
#include "ViewerEventState.h"
#include "ViewerThread/ViewerThread.h"
#include "Logger/ViewerLogger.h"
#include "Definition/ViewerDefinition.h"

//========================================================
ViewerEventState::ViewerEventState()
//========================================================
{
    m_threadPool = new QThreadPool;
    m_threadPool->setMaxThreadCount(MAX_THREAD_COUNT);

    // enorll Event
    m_state.RegisterEvent(VIEWER_EVENT::Event::MAKE_QRCODE,
                          &ViewerEventState::RecvMakeQrCodeAndDisplay);

    m_state.RegisterEvent(VIEWER_EVENT::Event::VIEWER_ROTATE_SCREEN,
                          &ViewerEventState::RecvRotateDeviceDisplay);

    m_state.RegisterEvent(VIEWER_EVENT::Event::PLAY_CUSTOM_PLAY_DATA,
                          &ViewerEventState::RecvPlayCustomPlayData);
    m_state.RegisterEvent(VIEWER_EVENT::Event::PLAY_FIXED_PLAY_DATA,
                          &ViewerEventState::RecvPlayFixedPlayData);

    m_state.RegisterEvent(VIEWER_EVENT::Event::ADD_CUSTOM_PLAY_DATA_LIST,
                          &ViewerEventState::RecvAddCustomPlayDataList);
    m_state.RegisterEvent(VIEWER_EVENT::Event::ADD_FIXED_PLAY_DATA_LIST,
                          &ViewerEventState::RecvAddFixedPlayDataList);

    m_state.RegisterEvent(VIEWER_EVENT::Event::REQUEST_SCREEN_CAPTURE,
                          &ViewerEventState::RecvRequestScreenCapture);

    m_state.RegisterEvent(VIEWER_EVENT::Event::UPDATE_PLAYER_PAUSE_STATUS,
                          &ViewerEventState::RecvUpdateContentsPlayerPause);
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

    QByteArray copyBytes = src;
    QDataStream copySteam(&copyBytes, QIODevice::ReadOnly);
    VIEWER_DEF::HEADING heading = VIEWER_DEF::HEADING::NONE_HEADING;
    quint8 headingValue = 0;
    copySteam >> headingValue;
    heading = static_cast<VIEWER_DEF::HEADING>(headingValue);
    ELGO_VIEWER_LOG("Rotation Heading: %d", heading);

    // emit signal to ContentsPlayer
    emit ContentsPlayer::GetInstance()->RotateScreenSignal(heading);
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
void ViewerEventState::RecvAddCustomPlayDataList(const QByteArray& src)
//========================================================
{
    /**
     * @note
     *       ELGO_MAIN -> ELGO_VIEWER
     *       ADD Custom PlayData after system booting
     * @param
     *       QVector<PlayJson::CustomPlayDataJson>  customPlayDataList
     */

    QByteArray copyBytes = src;
    QDataStream copySteam(&copyBytes, QIODevice::ReadOnly);

    QVector<PlayJson::CustomPlayDataJson> customDataList;
    copySteam >> customDataList;

    const int customDataListSize = customDataList.size();
    for(int idx = 0; idx < customDataListSize; idx++)
    {
        emit ContentsPlayer::GetInstance()->AddPlayDataSignal(customDataList[idx]);
    }
}

//========================================================
void ViewerEventState::RecvAddFixedPlayDataList(const QByteArray& src)
//========================================================
{
    /**
     * @note
     *       ELGO_MAIN -> ELGO_VIEWER
     *       ADD Fixed PlayData after system booting
     * @param
     *       QVector<PlayJson::FixedPlayDataJson>  fixedPlayDataList
     */

    QByteArray copyBytes = src;
    QDataStream copySteam(&copyBytes, QIODevice::ReadOnly);

    QVector<PlayJson::FixedPlayDataJson> fixedDataList;
    copySteam >> fixedDataList;

    const int fixedDataListSize = fixedDataList.size();
    for(int idx = 0; idx < fixedDataListSize; idx++)
    {
        emit ContentsPlayer::GetInstance()->AddPlayDataSignal(fixedDataList[idx]);
    }
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

//========================================================
void ViewerEventState::RecvUpdateContentsPlayerPause(const QByteArray& src)
//========================================================
{
    /**
     * @note
     *       ELGO_MAIN -> ELGO_VIEWER
     *       Contents Player item pause or play
     * @param
     *       bool isPause
     */

    QByteArray copyBytes = src;
    QDataStream copyStream(&copyBytes, QIODevice::ReadOnly);
    bool bIsPause = false;
    copyStream >> bIsPause;

    emit ContentsPlayer::GetInstance()->UpdatePlayerPauseSignal(bIsPause);
}
