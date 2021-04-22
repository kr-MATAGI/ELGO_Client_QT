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

// Common
#include "Common/Interface/ContentsPlayDataImpl.h"
#include "Common/Interface/ScheduleDef.h"

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
    /** @brief */
    bool ParseConnectWifi(const QString&src, Remote::ConnectWifi& dest);


    /// Content Server <-> elgo_control
    /** @brief */
    void ParseGetJwtResponse(const QString& src, QString& dest);
    /** @brief */
    bool ParseContentServerJsonResponse(const QString& src, ContentSchema::Summary& dest);
    /** @brief */
    bool ParsePayloadResponse(const QJsonObject& payloadObj, ContentSchema::Payload& dest);

    /** @brief  Schedules */
    bool ParseSchedulesResponse(const QString& src, QVector<ScheduleJson::PlaySchedule>& dest);

    /** @brief  Resource */
    void ParseResourceResponse(const QString& src, QVector<ResourceJson::Resource>& dest);


    /** @brief  Play Data */
    void ParsePlayDataJson(const QString& src, PlayJson::PlayData& dest);
    /** @brief  Custom Play Data */
    void ParseCustomPlayDataJson(const QString& src, PlayJson::CustomPlayDataJson& dest);
    /** @brief  Fixed Play Data */
    void ParseFixedPlayDataJson(const QString& src, PlayJson::FixedPlayDataJson& dest);
    /** @brief */
    void ParsePageDataJson(const QJsonArray& pageDataArr, QVector<PlayJson::PageData>& dest);
    /** @brief */
    void ParseCustomLayerDataJson(const QJsonArray& layerDataArr, QVector<PlayJson::CustomLayerData>& dest);
    /** @brief */
    void ParseFixedLayerDataJson(const QJsonArray& layerDataArr, QVector<PlayJson::FixedLayerData>& dest);
    /** @brief */
    void ParseCustomLayerContentJson(const QJsonObject& layerContentObj, PlayJson::ContentData& dest);
    /** @brief */
    void ParseFixedLayerContentJson(const QJsonObject& layerContentObj, PlayJson::ContentData& dest);
    /** @brief */
    void ParseSubtitleDataJson(const QJsonArray& subtitleDataArr, QVector<PlayJson::SubtitleData>& dest);


    /** @brief  Power Schedules */
    void ParsePowerSchedulesJson(const QString& src, ScheduleJson::PowerSchedules& dest);


    /** @brief  Single Play Schdules */
    void ParseSinglePlaySchedulesJson(const QString& src, ScheduleJson::SinglePlaySchedules& dest);


    /// Weather Info Parser
    /** @brief */
    bool ParseWeatherInfoJsonResponse(const QString& src, PlayJson::ContentData& dest);
    /** @brief */
    bool ParseWeatherItemsJsonResponse(const QJsonObject& itemsObj, PlayJson::ContentData& dest);
}

#endif // JSONPARSER_H
