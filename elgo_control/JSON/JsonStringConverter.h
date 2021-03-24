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
    ContentSchema::ResourceType ResourceTypeStringToEnum(const QString& src);


    /// enum to string
    /** @brief */
    void ContentServerEventEnumToString(const ContentSchema::Event event, QString& dest);
    /** @brief */
    void ContentServerPayloadTypeEnumToString(const ContentSchema::PayloadType type, QString& dest);

    /** @brief */
    void ResourceTypeEnumToString(const ContentSchema::ResourceType type, QString& dest);


    /// String to DataType
    /** @brief */
    void ScheduleDateTimeStringToQDateTime(const QString& src, QDateTime &dest);

    /** @brief */
    void CronCommandStringToStruct(const QString& src, ContentSchema::Cron& dest);
    /** @brief */
    void GetCronCommandConvertedList(const QString& src, const ContentSchema::CronFormat format,
                               QList<int>& dest, ContentSchema::CronOption& options);
    /** @brief */
    int GetCronFormatLimitValue(const ContentSchema::CronFormat format);
    /** @brief  cron format -> QDate */
    int GetConvertedDayOfWeek(const int cronDow);
}

#endif // JSONSTRINGCONVERTER_H
