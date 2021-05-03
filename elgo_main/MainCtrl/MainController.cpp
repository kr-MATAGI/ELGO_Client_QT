#include "MainController.h"

MainController* MainController::pInstance = nullptr;

//========================================================
MainController::MainController()
//========================================================
{
    m_mainCtrl = new MainCtrl();
    m_dbCtrl = new MainDBCtrl();
    m_scheduleTimer = new PlayScheduleTimer();
}

//========================================================
MainController::~MainController()
//========================================================
{
    delete m_mainCtrl;
    m_mainCtrl = NULL;

    delete m_dbCtrl;
    m_dbCtrl = NULL;

    delete m_scheduleTimer;
    m_scheduleTimer = NULL;
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
PlayScheduleTimer& MainController::GetScheduleTimer()
//========================================================
{
    return *m_scheduleTimer;
}
