#include "ViewerController.h"

ViewerController* ViewerController::pInstance = nullptr;

//========================================================
ViewerController::ViewerController()
//========================================================
{
    m_viewerCtrl = new ViewerCtrl();
}

//========================================================
ViewerController::~ViewerController()
//========================================================
{
    delete m_viewerCtrl;
    m_viewerCtrl = NULL;
}

//========================================================
ViewerController* ViewerController::GetInstance()
//========================================================
{
    if(nullptr == pInstance)
    {
        pInstance = new ViewerController();
    }
    return pInstance;
}

//========================================================
void ViewerController::DestoryInstance()
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
ViewerCtrl& ViewerController::GetViewerCtrl()
//========================================================
{
    return *m_viewerCtrl;
}
