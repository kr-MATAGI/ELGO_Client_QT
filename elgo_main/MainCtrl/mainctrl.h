#ifndef MAINCTRL_H
#define MAINCTRL_H

#include "Common/Deifinition.h"
#include "Definition/DeviceDef.h"
#include "DB/MainDBCtrl.h"

class MainCtrl
{
public:
    explicit MainCtrl();
    virtual ~MainCtrl();

public:
    /** brief */
    void LoadCurrentDeviceInfo();

    /** brief */
    QString MakeProcessPath(::ELGO_PROC::Proc proc);   

public:
    /** brief */
    const DEVICE::Info& GetDeviceInfo();

public:
    /** brief */
    MainDBCtrl& GetMainDBCtrl();

private:
    DEVICE::Info m_deviceInfo;
    bool m_procStatus[2]; // 0 : elgo_cotrol, 1 : elgo_viewer

    MainDBCtrl *m_dbCtrl;
};

#endif // MAINCTRL_H
