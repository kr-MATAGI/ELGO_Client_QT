#ifndef JSONSTRINGCONVERTER_H
#define JSONSTRINGCONVERTER_H

// QT
#include <QString>

// Control
#include "Schema/ContentServerSchema.h"
#include "Schema/RemoteSchema.h"

// Common
#include "Common/Interface/ContentsPlayDataDef.h"
#include "Common/Interface/ScheduleJsonDef.h"

namespace JsonStringConverter
{
    /// string to enum

    // elgo_control <-> contents server
    /** @brief */
    ContentSchema::Event ContentServerEventStringToEnum(const QString& src);
    /** @brief */
    ContentSchema::PayloadType ContentServerPayloadTypeStringToEnum(const QString& src);

    /** @brief */
    ResourceJson::ResourceType ResourceTypeStringToEnum(const QString& src);

    /** @brief */
    PlayJson::PlayDataType PlayDataTypeStringToEnum(const QString& src);
    /** @brief */
    PlayJson::Orientation OrientationTypeStringToEnum(const QString& src);
    /** @brief */
    PlayJson::ContentType ContentTypeStringToEnum(const QString& src);
    /** @brief */
    PlayJson::MediaType MediaTypeStringToEunum(const QString& src);
    /** @brief */
    PlayJson::HourType HourTypeStringToEnum(const QString& src);
    /** @brief */
    PlayJson::DateType DateTypeStringToEnum(const QString& src);
    /** @brief */
    PlayJson::NewsCategory NewsCategoryStringToEnum(const QString& src);
    /** @brief */
    PlayJson::AniFlowDirection SubtitleDirectionStringToEnum(const QString& src);
    /** @brief */
    PlayJson::SubtitleAction SubtitleActionStringToEnum(const QString& src);

    // elgo_control <-> elgo_remote
    /** @brief */
    Remote::Action RemoteActionStringToEnum(const QString& src);

    /// enum to string

    // elgo_control <-> contents server
    /** @brief */
    void ContentServerEventEnumToString(const ContentSchema::Event event, QString& dest);
    /** @brief */
    void ContentServerPayloadTypeEnumToString(const ContentSchema::PayloadType type, QString& dest);

    /** @brief */
    void ResourceTypeEnumToString(const ResourceJson::ResourceType type, QString& dest);

    /** @brief */
    void PlayDataTypeEnumToString(const PlayJson::PlayDataType type, QString& dest);
    /** @brief */
    void OrientationTypeEnumToString(const PlayJson::Orientation type, QString& dest);
    /** @brief */
    void ContentTypeEnumToString(const PlayJson::ContentType type, QString& dest);
    /** @brief */
    void MediaTypeEnumToString(const PlayJson::MediaType type, QString& dest);
    /** @brief */
    void HourTypeEnumToString(const PlayJson::HourType type, QString& dest);
    /** @brief */
    void DateTypeEnumToString(const PlayJson::DateType type, QString& dest);
    /** @brief */
    void NewsCategoryEnumToString(const PlayJson::NewsCategory type, QString& dest);
    /** @brief */
    void SubtitleDirectionEnumToString(const PlayJson::AniFlowDirection type, QString& dest);
    /** @brief */
    void SubtitleActionEnumToString(const PlayJson::SubtitleAction type, QString& dest);

    // elgo_control <-> elgo_remote
    /** @brief */
    void RemoteActionEnumToString(const Remote::Action type, QString& dest);


    /// String to DataType

    // elgo_control <-> contents server
    /** @brief */
    void ScheduleDateTimeStringToQDateTime(const QString& src, QDateTime &dest);

    /** @brief */
    void CronCommandStringToTimeRepeat(const QString& src, ScheduleJson::Cron& dest);
    /** @brief */
    void CronCommandStringToDateRepeat(const QDate& startDate, const QDate& endDate, ScheduleJson::Cron& dest);
    /** @brief */
    void GetCronCommandConvertedList(const QString& src, const ScheduleJson::CronFormat format,
                               QVector<int>& dest, ScheduleJson::CronOption& options);
    /** @brief */
    void PrintConvertedCron(const ScheduleJson::Cron& src);
    /** @brief */
    void PrintConvertedCronDataList(const ScheduleJson::CronFormat type, const QVector<int>& src);

    /** @brief */
    int GetCronFormatLimitValue(const ScheduleJson::CronFormat format);
    /** @brief  cron format -> QDate */
    int GetConvertedDayOfWeek(const int cronDow);

    /** @brief */
    void GetSchedulePlayDataIdInName(const QString& src, QString& playDataId, QString& playDataType);
}

#endif // JSONSTRINGCONVERTER_H
