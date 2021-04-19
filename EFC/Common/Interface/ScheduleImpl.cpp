#include "ScheduleImpl.h"

//========================================================
QDataStream& operator<<(QDataStream& ds, const ScheduleJson::PlaySchedule& src)
//========================================================
{
    ds << src.id;
    ds << src.scheduleList;

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
    ds << src.secList;
    ds << src.minList;
    ds << src.hourList;
    ds << src.dayList;
    ds << src.monthList;
    ds << src.dowList;
    ds << src.yearList;
    ds << src.options;

    return ds;
}

//========================================================
QDataStream& operator<<(QDataStream& ds, const ScheduleJson::CronOption& src)
//========================================================
{
    ds << src.last;
    ds << src.numOfWeek;
    ds << src.weekday;

    return ds;
}

//========================================================
QDataStream& operator<<(QDataStream& ds, const ScheduleJson::PowerSchedules& src)
//========================================================
{
    ds << src.onScheduleList;
    ds << src.offScheduleList;

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
    ds << src.schduleList;

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
QDataStream &operator>>(QDataStream& ds, ScheduleJson::PlaySchedule& dest)
//========================================================
{
    ds >> dest.id;
    ds >> dest.scheduleList;

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
    ds >> dest.secList;
    ds >> dest.minList;
    ds >> dest.hourList;
    ds >> dest.dayList;
    ds >> dest.monthList;
    ds >> dest.dowList;
    ds >> dest.yearList;
    ds >> dest.options;

    return ds;
}

//========================================================
QDataStream &operator>>(QDataStream& ds, ScheduleJson::CronOption& dest)
//========================================================
{
    ds >> dest.last;
    ds >> dest.numOfWeek;
    ds >> dest.weekday;

    return ds;
}

//========================================================
QDataStream& operator>>(QDataStream& ds, ScheduleJson::PowerSchedules& dest)
//========================================================
{
    ds >> dest.onScheduleList;
    ds >> dest.offScheduleList;

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
    ds >> dest.schduleList;

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
