#ifndef VIEWEREVENTSTATE_H
#define VIEWEREVENTSTATE_H

#define MAX_THREAD_COUNT 4

// QT
#include <QThreadPool>
#include <QByteArray>
#include <QMap>

// EFC
#include "Common/EventList.h"
#include "LocalSocketEvent/EventState.h"

// Viewer
#include "ViewerThread/ViewerThread.h"
#include "Widget/ContentsPlayer.h"

class ContentsPlayer;

class ViewerEventState
{
public:
    ViewerEventState();
    ~ViewerEventState();

public:
    /** @brief */
    void ExecState(const quint16 event, const QByteArray &src);

    /** @brief */
    void RecvMakeQrCodeAndDisplay(const QByteArray& src);

    /** @brief */
    void RecvRotateDeviceDisplay(const QByteArray& src);

    /** @brief */
    void RecvPlayCustomPlayData(const QByteArray& src);
    /** @brief */
    void RecvPlayFixedPlayData(const QByteArray& src);

    /** @brief */
    void RecvAddCustomPlayDataList(const QByteArray& src);
    /** @brief */
    void RecvAddFixedPlayDataList(const QByteArray& src);

    /** @brief */
    void RecvRequestScreenCapture(const QByteArray& src);

    /** @brief */
    void RecvUpdateContentsPlayerPause(const QByteArray& src);

public:
    QThreadPool *m_threadPool;
    EventState<ViewerEventState> m_state;
};


#endif // VIEWEREVENTSTATE_H
