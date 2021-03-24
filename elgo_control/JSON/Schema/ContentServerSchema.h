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
        RENAME = 3,
        SINGLE_PLAY = 4,
        PLAY_SCHEDULES = 5,
        POWER_SCHEDULES = 6,
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
        QString deviceName;
        QString url;
        PayloadType type;
        bool displayPower;
    };

    /** @brief */
    struct Summary
    {
        Event event;
        Payload payload;
    };


    // resource
    /** @brief */
    enum ResourceType
    {
        NONE_RSRC = 0,
        JS = 1,
        ICON = 2,
        DATA = 3,
        PAGE = 4,
        IMAGE = 5
    };
    const static char* ResourceTypeEnumToStr[] = { "NONE", "js", "icon", "data", "page", "image" };

    /** @brief */
    struct Resource
    {
        ResourceType type;
        QString name;
        QString url;
        int size; // unit : byte
    };

    // Schedule
    /** @brief */
    enum CronFormat
    {
        e_SEC = 1,
        e_MIN = 2,
        e_HOUR = 3,
        e_DAY = 4,
        e_MONTH = 5,
        e_DOW = 6
    };

    /** @brief */
    struct CronOption
    {
        QList<CronFormat> last;
        int numOfWeek;
        int weekday;
    };

    /** @brief */
    struct Cron
    {
        QList<int> secRepeat;
        QList<int> minRepeat;
        QList<int> hourRepeat;
        QList<int> dayRepeat;
        QList<int> monthRepeat;
        QList<int> dowRepeat;

        CronOption options;
    };

    /** @brief */
    struct ScheduleData
    {
        QDateTime startTime;
        QDateTime endTime;
        Cron cron;
        QString name;
    };

    /** @brief */
    struct Schedules
    {
        QString id;
        QList<ScheduleData> scheduleList;
    };
}

#endif // CONTENTSERVERSCHEMA_H
