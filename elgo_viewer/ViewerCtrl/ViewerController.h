#ifndef VIEWERCONTROLLER_H
#define VIEWERCONTROLLER_H

// EFC
#include "ShardMem/ShmCtrl.h"

// Viewer
#include "ViewerCtrl.h"

class ViewerCtrl;

class ViewerController
{
private:
    static ViewerController *pInstance;

    ViewerController();
    ~ViewerController();

public:
    /** @brief */
    static ViewerController* GetInstance();

    /** @brief */
    void DestoryInstance();

    /** @brief */
    ViewerCtrl& GetViewerCtrl();

private:
    ViewerCtrl *m_viewerCtrl;
};

#endif // VIEWERCONTROLLER_H
