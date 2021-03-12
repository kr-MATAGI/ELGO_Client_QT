#ifndef CONTENTSERVERSCHEMA_H
#define CONTENTSERVERSCHEMA_H

// QT
#include <QString>

namespace ContentSchema
{
    /** @brief  Server -> elgo_control */
    enum Event
    {
        NONE_EVENT = 0,
        READY = 1,
        ACCESS = 2,
        ERROR = 9
    };

    /** @brief */
    enum PayloadType
    {
        NONE_TYPE = 0,
        ONCE = 1,
        REQUEST = 2,
        RESPONSE = 3,
        PROGRESS = 4
    };

    /** @brief */
    struct Payload
    {
        QString src;
        QString dest;
        QString message;
        PayloadType type;
        bool displayPower;
    };

    /** @brief */
    struct Summary
    {
        Event event;
        Payload payload;
    };

}

#endif // CONTENTSERVERSCHEMA_H
