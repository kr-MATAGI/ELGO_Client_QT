#ifndef MAINEVENTSTATE_H
#define MAINEVENTSTATE_H

#define MAX_THREAD_COUNT 4

// QT
#include <QByteArray>
#include <QThreadPool>

// EFC
#include "Common/EventList.h"
#include "LocalSocketEvent/EventState.h"

class MainEventState
{
public:
    MainEventState();
    ~MainEventState();

public:
    /** @brief */
    void ExecState(quint16 event, QByteArray &src);

    /** @brief */
    void RecvProcecssReady(QByteArray& src);

    /** @brief */
    void RecvChangeDeviceOptions(QByteArray& src);

private:
    QThreadPool *m_threadPool;
    EventState<MainEventState> m_state;
};

#endif // MAINEVENTSTATE_H
