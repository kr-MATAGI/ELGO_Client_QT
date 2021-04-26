#ifndef WIFIMANAGER_H
#define WIFIMANAGER_H

// QT
#include <QString>

// Main
#include "Definition/DeviceDef.h"
#include "Definition/WifiDef.h"

namespace WifiManager
{
    // Searching
    /** @brief */
    void GetWlanInterfaceName(const DEVICE::OS os, QString& dest);
    /** @brief */
    void GetAcessibleWifiList(const DEVICE::OS os, const QString& wlanName, QVector<WifiInfo>& dest);

    // Connect
    /** @brief */
    bool ConnectNewWirelessInternet(const DEVICE::OS os, const QString& wlanName, const QString& ssid,
                                    const QString& password, const bool enc);

    // Wake up
    /** @brief */
    void WakeUpWirelessInterface(const DEVICE::OS os, const QString& wlanName);

    /** @brief */
    void ParsingLinuxString(const QString& src, QVector<WifiInfo>& dest);

    // Converter
    /** @brief */
    void ConvertUtf8ToKR(const QString&src, QString& dest);

    // hosts
    /** @brief */
    void UpdateRemoteServerHost(const DEVICE::OS os, const QString& ip);
    /** @brief */
    void ChangeChmodReadRightRight(const DEVICE::OS os, const QString& path);

    // IP
    /** @brief */
    void GetDeviceLocalIP(QString& dest);
}

#endif // WIFIMANAGER_H
