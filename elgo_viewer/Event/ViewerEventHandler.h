#ifndef EVENTHANDLER_H
#define EVENTHANDLER_H

#include "LocalSocketEvent/LocalSocketServer.h"
#include "ViewerEventState.h"

class ViewerEventHandler : public LocalSocketServer
{

public:
    ViewerEventHandler(ELGO_PROC::Proc name, QObject *parent = nullptr);
    virtual ~ViewerEventHandler();

private slots:
    void readEvent() override;

private:
    ViewerEventState m_state;
};

#endif // EVENTHANDLER_H
