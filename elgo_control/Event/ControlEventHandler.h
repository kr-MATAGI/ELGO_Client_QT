#ifndef EVENTHANDLER_H
#define EVENTHANDLER_H

#include "LocalSocketEvent/LocalSocketServer.h"

class EventHandler : public LocalSocketServer
{
public:
    EventHandler(ELGO_PROC::Proc proc, QObject *parent = nullptr);
    virtual ~EventHandler();

private:
    QList<quint16> m_enrolledEventList;
};

#endif // EVENTHANDLER_H
