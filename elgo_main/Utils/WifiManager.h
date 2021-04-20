#ifndef WIFIMANAGER_H
#define WIFIMANAGER_H

// QT
#include <QString>

// Main
#include "Definition/DeviceDef.h"
#include "Definition/WifiDef.h"

namespace WifiManager
{
    /** @brief */
    void GetWlanInterfaceName(const DEVICE::OS os, QString& dest);

    /** @brief */
    void GetAcessibleWifiList(const DEVICE::OS os, const QString& wlanName, QVector<WifiInfo>& dest);

    /** @brief */
    void ParsingLinuxString(const QString& src, QVector<WifiInfo>& dest);
}

#endif // WIFIMANAGER_H
