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
    void LoadConfigurationInfo();

    /** @brief */
    const DEVICE::Info& GetDeviceInfo();

    /** @brief */
    const DEVICE::INIT_CONFIG& GetInitConfig();

    /** @brief */
    void SetDisplaySleepStatus(const bool newValue);

    /** @brief */
    bool GetDisplaySleepStatus();

private:
    DEVICE::Info m_deviceInfo;
    DEVICE::INIT_CONFIG m_initConfig;

    bool m_bDisplaySleep;
};

#endif // MAINCTRL_H
