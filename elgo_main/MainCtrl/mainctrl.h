#ifndef MAINCTRL_H
#define MAINCTRL_H

#include "Definition/DeviceDef.h"

class MainCtrl
{
public:
    explicit MainCtrl();
    virtual ~MainCtrl();

public:
    /** brief */
    void LoadCurrentDeviceInfo();
    /** brief */
    const DEVICE::Info& GetDeviceInfo();
    /** brief */
    QString MakeProcessPath(::ELGO_PROC::Proc proc);

private:
    DEVICE::Info m_deviceInfo;
    bool m_procStatus[2]; // 0 : elgo_cotrol, 1 : elgo_viewer
};

#endif // MAINCTRL_H
