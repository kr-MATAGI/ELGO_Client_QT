#ifndef JSONPARSER_H
#define JSONPARSER_H

// QT
#include <QString>

// Control
#include "Schema/RemoteSchema.h"

namespace JsonParser
{
    /** @brief */
    QString LatestVersionParse(const QString &src);

    // elgo_remote -> elgo_control
    // Parse
    /** @brief */
    Remote::Action PaseRemoteContorlActionText(const QString &src);

    /** @brief */
    bool ParseRemoteControlDeviceLogin(const QString &src, Remote::DeviceLogin& dest);

    /** @brief */
    bool ParseRemoteControlManageDevice(const QString &src, Remote::ManageDevice& dest);

    /** @brief */
    bool ParseRemoteControlRotateDevice(const QString &src, Remote::RotateDisplay& dest);

    // Write
    /** @brief */
    void MakeDateTimeString(QString& dest);

    /** @brief */
    void WriteDeviceLoginResponse(const Remote::Result::Contents& results, QString& dest);

    /** @brief */
    void WriteManageDeviceResponse(const Remote::Result::Contents& results, QString& dest);

    /** @brief */
    void WriteRotateDisplayResponse(const Remote::Result::Contents& results, QString& dest);
}

#endif // JSONPARSER_H
