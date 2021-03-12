#ifndef JSONSTRINGCONVERTER_H
#define JSONSTRINGCONVERTER_H

// QT
#include <QString>

// Control
#include "Schema/ContentServerSchema.h"

namespace JsonStringConverter
{
    // string to enum
    /** @brief */
    ContentSchema::Event ContentServerEventStringToEnum(const QString& src);
    /** @brief */
    ContentSchema::PayloadType ContentServerPayloadTypeStringToEnum(const QString& src);

    // enum to string
    /** @brief */
    void ContentServerEventEnumToString(const ContentSchema::Event event, QString& dest);
    /** @brief */
    void ContentServerPayloadTypeEnumToString(const ContentSchema::PayloadType type, QString& dest);
}

#endif // JSONSTRINGCONVERTER_H
