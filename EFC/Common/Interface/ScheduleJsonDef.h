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
        DOW = 6,
        YEAR = 7
    };

    static const char* CronFormatEnumToStr[] = {"none", "sec", "min", "hour",
                                               "day", "month", "DOW", "year"};

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
        /**
         * @note    CronOption is not used
         */
        QVector<int> secList;
        QVector<int> minList;
        QVector<int> hourList;
        QVector<int> dayList;
        QVector<int> monthList;
        QVector<int> dowList;
        QVector<int> yearList;

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
    struct PlaySchedule
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

    struct PowerSchedule
    {
        QVector<PowerScheduleData> onScheduleList;
        QVector<PowerScheduleData> offScheduleList;
    };


    /**
     *  @note
     *          Not using maybe
     *          - siglePlay not related schedule
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
