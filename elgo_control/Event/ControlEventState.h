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
    void ExecState(quint16 event, QByteArray &src);

    /** @brief */
    void RecvServerInfoFromMain(QByteArray& src);

    /** @brief */
    void RecvUpdateDisplaySleepStatus(QByteArray& src);


private:
    QThreadPool *m_threadPool;
    EventState<ControlEventState> m_state;
};

#endif // CONTROLEVENTSTATE_H
