#ifndef EVENTHANDLER_H
#define EVENTHANDLER_H

// EFC
#include "LocalSocketEvent/LocalSocketServer.h"

// Main
#include "MainEventState.h"

class MainEventHandler : public LocalSocketServer
{
public:
    MainEventHandler(ELGO_PROC::Proc name, QObject *parent = nullptr);
    virtual ~MainEventHandler();

private:
    void readEvent() override;

private:
    MainEventState m_state;
};

#endif // EVENTHANDLER_H
