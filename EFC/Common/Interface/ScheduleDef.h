#ifndef PLAYSCHEDULEDEF_H
#define PLAYSCHEDULEDEF_H

// QT
#include <QList>
#include <QVector>
#include <QString>
#include <QDateTime>

// Common
#include "ContentsPlayDataDef.h"

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
        QVector<CronFormat> last;
        int numOfWeek;
        int weekday;
    };

    /** @brief */
    struct Cron
    {
        QVector<int> secRepeat;
        QVector<int> minRepeat;
        QVector<int> hourRepeat;
        QVector<int> dayRepeat;
        QVector<int> monthRepeat;
        QVector<int> dowRepeat;

        CronOption options;
    };

    /** @brief */
    struct PlayScheduleData
    {
        QDateTime startTime;
        QDateTime endTime;
        Cron cron;
        int playDataId; // name
        PlayJson::PlayDataType type;
    };

    /** @brief  Play Schedules */
    struct PlaySchedules
    {
        QString id;
        QVector<PlayScheduleData> scheduleList;
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
        QVector<PowerScheduleData> onScheduleList;
        QVector<PowerScheduleData> offScheduleList;
    };


    /**
     *  @note
     *          Not using maybe
     *          - siglePlay not related schedules
     *  @brief
     *          Single Play Schedule */
    struct SinglePlayScheduleData
    {
        QDateTime startTime;
        QDateTime endTime;
        Cron cron;
        int playDataId;
        PlayJson::PlayDataType type;
    };

    struct SinglePlaySchedules
    {
        QVector<SinglePlayScheduleData> schduleList;
    };
}

#endif // PLAYSCHEDULEDEF_H
