#ifndef EVENTHANDLER_H
#define EVENTHANDLER_H

// EFC
#include "LocalSocketEvent/LocalSocketServer.h"

// Control
#include "ControlEventState.h"

class ControlEventHandler : public LocalSocketServer
{
public:
    ControlEventHandler(ELGO_SYS::Proc proc, QObject *parent = nullptr);
    virtual ~ControlEventHandler();

private slots:
    void ReadEventSlot() override;

private:
    ControlEventState m_state;
};

#endif // EVENTHANDLER_H
