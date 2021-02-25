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
    void ExecState(VIEWER_EVENT::Event event, QByteArray &src);

    /** @brief */
    void MakeQrCodeState(QByteArray& src);

public:
    QThreadPool *m_threadPool;
    EventState<ViewerEventState> m_state;
};


#endif // VIEWEREVENTSTATE_H
