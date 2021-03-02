#include "NetworkController.h"

NetworkController* NetworkController::pInstance = nullptr;

//========================================================
NetworkController::NetworkController()
//========================================================
{
    m_netCtrl = new NetworkCtrl;
}

//========================================================
NetworkController::~NetworkController()
//========================================================
{
    delete m_netCtrl;
    m_netCtrl = NULL;
}

//========================================================
NetworkController* NetworkController::GetInstance()
//========================================================
{
    if(nullptr == pInstance)
    {
        pInstance = new NetworkController();
    }
    return pInstance;
}

//========================================================
void NetworkController::DestoryInstance()
//========================================================
{
    if(nullptr != pInstance)
    {
        return;
    }
    delete pInstance;
    pInstance = NULL;
}

//========================================================
NetworkCtrl& NetworkController::GetNetworkCtrl()
//========================================================
{
    return *m_netCtrl;
}
