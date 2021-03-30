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
    void ExecState(const quint16 event, const QByteArray &src);

    /** @brief */
    void RecvProcecssReady(const QByteArray& src);

    /** @brief */
    void RecvChangeDeviceOptions(const QByteArray& src);

private:
    QThreadPool *m_threadPool;
    EventState<MainEventState> m_state;
};

#endif // MAINEVENTSTATE_H
