#ifndef NETWORKCONTROLLER_H
#define NETWORKCONTROLLER_H

// Control
#include "NetworkCtrl.h"
#include "DB/NetworkDBCtrl.h"
#include "RemoteControl/RemoteControlServer.h"
#include "Timer/UpdateWidgetTimer.h"

class NetworkCtrl;
class NetworkDBCtrl;
class RemoteControlServer;
class UpdateWidgetTimer;

class NetworkController
{
private:
    static NetworkController *pInstance;

    NetworkController();
    ~NetworkController();

public:
    /** @brief */
    static NetworkController* GetInstance();

    /** @brief */
    void DestoryInstance();

    /** @brief */
    NetworkCtrl& GetNetworkCtrl();

    /** @brief */
    NetworkDBCtrl& GetDBCtrl();

    /** @brief */
    UpdateWidgetTimer& GetUpdateWidgetTimer();

private:
    NetworkCtrl *m_netCtrl;
    NetworkDBCtrl *m_dbCtrl;

    UpdateWidgetTimer *m_updateTimer;
};

#endif // NETWORKCONTROLLER_H
