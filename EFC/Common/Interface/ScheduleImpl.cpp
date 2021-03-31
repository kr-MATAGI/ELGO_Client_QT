#include "ScheduleImpl.h"

//========================================================
QDataStream& operator<<(QDataStream& ds, const ScheduleJson::PlaySchedules& src)
//========================================================
{
    ds << src.id;
    const int scheduleDataListSize = src.scheduleList.size();
    ds << scheduleDataListSize;

    for(int idx = 0; idx < scheduleDataListSize; idx++)
    {
        ds << src.scheduleList[idx];
    }

    return ds;
}

//========================================================
QDataStream& operator<<(QDataStream& ds, const ScheduleJson::PlayScheduleData& src)
//========================================================
{
    ds << src.startTime;
    ds << src.endTime;
    ds << src.cron;
    ds << src.playDataId;
    ds << src.type;

    return ds;
}

//========================================================
QDataStream& operator<<(QDataStream& ds, const ScheduleJson::Cron& src)
//========================================================
{
    const int secListSize = src.secRepeat.size();
    ds << secListSize;
    for(int idx = 0; idx < secListSize; idx++)
    {
        ds << src.secRepeat[idx];
    }

    const int minListSize = src.minRepeat.size();
    ds << minListSize;
    for(int idx = 0; idx < minListSize; idx++)
    {
        ds << src.minRepeat[idx];
    }

    const int hourListSize = src.hourRepeat.size();
    ds << hourListSize;
    for(int idx = 0; idx < hourListSize; idx++)
    {
        ds << src.hourRepeat[idx];
    }

    const int dayListSize = src.dayRepeat.size();
    ds << dayListSize;
    for(int idx = 0; idx < dayListSize; idx++)
    {
        ds << src.dayRepeat[idx];
    }

    const int monthListSize = src.monthRepeat.size();
    ds << monthListSize;
    for(int idx = 0; idx < monthListSize; idx++)
    {
        ds << src.monthRepeat[idx];
    }

    const int dowListSize = src.dowRepeat.size();
    ds << dowListSize;
    for(int idx = 0; idx < dowListSize; idx++)
    {
        ds << src.dowRepeat[idx];
    }

    ds << src.options;

    return ds;
}

//========================================================
QDataStream& operator<<(QDataStream& ds, const ScheduleJson::CronOption& src)
//========================================================
{
    const int lastListSize = src.last.size();
    ds << lastListSize;
    for(int idx = 0; idx < lastListSize; idx++)
    {
        ds << src.last[idx];
    }

    ds << src.numOfWeek;
    ds << src.weekday;

    return ds;
}

//========================================================
QDataStream& operator<<(QDataStream& ds, const ScheduleJson::PowerSchedules& src)
//========================================================
{
    const int onScheduleListSize = src.onScheduleList.size();
    ds << onScheduleListSize;
    for(int idx = 0; idx < onScheduleListSize; idx++)
    {
        ds << src.onScheduleList[idx];
    }

    const int offScheduleListSize = src.offScheduleList.size();
    ds << offScheduleListSize;
    for(int idx = 0; idx < offScheduleListSize; idx++)
    {
        ds << src.offScheduleList[idx];
    }

    return ds;
}

//========================================================
QDataStream& operator<<(QDataStream& ds, const ScheduleJson::PowerScheduleData& src)
//========================================================
{
    ds << src.id;
    ds << src.startTime;
    ds << src.endTime;
    ds << src.cron;

    return ds;
}

//========================================================
QDataStream& operator<<(QDataStream& ds, const ScheduleJson::SinglePlaySchedules& src)
//========================================================
{
    const int scheduleListSize = src.schduleList.size();
    ds << scheduleListSize;
    for(int idx = 0; idx < scheduleListSize; idx++)
    {
        ds << src.schduleList[idx];
    }

    return ds;
}

//========================================================
QDataStream& operator<<(QDataStream& ds, const ScheduleJson::SinglePlayScheduleData& src)
//========================================================
{
    ds << src.startTime;
    ds << src.endTime;
    ds << src.cron;
    ds << src.playDataId;
    ds << src.type;

    return ds;
}

//========================================================
QDataStream &operator>>(QDataStream& ds, ScheduleJson::PlaySchedules& dest)
//========================================================
{
    ds >> dest.id;

    int scheduleDataListSize = 0;
    ds >> scheduleDataListSize;
    for(int idx = 0; idx < scheduleDataListSize; idx++)
    {
        ScheduleJson::PlayScheduleData scheduleData;
        ds >> scheduleData;
        dest.scheduleList.push_back(scheduleData);
    }

    return ds;
}

