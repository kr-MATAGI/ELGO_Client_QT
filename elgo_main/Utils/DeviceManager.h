#ifndef DEVICEMANAGER_H
#define DEVICEMANAGER_H

// QT
#include <QString>

// Main
#include "Definition/DeviceDef.h"

namespace DeviceManager
{
    // Display
    /** @brief */
    void UpdateSleepStatus(const DEVICE::OS os, const bool isSleep);

    // Reboot
    /** @brief */
    void SystemReboot(const DEVICE::OS os);
}

#endif // DEVICEMANAGER_H
