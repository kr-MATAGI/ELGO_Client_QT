// control
#include "NetworkController.h"

NetworkController* NetworkController::pInstance = nullptr;

//========================================================
NetworkController::NetworkController()
//========================================================
{
    m_netCtrl = new NetworkCtrl;
    m_dbCtrl = new NetworkDBCtrl;
    m_updateTimer = new UpdateWidgetTimer;
}

//========================================================
NetworkController::~NetworkController()
//========================================================
{
    delete m_netCtrl;
    m_netCtrl = NULL;

    delete m_dbCtrl;
    m_dbCtrl = NULL;

    delete m_updateTimer;
    m_updateTimer = NULL;
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

//========================================================
NetworkDBCtrl& NetworkController::GetDBCtrl()
//========================================================
{
    return *m_dbCtrl;
}

//========================================================
UpdateWidgetTimer& NetworkController::GetUpdateWidgetTimer()
//========================================================
{
    return *m_updateTimer;
}
