#ifndef VIEWERCTRL_H
#define VIEWERCTRL_H

// QT
#include <QString>

class ViewerCtrl
{
public:
    ViewerCtrl();
    ~ViewerCtrl();

public:
    /** @brief  notify process status */
    void SendViewerIsReady();

private:
};

#endif // VIEWERCTRL_H
