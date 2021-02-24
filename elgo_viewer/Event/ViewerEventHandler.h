#ifndef EVENTHANDLER_H
#define EVENTHANDLER_H

#include <LocalSocketEvent/LocalSocketServer.h>

class EventHandler : public LocalSocketServer
{

public:
    EventHandler(ELGO_PROC::Proc name, QObject *parent = nullptr);
    virtual ~EventHandler();

private:


};

#endif // EVENTHANDLER_H
