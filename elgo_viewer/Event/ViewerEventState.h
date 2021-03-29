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
    void ExecState(quint16 event, QByteArray &src);

    /** @brief */
    void MakeQrCodeAndDisplay(QByteArray& src);

    /** @brief */
    void RotateDeviceDisplay(QByteArray& src);

    /** @brief */
    void RecvCustomPlayData(QByteArray& src);

    /** @brief */
    void RecvFixedPlayData(QByteArray& src);

public:
    QThreadPool *m_threadPool;
    EventState<ViewerEventState> m_state;
};


#endif // VIEWEREVENTSTATE_H
