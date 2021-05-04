#ifndef CONTENTSERVERSCHEMA_H
#define CONTENTSERVERSCHEMA_H

// QT
#include <QString>
#include <QDateTime>

namespace ContentSchema
{
    /** @brief  Server -> elgo_control */
    enum Event
    {
        NONE_EVENT = 0,

        READY = 1,
        ACCESS = 2,
        RENAME = 3, // payload type : once, request
        SINGLE_PLAY = 4,
        PLAY_SCHEDULE = 5,
        POWER_SCHEDULE = 6,
        DISPLAY_ON = 7,
        DISPLAY_OFF = 8,
        SCREEN_CAPTURE = 9,
        SYSTEM_REBOOT = 10,
        CLEAR_PLAY_SCHEDULE = 11,
        CLEAR_POWER_SCHEDULE = 12,

        ERROR = 255
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
        QString deviceName;
        QString url;
        QString path;
        QString scheduleId;
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

namespace ResourceJson
{
    // resource
    /** @brief */
    enum ResourceType
    {
        NONE_RSRC = 0,
        JS = 1,
        ICON = 2,
        DATA = 3,
        PAGE = 4,
        IMAGE = 5,
        VIDEO = 6,
        OBJECT = 7
    };
    const static char* ResourceTypeEnumToStr[] = { "NONE", "js", "icon", "data",
                                                   "page", "image", "video", "object" };

    /** @brief */
    struct Resource
    {
        ResourceType type;
        QString name;
        QString url;
        int size; // unit : byte
    };
}

#endif // CONTENTSERVERSCHEMA_H
