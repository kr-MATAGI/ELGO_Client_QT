#ifndef NETWORKCONTROLLER_H
#define NETWORKCONTROLLER_H

// Control
#include "NetworkCtrl.h"
#include "DB/NetworkDBCtrl.h"
#include "RemoteControl/RemoteControlServer.h"

class NetworkCtrl;
class NetworkDBCtrl;
class RemoteControlServer;

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

private:
    NetworkCtrl *m_netCtrl;
    NetworkDBCtrl *m_dbCtrl;
};

#endif // NETWORKCONTROLLER_H
