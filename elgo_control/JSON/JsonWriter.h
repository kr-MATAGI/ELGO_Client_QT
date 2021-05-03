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

    /** @brief  access Event */
    void WriteContentServerAccessEvent(const ContentSchema::Summary& src, QString& dest);
    /** @brief  rename Event */
    void WriteContentServerRenameEvent(const ContentSchema::Summary& src, QString& dest);
    /** @brief  singlePlay Event */
    void WriteContentServerSinglePlayEvent(const ContentSchema::Summary& src, QString& dest);
    /** @brief  playSchedule Event */
    void WriteContentServerPlayScheduleEvent(const ContentSchema::Summary& src, QString& dest);
    /** @brief  capture Event */
    void WriteContentServerScreenCaptureEvent(const ContentSchema::Summary& src, QString& dest);
    /** @brief  Display On/Off Event */
    void WriteContentServerDisplayOnOffEvent(const ContentSchema::Summary& src, QString& dest);
    /** @brief  Reboot Event */
    void WriteContentServerRebootEvent(const ContentSchema::Summary& src, QString& dest);
    /** @brief  ClearPlaySchedule Event */
    void WriteContentServerClearPlayScheduleEvent(const ContentSchema::Summary& src, QString& dest);
    /** @brief  Write Defulat Response */
    void WriteContentServerDefaultResponse(const ContentSchema::Summary& src, QString& dest);

    /** @brief */
    void WriteContentServerPayload(const ContentSchema::Summary& src, QJsonObject& dest,
                                   const bool bIsError = false, const QString& errorStr = QString());

    /** @brief */
    void WriteContentServerErrorResponse(const ContentSchema::Summary& src, QString& dest,
                                         const bool bIsError, const QString& errorStr);


    /// elgo_remote <-> elgo_control
    /** @brief */
    void MakeDateTimeString(QString& dest);

    /** @brief */
    void WriteDateActionResultResponse(const Remote::Action action,
                                       const Remote::Result::Contents& contents,
                                       QJsonObject& dest);

    /** @brief */
    void WriteDeviceLoginResponse(const Remote::Action action,
                                  const Remote::Result::Contents& contents,
                                  QString& dest);
    /** @brief */
    void WriteManageDeviceResponse(const Remote::Action action,
                                   const Remote::Result::Contents& contents,
                                   QString& dest);
    /** @brief */
    void WriteRotateDisplayResponse(const Remote::Action action,
                                    const Remote::Result::Contents& contents,
                                    QString& dest);
    /** @brief */
    void WriteDeviceOptionsResponse(const Remote::Action action,
                                    const Remote::Result::Contents& contents,
                                    QString& dest);
    /** @brief */
    void WriteUpdateWifiListResponse(const Remote::Action action,
                                     const Remote::Result::Contents& contents,
                                     QString& dest);

    /** @brief */
    void WriteConnectWifiResultResponse(const Remote::Action action,
                                        const Remote::Result::Contents& contents,
                                        QString& dest);

    /** @brief */
    void WriteRemoteUserLoginResponse(const Remote::Action action,
                                      const Remote::Result::Contents& contents,
                                      QString& dest);

    /// ETC Utils
    /** @brief */
    void GetBeautifyUDID(const QString& src, QString& dest);
    /** @brief */
    void GetWeatherRequestBaseDateTime(QString& baseDate, QString& baseTime);
}

#endif // JSONWRITER_H
