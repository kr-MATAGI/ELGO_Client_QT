#ifndef JSONWRITER_H
#define JSONWRITER_H

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

namespace JsonWriter
{
    /// elgo_control <-> Content server
    /** @brief */
    void WriteGetJwtRequest(const QString& udid, const QString& os, std::string& dest);
    /** @brief */
    void WriteContentServerAccessRequest(const ContentSchema::Summary& src, QString& dest);
    /** @brief */
    void WriteContentServerPayloadRequest(const ContentSchema::Payload& src, QJsonObject& dest);

    /// elgo_remote <-> elgo_control
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

    /// ETC Utils
    /** @brief */
    void GetBeautifyUDID(const QString& src, QString& dest);
    /** @brief */
    void GetWeatherRequestBaseDateTime(QString& baseDate, QString& baseTime);
}

#endif // JSONWRITER_H