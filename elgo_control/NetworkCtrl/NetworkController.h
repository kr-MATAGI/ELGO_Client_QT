#ifndef NETWORKCONTROLLER_H
#define NETWORKCONTROLLER_H

//EFC
#include "ShardMem/ShmCtrl.h"

// Control
#include "NetworkCtrl.h"
#include "TCP/RemoteTCPServer.h"

class NetworkCtrl;
class RemoteTCPServer;

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

private:
    NetworkCtrl *m_netCtrl;
};

#endif // NETWORKCONTROLLER_H
