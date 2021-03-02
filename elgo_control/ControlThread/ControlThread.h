#ifndef CONTROLTHREAD_H
#define CONTROLTHREAD_H

// QT
#include <QRunnable>
#include <QByteArray>

// EFC
#include "Common/EventList.h"


class ControlThread : public QRunnable
{
public:
    ControlThread();
    ~ControlThread();

public:
    /** @brief  QRunnable run() override */
    void run() override;

    /** @brief */
    void ExecRecvServerInfoFromMain();

public:
    /** @brief */
    void SetControlEvent(const CONTROL_EVENT::Event event);

    /** @brief */
    void SetRecvBytes(const QByteArray &src);

private:
    CONTROL_EVENT::Event m_event;
    QByteArray m_bytes;

};

#endif // CONTROLTHREAD_H
