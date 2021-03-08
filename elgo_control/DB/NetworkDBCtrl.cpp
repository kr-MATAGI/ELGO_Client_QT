#include "NetworkDBCtrl.h"

//========================================================
NetworkDBCtrl::NetworkDBCtrl()
//========================================================
{
    m_mutex = new QMutex;
}

//========================================================
NetworkDBCtrl::~NetworkDBCtrl()
//========================================================
{
    delete m_mutex;
    m_mutex = NULL;
}

//========================================================
bool NetworkDBCtrl::CheckDeviceLogingInfo(Remote::DeviceJson& deviceInfo)
//========================================================
{
    bool retValue = true;


    return retValue;
}
