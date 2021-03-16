#ifndef JSONPARSER_H
#define JSONPARSER_H

// QT
#include <QString>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QString>

// Control
#include "Schema/RemoteSchema.h"
#include "Schema/ContentServerSchema.h"
#include "DownloadThread/DownloadDef.h"

namespace JsonParser
{
    /** @brief */
    QString LatestVersionParse(const QString &src);

    /** @brief */
    void ParseGetJwtResponse(const QString& src, QString& dest);

    /** @brief */
    void WriteGetJwtRequest(const QString& udid, const QString& os, std::string& dest);

    /// elgo_remote -> elgo_control
    // Parse
    /** @brief */
    Remote::Action PaseRemoteActionText(const QString &src);

    /** @brief */
    bool ParseRemoteDeviceLogin(const QString &src, Remote::DeviceLogin& dest);

    /** @brief */
    bool ParseRemoteManageDevice(const QString &src, Remote::ManageDevice& dest);

    /** @brief */
    bool ParseRemoteRotateDevice(const QString &src, Remote::RotateDisplay& dest);

    /** @brief */
    bool ParseRemoteDeviceOptions(const QString& src, Remote::DeviceOptions& dest);

    // Write
    /** @brief */
    void MakeDateTimeString(QString& dest);

    /** @brief */
    void WriteDeviceLoginResponse(const Remote::Result::Contents& results, QString& dest);

    /** @brief */
    void WriteManageDeviceResponse(const Remote::Result::Contents& results, QString& dest);

    /** @brief */
    void WriteRotateDisplayResponse(const Remote::Result::Contents& results, QString& dest);

    /** @brief */
    void WriteDeviceOptionsResponse(const Remote::Result::Contents& results, QString& dest);


    /// Content Server -> elgo_control
    // Parse
    /** @brief */
    bool ParseContentServerJsonResponse(const QString& src, ContentSchema::Summary& dest);
    /** @brief */
    bool ParsePayloadResponse(const QJsonObject& payloadObj, ContentSchema::Payload& dest);


    // Write
    /** @brief */
    void WriteContentServerAccessRequest(const ContentSchema::Summary& src, QString& dest);
    /** @brief */
    void WriteContentServerPayloadRequest(const ContentSchema::Payload& src, QJsonObject& dest);

    /// Weather Info Parser
    // Parse
    /** @brief */
    bool ParseWeatherInfoJsonResponse(const QString& src, DownloadDef::Weather::Response& dest);
    /** @brief */
    bool ParseWeatherItemsJsonResponse(const QJsonObject& itemsObj, DownloadDef::Weather::Response& dest);

    // etc
    /** @brief */
    void GetBeautifyUDID(const QString& src, QString& dest);
    /** @brief */
    void GetWeatherRequestBaseDateTime(QString& baseDate, QString& baseTime);
}

#endif // JSONPARSER_H
