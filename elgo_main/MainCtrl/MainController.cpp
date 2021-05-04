#include "MainController.h"

MainController* MainController::pInstance = nullptr;

//========================================================
MainController::MainController()
//========================================================
{
    m_mainCtrl = new MainCtrl();
    m_dbCtrl = new MainDBCtrl();
    m_playTimer = new PlayScheduleTimer();
    m_powerTimer = new PowerScheduleTimer();
}

//========================================================
MainController::~MainController()
//========================================================
{
    delete m_mainCtrl;
    m_mainCtrl = NULL;

    delete m_dbCtrl;
    m_dbCtrl = NULL;

    delete m_playTimer;
    m_playTimer = NULL;

    delete m_powerTimer;
    m_powerTimer = NULL;
}

//========================================================
MainController* MainController::GetInstance()
//========================================================
{
    if(nullptr == pInstance)
    {
        pInstance = new MainController();
    }
    return pInstance;
}

//========================================================
void MainController::DestoryInstance()
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
MainCtrl& MainController::GetMainCtrl()
//========================================================
{
    return *m_mainCtrl;
}

//========================================================
MainDBCtrl& MainController::GetDBCtrl()
//========================================================
{
    return *m_dbCtrl;
}

//========================================================
PlayScheduleTimer& MainController::GetPlayTimer()
//========================================================
{
    return *m_playTimer;
}

//========================================================
PowerScheduleTimer& MainController::GetPowerTimer()
//========================================================
{
    return *m_powerTimer;
}
