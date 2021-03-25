#ifndef JSONSTRINGCONVERTER_H
#define JSONSTRINGCONVERTER_H

// QT
#include <QString>

// Control
#include "Schema/ContentServerSchema.h"

namespace JsonStringConverter
{
    /// string to enum
    /** @brief */
    ContentSchema::Event ContentServerEventStringToEnum(const QString& src);
    /** @brief */
    ContentSchema::PayloadType ContentServerPayloadTypeStringToEnum(const QString& src);

    /** @brief */
    ResourceJson::ResourceType ResourceTypeStringToEnum(const QString& src);

    /** @brief */
    ObjectJson::PlayDataType PlayDataTypeStringToEnum(const QString& src);
    /** @brief */
    ObjectJson::Orientation OrientationTypeStringToEnum(const QString& src);
    /** @brief */
    ObjectJson::ContentType ContentTypeStringToEnum(const QString& src);
    /** @brief */
    ObjectJson::MediaType MediaTypeStringToEunum(const QString& src);
    /** @brief */
    ObjectJson::HourType HourTypeStringToEnum(const QString& src);
    /** @brief */
    ObjectJson::DateType DateTypeStringToEnum(const QString& src);
    /** @brief */
    ObjectJson::NewsCategory NewsCategoryStringToEnum(const QString& src);
    /** @brief */
    ObjectJson::SubtitleDirection SubtitleDirectionStringToEnum(const QString& src);
    /** @brief */
    ObjectJson::SubtitleAction SubtitleActionStringToEnum(const QString& src);


    /// enum to string
    /** @brief */
    void ContentServerEventEnumToString(const ContentSchema::Event event, QString& dest);
    /** @brief */
    void ContentServerPayloadTypeEnumToString(const ContentSchema::PayloadType type, QString& dest);

    /** @brief */
    void ResourceTypeEnumToString(const ResourceJson::ResourceType type, QString& dest);

    /** @brief */
    void PlayDataTypeEnumToString(const ObjectJson::PlayDataType type, QString& dest);
    /** @brief */
    void OrientationTypeEnumToString(const ObjectJson::Orientation type, QString& dest);
    /** @brief */
    void ContentTypeEnumToString(const ObjectJson::ContentType type, QString& dest);
    /** @brief */
    void MediaTypeEnumToString(const ObjectJson::MediaType type, QString& dest);
    /** @brief */
    void HourTypeEnumToString(const ObjectJson::HourType type, QString& dest);
    /** @brief */
    void DateTypeEnumToString(const ObjectJson::DateType type, QString& dest);
    /** @brief */
    void NewsCategoryEnumToString(const ObjectJson::NewsCategory type, QString& dest);
    /** @brief */
    void SubtitleDirectionEnumToString(const ObjectJson::SubtitleDirection type, QString& dest);
    /** @brief */
    void SubtitleActionEnumToString(const ObjectJson::SubtitleAction type, QString& dest);


    /// String to DataType
    /** @brief */
    void ScheduleDateTimeStringToQDateTime(const QString& src, QDateTime &dest);

    /** @brief */
    void CronCommandStringToStruct(const QString& src, ScheduleJson::Cron& dest);
    /** @brief */
    void GetCronCommandConvertedList(const QString& src, const ScheduleJson::CronFormat format,
                               QList<int>& dest, ScheduleJson::CronOption& options);
    /** @brief */
    int GetCronFormatLimitValue(const ScheduleJson::CronFormat format);
    /** @brief  cron format -> QDate */
    int GetConvertedDayOfWeek(const int cronDow);
}

#endif // JSONSTRINGCONVERTER_H
