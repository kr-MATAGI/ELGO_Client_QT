#ifndef EVENTHANDLER_H
#define EVENTHANDLER_H

#include "LocalSocketEvent/LocalSocketServer.h"

class ControlEventHandler : public LocalSocketServer
{
public:
    ControlEventHandler(ELGO_PROC::Proc proc, QObject *parent = nullptr);
    virtual ~ControlEventHandler();

private slots:
    void readEvent() override;

private:
    QList<quint16> m_enrolledEventList;
};

#endif // EVENTHANDLER_H
