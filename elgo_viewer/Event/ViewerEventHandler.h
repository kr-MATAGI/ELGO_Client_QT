#ifndef EVENTHANDLER_H
#define EVENTHANDLER_H

#include "LocalSocketEvent/LocalSocketServer.h"
#include "ViewerEventState.h"

class ViewerEventHandler : public LocalSocketServer
{

public:
    /** @brief */
    ViewerEventHandler(ELGO_SYS::Proc name, QObject *parent = nullptr);
    /** @brief */
    virtual ~ViewerEventHandler();

private slots:
    /** @brief */
    void ReadEventSlot() override;

private:
    ViewerEventState m_state;
};

#endif // EVENTHANDLER_H
