#ifndef MAINCTRL_H
#define MAINCTRL_H

//EFC
#include "Common/Deifinition.h"
#include "ShardMem/ShmCtrl.h"

#include "Definition/DeviceDef.h"
#include "DB/MainDBCtrl.h"

class MainCtrl
{
public:
    explicit MainCtrl();
    virtual ~MainCtrl();

public:
    /** @brief */
    void LoadCurrentDeviceInfo();

    /** @brief */
    QString MakeProcessPath(::ELGO_PROC::Proc proc);   

    /** @brief */
    bool CheckingWirelessInternet();

    /** @brief */
    const DEVICE::Info& GetDeviceInfo();

public:
    MainDBCtrl& GetDBCtrl();

private:
    DEVICE::Info m_deviceInfo;

    MainDBCtrl *m_dbCtrl;
    ShmCtrl *m_shmCtrl;
};

#endif // MAINCTRL_H
