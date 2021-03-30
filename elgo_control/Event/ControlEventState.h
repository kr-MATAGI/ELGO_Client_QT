#ifndef CONTROLEVENTSTATE_H
#define CONTROLEVENTSTATE_H

#define MAX_THREAD_COUNT    4

// QT
#include <QByteArray>
#include <QThreadPool>

// EFC
#include "Common/EventList.h"
#include "LocalSocketEvent/EventState.h"

// Control
#include "NetworkCtrl/NetworkController.h"

class NetworkController;

class ControlEventState
{
public:
    explicit ControlEventState();
    ~ControlEventState();

public:
    /** @brief */
    void ExecState(const quint16 event, const QByteArray &src);

    /** @brief */
    void RecvServerInfoFromMain(const QByteArray& src);

    /** @brief */
    void RecvUpdateDisplaySleepStatus(const QByteArray& src);

    /** @brief */
    void RecvResponseScreenCapture(const QByteArray& src);

private:
    QThreadPool *m_threadPool;
    EventState<ControlEventState> m_state;
};

#endif // CONTROLEVENTSTATE_H
