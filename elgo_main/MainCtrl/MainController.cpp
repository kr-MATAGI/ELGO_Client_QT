#include "MainController.h"

MainController* MainController::pInstance = nullptr;

//========================================================
MainController::MainController()
//========================================================
{
    m_mainCtrl = new MainCtrl();
    m_dbCtrl = new MainDBCtrl();
    m_shmCtrl = new ShmCtrl();
}

//========================================================
MainController::~MainController()
//========================================================
{
    delete m_mainCtrl;
    m_mainCtrl = NULL;

    delete m_dbCtrl;
    m_dbCtrl = NULL;

    delete m_shmCtrl;
    m_shmCtrl = NULL;
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
ShmCtrl& MainController::GetShmCtrl()
//========================================================
{
    return *m_shmCtrl;
}
