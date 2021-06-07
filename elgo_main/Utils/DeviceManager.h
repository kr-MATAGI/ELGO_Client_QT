#ifndef DEVICEMANAGER_H
#define DEVICEMANAGER_H

// QT
#include <QString>

// Main
#include "Definition/DeviceDef.h"

namespace DeviceManager
{
    /** @brief  Sleep */
    void UpdateSleepStatus(const DEVICE::OS os, const bool isSleep);

    /** @brief  Reboot */
    void SystemReboot(const DEVICE::OS os);

    /** @brief  Mute */
    void DeviceMute(const DEVICE::OS os, const bool bIsMute);

    /** @brief  Rotation */
    void RotateScreen(const DEVICE::OS os, quint8 heading);

    /** @brief  StartProcess */
    bool StartProcess(const QString& procPath);
}

#endif // DEVICEMANAGER_H
