#ifndef MAINCTRL_H
#define MAINCTRL_H

//EFC
#include "Common/Deifinition.h"
#include "ShardMem/ShmCtrl.h"

// Main
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
    QString MakeProcessPath(::ELGO_SYS::Proc proc);   

    /** @brief */
    void CheckResourceFolder();

    /** @brief */
    void LoadConfigurationInfo();

    /** @brief */
    const DEVICE::Info& GetDeviceInfo();
    /** @brief */
    void SetDeviceIP(const QString& src);

    /** @brief */
    const DEVICE::INIT_CONFIG& GetInitConfig();

    /** @brief */
    void CheckDisplaySleepStatus();
    /** @brief */
    void SetDisplaySleepStatus(const bool newValue);
    /** @brief */
    bool GetDisplaySleepStatus();

    /** @brief */
    void SetDeviceWlanName(const QString& src);
    /** @brief */
    const QString& GetDeviceWlanName();

private:
    DEVICE::Info m_deviceInfo;
    DEVICE::INIT_CONFIG m_initConfig;
    QString m_wlanName;

    bool m_bDisplaySleep;
};

#endif // MAINCTRL_H