//========================================================
QDataStream &operator>>(QDataStream& ds, ScheduleJson::PlayScheduleData& dest)
//========================================================
{
    ds >> dest.startTime;
    ds >> dest.endTime;
    ds >> dest.cron;
    ds >> dest.playDataId;
    ds >> dest.type;

    return ds;
}

//========================================================
QDataStream &operator>>(QDataStream& ds, ScheduleJson::Cron& dest)
//========================================================
{
    int secListSize = 0;
    ds >> secListSize;
    for(int idx = 0; idx < secListSize; idx++)
    {
        int sec = 0;
        ds >> sec;
        dest.secRepeat.push_back(sec);
    }

    int minListSize = 0;
    ds >> minListSize;
    for(int idx = 0; idx < minListSize; idx++)
    {
        int min = 0;
        ds >> min;
        dest.minRepeat.push_back(min);
    }

    int hourListSize = 0;
    ds >> hourListSize;
    for(int idx = 0; idx < hourListSize; idx++)
    {
        int hour = 0;
        ds >> hour;
        dest.hourRepeat.push_back(hour);
    }

    int dayListSize = 0;
    ds >> dayListSize;
    for(int idx = 0; idx < dayListSize; idx++)
    {
        int day = 0;
        ds >> day;
        dest.dayRepeat.push_back(day);
    }

    int monthListSize = 0;
    ds >> monthListSize;
    for(int idx = 0; idx < monthListSize; idx++)
    {
        int month = 0;
        ds >> month;
        dest.monthRepeat.push_back(month);
    }

    int dowListSize = 0;
    ds >> dowListSize;
    for(int idx = 0; idx < dowListSize; idx++)
    {
        int dow = 0;
        ds >> dow;
        dest.dowRepeat.push_back(dow);
    }

    ds >> dest.options;

    return ds;
}

//========================================================
QDataStream &operator>>(QDataStream& ds, ScheduleJson::CronOption& dest)
//========================================================
{
    int lastListSize = 0;
    ds >> lastListSize;
    for(int idx = 0; idx < lastListSize; idx++)
    {
        ScheduleJson::CronFormat format;
        ds >> format;
        dest.last.push_back(format);
    }

    ds >> dest.numOfWeek;
    ds >> dest.weekday;

    return ds;
}

//========================================================
QDataStream& operator>>(QDataStream& ds, ScheduleJson::PowerSchedules& dest)
//========================================================
{
    int onScheduleListSize = 0;
    ds >> onScheduleListSize;
    for(int idx = 0; idx < onScheduleListSize; idx++)
    {
        ScheduleJson::PowerScheduleData powerSchedule;
        ds >> powerSchedule;
        dest.onScheduleList.push_back(powerSchedule);
    }

    int offScheduleListSize = 0;
    ds >> offScheduleListSize;
    for(int idx = 0; idx < offScheduleListSize; idx++)
    {
        ScheduleJson::PowerScheduleData powerSchedule;
        ds >> powerSchedule;
        dest.offScheduleList.push_back(powerSchedule);
    }

    return ds;
}

//========================================================
QDataStream& operator>>(QDataStream& ds, ScheduleJson::PowerScheduleData& dest)
//========================================================
{
    ds >> dest.id;
    ds >> dest.startTime;
    ds >> dest.endTime;
    ds >> dest.cron;

    return ds;
}

//========================================================
QDataStream& operator>>(QDataStream& ds, ScheduleJson::SinglePlaySchedules& dest)
//========================================================
{
    int scheduleListSize = 0;
    ds >> scheduleListSize;
    for(int idx = 0; idx < scheduleListSize; idx++)
    {
        ScheduleJson::SinglePlayScheduleData singlePlayData;
        ds >> singlePlayData;
        dest.schduleList.push_back(singlePlayData);
    }

    return ds;
}

//========================================================
QDataStream& operator>>(QDataStream& ds, ScheduleJson::SinglePlayScheduleData& dest)
//========================================================
{
    ds >> dest.startTime;
    ds >> dest.endTime;
    ds >> dest.cron;
    ds >> dest.playDataId;
    ds >> dest.type;

    return ds;
}
