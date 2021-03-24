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


    /// elgo_remote <-> elgo_control
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


    /// Content Server <-> elgo_control
    /** @brief */
    void ParseGetJwtResponse(const QString& src, QString& dest);
    /** @brief */
    bool ParseContentServerJsonResponse(const QString& src, ContentSchema::Summary& dest);
    /** @brief */
    bool ParsePayloadResponse(const QJsonObject& payloadObj, ContentSchema::Payload& dest);
    /** @brief */
    bool ParseSchedulesResponse(const QString& src, QList<ContentSchema::Schedules>& dest);


    /// Weather Info Parser
    /** @brief */
    bool ParseWeatherInfoJsonResponse(const QString& src, DownloadDef::Weather::Response& dest);
    /** @brief */
    bool ParseWeatherItemsJsonResponse(const QJsonObject& itemsObj, DownloadDef::Weather::Response& dest);    
}

#endif // JSONPARSER_H
