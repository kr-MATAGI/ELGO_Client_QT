#ifndef VIEWERCONTROLLER_H
#define VIEWERCONTROLLER_H

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
