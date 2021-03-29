#ifndef PLAYSCHEDULEDEF_H
#define PLAYSCHEDULEDEF_H

#include <QList>
#include <QString>
#include <QDateTime>

namespace ScheduleJson
{
    // Schedule
    /** @brief */
    enum CronFormat
    {
        NONE_CRON_FORMAT = 0,
        SEC = 1,
        MIN = 2,
        HOUR = 3,
        DAY = 4,
        MONTH = 5,
        DOW = 6
    };

    static const char* CronFormatEnumToStr[] = {"none", "sec", "min", "hour",
                                               "day", "month", "DOW"};

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
    struct PlayScheduleData
    {
        QDateTime startTime;
        QDateTime endTime;
        Cron cron;
        QString id;
    };

    /** @brief  Play Schedules */
    struct PlaySchedules
    {
        QString id;
        QList<PlayScheduleData> scheduleList;
    };


    /** @brief  Power Schedules */
    struct PowerScheduleData
    {
        QString id;
        QDateTime startTime;
        QDateTime endTime;
        Cron cron;
    };

    struct PowerSchedules
    {
        QList<PowerScheduleData> onScheduleList;
        QList<PowerScheduleData> offScheduleList;
    };


    /** @brief  Single Play Schedule */
    struct SinglePlayData
    {
        QDateTime startTime;
        QDateTime endTime;
        Cron cron;
        QString id;
    };

    struct SinglePlaySchedules
    {
        QList<SinglePlayData> schduleList;
    };
}

#endif // PLAYSCHEDULEDEF_H
