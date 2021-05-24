#ifndef EVENTHANDLER_H
#define EVENTHANDLER_H

// EFC
#include "LocalSocketEvent/LocalSocketServer.h"

// Control
#include "ControlEventState.h"

class ControlEventHandler : public LocalSocketServer
{
public:
    /** @brief */
    ControlEventHandler(ELGO_SYS::Proc proc, QObject *parent = nullptr);
    /** @brief */
    virtual ~ControlEventHandler();

private slots:
    /** @brief */
    void ReadEventSlot() override;

private:
    ControlEventState m_state;
};

#endif // EVENTHANDLER_H
