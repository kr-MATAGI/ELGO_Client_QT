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

#include "ViewerThread/ViewerThread.h"

class ViewerEventState
{
public:
    ViewerEventState();
    ~ViewerEventState();

public:
    /** @brief */
    void ExecState(const quint16 event, const QByteArray &src);

    /** @brief */
    void MakeQrCodeAndDisplay(const QByteArray& src);

    /** @brief */
    void RotateDeviceDisplay(const QByteArray& src);

    /** @brief */
    void RecvCustomPlayData(const QByteArray& src);
    /** @brief */
    void RecvFixedPlayData(const QByteArray& src);

    /** @brief */
    void RecvCustomPlaySchedules(const QByteArray& src);
    /** @brief */
    void RecvFixedPlaySchedules(const QByteArray& src);

    /** @brief */
    void RecvRequestScreenCapture(const QByteArray& src);

public:
    QThreadPool *m_threadPool;
    EventState<ViewerEventState> m_state;
};


#endif // VIEWEREVENTSTATE_H
