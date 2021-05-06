#ifndef DEVICEMANAGER_H
#define DEVICEMANAGER_H

// QT
#include <QString>

// Main
#include "Definition/DeviceDef.h"

namespace DeviceManager
{
    // Display
    void UpdateSleepStatus(const DEVICE::OS os, bool isSleep);


}

#endif // DEVICEMANAGER_H
