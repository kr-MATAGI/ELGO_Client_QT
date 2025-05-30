// std
#include <sstream>

// Control
#include "Logger/ControlLogger.h"
#include "JsonStringConverter.h"

//========================================================
ContentSchema::Event JsonStringConverter::ContentServerEventStringToEnum(const QString& src)
//========================================================
{
    ContentSchema::Event retValue = ContentSchema::Event::NONE_EVENT;

    if(0 == strcmp("ready", src.toStdString().c_str()))
    {
        retValue = ContentSchema::Event::READY;
    }
    else if(0 == strcmp("access", src.toStdString().c_str()))
    {
        retValue = ContentSchema::Event::ACCESS;
    }
    else if(0 == strcmp("rename", src.toStdString().c_str()))
    {
        retValue = ContentSchema::Event::RENAME;
    }
    else if(0 == strcmp("on", src.toStdString().c_str()))
    {
        retValue = ContentSchema::Event::DISPLAY_ON;
    }
    else if(0 == strcmp("off", src.toStdString().c_str()))
    {
        retValue = ContentSchema::Event::DISPLAY_OFF;
    }
    else if(0 == strcmp("capture", src.toStdString().c_str()))
    {
        retValue = ContentSchema::Event::SCREEN_CAPTURE;
    }
    else if(0 == strcmp("reboot", src.toStdString().c_str()))
    {
        retValue = ContentSchema::Event::SYSTEM_REBOOT;
    }
    else if(0 == strcmp("singlePlay", src.toStdString().c_str()))
    {
        retValue = ContentSchema::Event::SINGLE_PLAY;
    }
    else if(0 == strcmp("playSchedule", src.toStdString().c_str()))
    {
        retValue = ContentSchema::Event::PLAY_SCHEDULE;
    }
    else if(0 == strcmp("powerSchedule", src.toStdString().c_str()))
    {
        retValue = ContentSchema::Event::POWER_SCHEDULE;
    }
    else if(0 == strcmp("clearPlaySchedule", src.toStdString().c_str()))
    {
        retValue = ContentSchema::Event::CLEAR_PLAY_SCHEDULE;
    }
    else if(0 == strcmp("clearPowerSchedule", src.toStdString().c_str()))
    {
        retValue = ContentSchema::Event::CLEAR_POWER_SCHEDULE;
    }
    else if(0 == strcmp("error", src.toStdString().c_str()))
    {
        retValue = ContentSchema::Event::ERROR;
    }
    else
    {
        ELGO_CONTROL_LOG("Error - Unknwon Event %s", src.toUtf8().constData());
    }

    return retValue;
}

//========================================================
ContentSchema::PayloadType JsonStringConverter::ContentServerPayloadTypeStringToEnum(const QString& src)
//========================================================
{
    ContentSchema::PayloadType retValue = ContentSchema::PayloadType::NONE_TYPE;

    if(0 == strcmp("once", src.toStdString().c_str()))
    {
        retValue = ContentSchema::PayloadType::ONCE;
    }
    else if(0 == strcmp("request", src.toStdString().c_str()))
    {
        retValue = ContentSchema::PayloadType::REQUEST;
    }
    else if(0 == strcmp("response", src.toStdString().c_str()))
    {
        retValue = ContentSchema::PayloadType::RESPONSE;
    }
    else if(0 == strcmp("progress", src.toStdString().c_str()))
    {
        retValue = ContentSchema::PayloadType::PROGRESS;
    }
    else
    {
        ELGO_CONTROL_LOG("Error - Unknwon payload type : %s", src.toUtf8().constData());
    }

    return retValue;
}

//========================================================
ResourceJson::ResourceType JsonStringConverter::ResourceTypeStringToEnum(const QString& src)
//========================================================
{
    ResourceJson::ResourceType retValue = ResourceJson::ResourceType::NONE_RSRC;

    if(0 == strcmp("js", src.toStdString().c_str()))
    {
        retValue = ResourceJson::ResourceType::JS;
    }
    else if(0 == strcmp("icon", src.toStdString().c_str()))
    {
        retValue = ResourceJson::ResourceType::ICON;
    }
    else if(0 == strcmp("data", src.toStdString().c_str()))
    {
        retValue = ResourceJson::ResourceType::DATA;
    }
    else if(0 == strcmp("page", src.toStdString().c_str()))
    {
        retValue = ResourceJson::ResourceType::PAGE;
    }
    else if(0 == strcmp("image", src.toStdString().c_str()))
    {
        retValue = ResourceJson::ResourceType::IMAGE;
    }
    else if(0 == strcmp("video", src.toStdString().c_str()))
    {
        retValue = ResourceJson::ResourceType::VIDEO;
    }
    else if(0 == strcmp("object", src.toStdString().c_str()))
    {
        retValue = ResourceJson::ResourceType::OBJECT;
    }
    else
    {
        ELGO_CONTROL_LOG("Error - Unknwon resource type : %s", src.toUtf8().constData());
    }

    return retValue;
}

//========================================================
PlayJson::PlayDataType JsonStringConverter::PlayDataTypeStringToEnum(const QString& src)
//========================================================
{
    PlayJson::PlayDataType retValue = PlayJson::PlayDataType::NONE_PLAY_DATA_TYPE;

    if(0 == strcmp("custom", src.toStdString().c_str()))
    {
        retValue = PlayJson::PlayDataType::CUSTOM;
    }
    else if(0 == strcmp("fixed", src.toStdString().c_str()))
    {
        retValue = PlayJson::PlayDataType::FIXED;
    }
    else
    {
        ELGO_CONTROL_LOG("Error - Unknwon playdata type : %s", src.toUtf8().constData());
    }

    return retValue;
}

//========================================================
PlayJson::Orientation JsonStringConverter::OrientationTypeStringToEnum(const QString& src)
//========================================================
{
    PlayJson::Orientation retValue = PlayJson::Orientation::NONE_ORIENTATION;

    if(0 == strcmp("landscape", src.toStdString().c_str()))
    {
        retValue = PlayJson::Orientation::HORIZONTAL;
    }
    else if(0 == strcmp("portrait", src.toStdString().c_str()))
    {
        retValue = PlayJson::Orientation::VERTICAL;
    }
    else
    {
        ELGO_CONTROL_LOG("Error - Unknwon orientation type : %s", src.toUtf8().constData());
    }

    return retValue;
}

//========================================================
PlayJson::ContentType JsonStringConverter::ContentTypeStringToEnum(const QString& src)
//========================================================
{
    PlayJson::ContentType retValue = PlayJson::ContentType::NONE_CONTENT;

    if(0 == strcmp("file", src.toStdString().c_str()))
    {
        retValue = PlayJson::ContentType::FILE;
    }
    else if(0 == strcmp("widget", src.toStdString().c_str()))
    {
        retValue = PlayJson::ContentType::WIDGET;
    }
    else
    {
        ELGO_CONTROL_LOG("Error - Unknwon content type : %s", src.toUtf8().constData());
    }

    return retValue;
}

//========================================================
PlayJson::MediaType JsonStringConverter::MediaTypeStringToEunum(const QString& src)
//========================================================
{
    PlayJson::MediaType retValue = PlayJson::MediaType::NONE_MEDIA;

    if(0 == strcmp("image", src.toStdString().c_str()))
    {
        retValue = PlayJson::MediaType::IMAGE;
    }
    else if(0 == strcmp("video", src.toStdString().c_str()))
    {
        retValue = PlayJson::MediaType::VIDEO;
    }
    else if(0 == strcmp("clock", src.toStdString().c_str()))
    {
        retValue = PlayJson::MediaType::CLOCK;
    }
    else if(0 == strcmp("date", src.toStdString().c_str()))
    {
        retValue = PlayJson::MediaType::DATE;
    }
    else if(0 == strcmp("weather", src.toStdString().c_str()))
    {
        retValue = PlayJson::MediaType::WEATHER;
    }
    else if(0 == strcmp("news", src.toStdString().c_str()))
    {
        retValue = PlayJson::MediaType::NEWS;
    }
    else
    {
        ELGO_CONTROL_LOG("Error - Unknwon media type : %s", src.toUtf8().constData());
    }

    return retValue;
}

//========================================================
PlayJson::HourType JsonStringConverter::HourTypeStringToEnum(const QString& src)
//========================================================
{
    PlayJson::HourType retValue = PlayJson::HourType::NONE_HOUR_TYPE;

    if(0 == strcmp("12h", src.toStdString().c_str()))
    {
        retValue = PlayJson::HourType::HOUR_12;
    }
    else if(0 == strcmp("24h", src.toStdString().c_str()))
    {
        retValue = PlayJson::HourType::HOUR_24;
    }
    else
    {
        ELGO_CONTROL_LOG("Error - Unknwon hour type : %s", src.toUtf8().constData());
    }

    return retValue;
}

//========================================================
PlayJson::DateType JsonStringConverter::DateTypeStringToEnum(const QString& src)
//========================================================
{
    PlayJson::DateType retValue = PlayJson::DateType::NONE_DATE_TYPE;

    if(0 == strcmp("yes", src.toStdString().c_str()))
    {
        retValue = PlayJson::DateType::YES;
    }
    else if(0 == strcmp("no", src.toStdString().c_str()))
    {
        retValue = PlayJson::DateType::NO;
    }
    else
    {
        ELGO_CONTROL_LOG("Error - Unknwon date type : %s", src.toUtf8().constData());
    }

    return retValue;
}

//========================================================
PlayJson::NewsCategory JsonStringConverter::NewsCategoryStringToEnum(const QString& src)
//========================================================
{
    PlayJson::NewsCategory retValue = PlayJson::NewsCategory::NONE_NEWS_CATEGORY;

    if(0 == strcmp("browse", src.toStdString().c_str()))
    {
        retValue = PlayJson::NewsCategory::LATEST;
    }
    else if(0 == strcmp("headline", src.toStdString().c_str()))
    {
        retValue = PlayJson::NewsCategory::HEADLINE;
    }
    else if(0 == strcmp("politics", src.toStdString().c_str()))
    {
        retValue = PlayJson::NewsCategory::POLITICS;
    }
    else if(0 == strcmp("economy", src.toStdString().c_str()))
    {
        retValue = PlayJson::NewsCategory::ECONOMY;
    }
    else if(0 == strcmp("society", src.toStdString().c_str()))
    {
        retValue = PlayJson::NewsCategory::SOCIETY;
    }
    else if(0 == strcmp("local", src.toStdString().c_str()))
    {
        retValue = PlayJson::NewsCategory::LOCAL;
    }
    else if(0 == strcmp("international", src.toStdString().c_str()))
    {
        retValue = PlayJson::NewsCategory::GLBOAL;
    }
    else if(0 == strcmp("culture", src.toStdString().c_str()))
    {
        retValue = PlayJson::NewsCategory::CULTURE;
    }
    else if(0 == strcmp("sports", src.toStdString().c_str()))
    {
        retValue = PlayJson::NewsCategory::SPORT;
    }
    else if(0 == strcmp("weather", src.toStdString().c_str()))
    {
        retValue = PlayJson::NewsCategory::NEWS_WEATHER;
    }
    else
    {
        ELGO_CONTROL_LOG("Error - Unknwon news category type : %s", src.toUtf8().constData());
    }

    return retValue;
}

//========================================================
PlayJson::AniFlowDirection JsonStringConverter::SubtitleDirectionStringToEnum(const QString& src)
//========================================================
{
    PlayJson::AniFlowDirection retValue = PlayJson::AniFlowDirection::NONE_DIRECTION;

    if(0 == strcmp("left", src.toStdString().c_str()))
    {
        retValue = PlayJson::AniFlowDirection::RIGHT_TO_LEFT;
    }
    else if(0 == strcmp("right", src.toStdString().c_str()))
    {
        retValue = PlayJson::AniFlowDirection::LEFT_TO_RIGHT;
    }
    else if(0 == strcmp("up", src.toStdString().c_str()))
    {
        retValue = PlayJson::AniFlowDirection::BOTTOM_TO_TOP;
    }
    else if(0 == strcmp("down", src.toStdString().c_str()))
    {
        retValue = PlayJson::AniFlowDirection::TOP_TO_BOTTOM;
    }
    else
    {
        ELGO_CONTROL_LOG("Error - Unknwon subtitle direction : %s", src.toUtf8().constData());
    }

    return retValue;
}

//========================================================
PlayJson::SubtitleAction JsonStringConverter::SubtitleActionStringToEnum(const QString& src)
//========================================================
{
    PlayJson::SubtitleAction retValue = PlayJson::SubtitleAction::NONE_ACTION;

    if(0 == strcmp("scroll", src.toStdString().c_str()))
    {
        retValue = PlayJson::SubtitleAction::SCROLL;
    }
    else if(0 == strcmp("alternate", src.toStdString().c_str()))
    {
        retValue = PlayJson::SubtitleAction::LOOP;
    }
    else
    {
        ELGO_CONTROL_LOG("Error - Unknwon subtitle action : %s", src.toUtf8().constData());
    }

    return retValue;
}

//========================================================
Remote::Action JsonStringConverter::RemoteActionStringToEnum(const QString& src)
//========================================================
{
    Remote::Action retValue = Remote::Action::NONE_ACTION;

    if(0 == strcmp("DEVICE_LOGIN", src.toStdString().c_str()))
    {
        retValue = Remote::Action::DEVICE_LOGIN;
    }
    else if(0 == strcmp("UPDATE_WIFI_LIST", src.toStdString().c_str()))
    {
        retValue = Remote::Action::UPDATE_WIFI_LIST;
    }
    else if(0 == strcmp("MANAGE_DEVICE", src.toStdString().c_str()))
    {
        retValue = Remote::Action::MANAGE_DEVICE;
    }
    else if(0 == strcmp("ROTATE_DISPLAY", src.toStdString().c_str()))
    {
        retValue = Remote::Action::ROTATE_DISPLAY;
    }
    else if(0 == strcmp("DEVICE_OPTIONS", src.toStdString().c_str()))
    {
        retValue = Remote::Action::DEVICE_OPTIONS;
    }
    else if(0 == strcmp("CONNECT_WIFI", src.toStdString().c_str()))
    {
        retValue = Remote::Action::CONNECT_WIFI;
    }
    else if(0 == strcmp("USER_LOGIN", src.toStdString().c_str()))
    {
        retValue = Remote::Action::USER_LOGIN;
    }
    else
    {
        ELGO_CONTROL_LOG("Error - Unknwon remote action : %s", src.toStdString().c_str());
    }

    return retValue;
}

//========================================================
Remote::Heading JsonStringConverter::RemoteHeadingStringToEnum(const QString& src)
//========================================================
{
    Remote::Heading retValue = Remote::Heading::NONE_HEADING;

    if(0 == strcmp("HEAD_TOP", src.toStdString().c_str()))
    {
        retValue = Remote::Heading::HEAD_TOP;
    }
    else if(0 == strcmp("HEAD_RIGHT", src.toStdString().c_str()))
    {
        retValue = Remote::Heading::HEAD_RIGHT;
    }
    else if(0 == strcmp("HEAD_BOTTOM", src.toStdString().c_str()))
    {
        retValue = Remote::Heading::HEAD_BOTTOM;
    }
    else if(0 == strcmp("HEAD_LEFT", src.toStdString().c_str()))
    {
        retValue = Remote::Heading::HEAD_LEFT;
    }
    else
    {
        ELGO_CONTROL_LOG("ERROR - Unknown Type: %s", src.toStdString().c_str());
    }

    return retValue;
}

//========================================================
void JsonStringConverter::ContentServerEventEnumToString(const ContentSchema::Event event, QString& dest)
//========================================================
{
    if(ContentSchema::Event::ACCESS == event)
    {
        dest = "access";
    }
    else if(ContentSchema::Event::RENAME == event)
    {
        dest = "rename";
    }
    else if(ContentSchema::Event::READY == event)
    {
        dest = "ready";
    }
    else if(ContentSchema::Event::SINGLE_PLAY == event)
    {
        dest = "singlePlay";
    }
    else if(ContentSchema::Event::DISPLAY_ON == event)
    {
        dest = "on";
    }
    else if(ContentSchema::Event::DISPLAY_OFF == event)
    {
        dest = "off";
    }
    else if(ContentSchema::Event::SCREEN_CAPTURE == event)
    {
        dest = "capture";
    }
    else if(ContentSchema::Event::SYSTEM_REBOOT == event)
    {
        dest = "reboot";
    }
    else if(ContentSchema::Event::PLAY_SCHEDULE == event)
    {
        dest = "playSchedule";
    }
    else if(ContentSchema::Event::POWER_SCHEDULE == event)
    {
        dest = "powerSchedule";
    }
    else if(ContentSchema::Event::CLEAR_PLAY_SCHEDULE == event)
    {
        dest = "clearPlaySchedule";
    }
    else if(ContentSchema::Event::CLEAR_POWER_SCHEDULE == event)
    {
        dest = "clearPowerSchedule";
    }
    else if(ContentSchema::Event::ERROR == event)
    {
        dest = "error";
    }
    else
    {
        dest = "none";
    }
}

//========================================================
void JsonStringConverter::ContentServerPayloadTypeEnumToString(const ContentSchema::PayloadType type, QString& dest)
//========================================================
{
    if(ContentSchema::PayloadType::ONCE == type)
    {
        dest = "once";
    }
    else if(ContentSchema::PayloadType::PROGRESS == type)
    {
        dest = "progress";
    }
    else if(ContentSchema::PayloadType::REQUEST == type)
    {
        dest = "request";
    }
    else if(ContentSchema::PayloadType::RESPONSE == type)
    {
        dest = "response";
    }
    else
    {
        dest = "none";
    }
}

//========================================================
void JsonStringConverter::ResourceTypeEnumToString(const ResourceJson::ResourceType type, QString& dest)
//========================================================
{
    if(ResourceJson::ResourceType::JS == type)
    {
        dest = "js";
    }
    else if(ResourceJson::ResourceType::ICON == type)
    {
        dest = "icon";
    }
    else if(ResourceJson::ResourceType::DATA == type)
    {
        dest = "data";
    }
    else if(ResourceJson::ResourceType::PAGE == type)
    {
        dest = "page";
    }
    else if(ResourceJson::ResourceType::IMAGE == type)
    {
        dest = "image";
    }
    else if(ResourceJson::ResourceType::VIDEO == type)
    {
        dest = "video";
    }
    else if(ResourceJson::ResourceType::OBJECT == type)
    {
        dest = "object";
    }
    else
    {
        dest = "none";
    }
}

//========================================================
void JsonStringConverter::PlayDataTypeEnumToString(const PlayJson::PlayDataType type, QString& dest)
//========================================================
{
    if(PlayJson::PlayDataType::CUSTOM == type)
    {
        dest = "custom";
    }
    else if(PlayJson::PlayDataType::FIXED == type)
    {
        dest = "fixed";
    }
    else
    {
        dest = "none";
    }
}

//========================================================
void JsonStringConverter::OrientationTypeEnumToString(const PlayJson::Orientation type, QString& dest)
//========================================================
{
    if(PlayJson::Orientation::HORIZONTAL == type)
    {
        dest = "landscape";
    }
    else if(PlayJson::Orientation::VERTICAL == type)
    {
        dest = "portrait";
    }
    else
    {
        dest = "none";
    }
}

//========================================================
void JsonStringConverter::ContentTypeEnumToString(const PlayJson::ContentType type, QString& dest)
//========================================================
{
    if(PlayJson::ContentType::FILE == type)
    {
        dest = "file";
    }
    else if(PlayJson::ContentType::WIDGET == type)
    {
        dest = "widget";
    }
    else
    {
        dest = "none";
    }
}

//========================================================
void JsonStringConverter::MediaTypeEnumToString(const PlayJson::MediaType type, QString& dest)
//========================================================
{
    if(PlayJson::MediaType::IMAGE == type)
    {
        dest = "image";
    }
    else if(PlayJson::MediaType::VIDEO == type)
    {
        dest = "video";
    }
    else if(PlayJson::MediaType::CLOCK == type)
    {
        dest = "clock";
    }
    else if(PlayJson::MediaType::DATE == type)
    {
        dest = "date";
    }
    else if(PlayJson::MediaType::WEATHER == type)
    {
        dest = "weather";
    }
    else if(PlayJson::MediaType::NEWS == type)
    {
        dest = "news";
    }
    else
    {
        dest = "none";
    }
}

//========================================================
void JsonStringConverter::HourTypeEnumToString(const PlayJson::HourType type, QString& dest)
//========================================================
{
    if(PlayJson::HourType::HOUR_12 == type)
    {
        dest = "12h";
    }
    else if(PlayJson::HourType::HOUR_24 == type)
    {
        dest = "24h";
    }
    else
    {
        dest = "none";
    }
}

//========================================================
void JsonStringConverter::DateTypeEnumToString(const PlayJson::DateType type, QString& dest)
//========================================================
{
    if(PlayJson::DateType::YES == type)
    {
        dest = "yes";
    }
    else if(PlayJson::DateType::NO == type)
    {
        dest = "no";
    }
    else
    {
        dest = "none";
    }
}

//========================================================
void JsonStringConverter::NewsCategoryEnumToString(const PlayJson::NewsCategory type, QString& dest)
//========================================================
{
    if(PlayJson::NewsCategory::LATEST == type)
    {
        dest = "browse";
    }
    else if(PlayJson::NewsCategory::HEADLINE == type)
    {
        dest = "headline";
    }
    else if(PlayJson::NewsCategory::POLITICS == type)
    {
        dest = "politics";
    }
    else if(PlayJson::NewsCategory::ECONOMY == type)
    {
        dest = "economy";
    }
    else if(PlayJson::NewsCategory::SOCIETY == type)
    {
        dest = "society";
    }
    else if(PlayJson::NewsCategory::LOCAL == type)
    {
        dest = "local";
    }
    else if(PlayJson::NewsCategory::GLBOAL == type)
    {
        dest = "international";
    }
    else if(PlayJson::NewsCategory::CULTURE == type)
    {
        dest = "culture";
    }
    else if(PlayJson::NewsCategory::SPORT == type)
    {
        dest = "sports";
    }
    else if(PlayJson::NewsCategory::NEWS_WEATHER == type)
    {
        dest = "weather";
    }
}

//========================================================
void JsonStringConverter::SubtitleDirectionEnumToString(const PlayJson::AniFlowDirection type, QString& dest)
//========================================================
{
    if(PlayJson::AniFlowDirection::RIGHT_TO_LEFT == type)
    {
        dest = "left";
    }
    else if(PlayJson::AniFlowDirection::LEFT_TO_RIGHT == type)
    {
        dest = "right";
    }
    else if(PlayJson::AniFlowDirection::TOP_TO_BOTTOM == type)
    {
        dest = "down";
    }
    else if(PlayJson::AniFlowDirection::BOTTOM_TO_TOP == type)
    {
        dest = "up";
    }
    else
    {
        dest = "none";
    }
}

//========================================================
void JsonStringConverter::SubtitleActionEnumToString(const PlayJson::SubtitleAction type, QString& dest)
//========================================================
{
    if(PlayJson::SubtitleAction::SCROLL == type)
    {
        dest = "scroll";
    }
    else if(PlayJson::SubtitleAction::LOOP == type)
    {
        dest = "alternate";
    }
    else
    {
        dest = "none";
    }
}

//========================================================
void JsonStringConverter::RemoteActionEnumToString(const Remote::Action type, QString& dest)
//========================================================
{
    if(Remote::Action::DEVICE_LOGIN == type)
    {
        dest = "DEVICE_LOGIN";
    }
    else if(Remote::Action::UPDATE_WIFI_LIST == type)
    {
        dest = "UPDATE_WIFI_LIST";
    }
    else if(Remote::Action::MANAGE_DEVICE == type)
    {
        dest = "MANAGE_DEVICE";
    }
    else if(Remote::Action::ROTATE_DISPLAY == type)
    {
        dest = "ROTATE_DISPLAY";
    }
    else if(Remote::Action::DEVICE_OPTIONS == type)
    {
        dest = "DEVICE_OPTIONS";
    }
    else if(Remote::Action::CONNECT_WIFI == type)
    {
        dest = "CONNECT_WIFI";
    }
    else if(Remote::Action::USER_LOGIN == type)
    {
        dest = "USER_LOGIN";
    }
    else
    {
        dest = "NONE";
    }
}

//========================================================
void JsonStringConverter::ScheduleDateTimeStringToQDateTime(const QString& src, QDateTime &dest,
                                                            const bool bIsEndTime)
//========================================================
{
    const QStringList dateTimeSplit = src.split("T");
    const QString dateStr = dateTimeSplit[0];
    const QString timeStr = dateTimeSplit[1];

    // date
    QDate date;
    const QStringList dateSplit = dateStr.split("-");
    const int year = dateSplit[0].toInt();
    const int month = dateSplit[1].toInt();
    const int day = dateSplit[2].toInt();
    date.setDate(year, month, day);

    // time
    QTime time;
    const QStringList timeSplit = timeStr.split(":");
    int hour = timeSplit[0].toInt();
    int min = timeSplit[1].toInt();
    if(true == bIsEndTime)
    {
        min += 1;
        if(60 == min)
        {
            min = 0;
            hour +=1;
        }
        if(24 == hour)
        {
            hour = 0;
        }
    }

    const QString secMsecStr = timeSplit[2];
    const QStringList secMsecSplit = secMsecStr.split(".");
    int sec = secMsecSplit[0].toInt();
    const int msec = secMsecSplit[1].split("Z")[0].toInt();
    if(true == bIsEndTime)
    {
        sec = 59;
    }

    time.setHMS(hour, min, sec, msec);

    dest.setTime(time);
    dest.setDate(date);

    const qint64 utcSec = dest.toSecsSinceEpoch();

    // utc -> local(kor)
    dest.setSecsSinceEpoch(utcSec + 32400);
}

//========================================================
void JsonStringConverter::CronCommandStringToTimeRepeat(const QString& src, ScheduleJson::Cron& dest)
//========================================================
{
    ScheduleJson::CronOption options;

    QStringList cronStrSplit = src.split(" ");
//    QString secStr = cronStrSplit[0];
    QString minStr = cronStrSplit[1];
    QString hourStr = cronStrSplit[2];

//    QString dayStr = cronStrSplit[3];
//    QString monthStr = cronStrSplit[4];
    QString dayOfWeekStr = cronStrSplit[5];

    /// time
    // sec - not use
//    GetCronCommandConvertedList(secStr, ScheduleJson::CronFormat::SEC, dest.secList, options);

    // min
    GetCronCommandConvertedList(minStr, ScheduleJson::CronFormat::MIN, dest.minList, options);

    // hour()
    GetCronCommandConvertedList(hourStr, ScheduleJson::CronFormat::HOUR, dest.hourList, options);

    // day - not setting from server, plz see a CronCommandStringToDateRepeat()
//    GetCronCommandConvertedList(dayStr, ScheduleJson::CronFormat::DAY, dest.dayRepeat, options);

    // month - not setting from server, plz see a CronCommandStringToDateRepeat()
//    GetCronCommandConvertedList(monthStr, ScheduleJson::CronFormat::MONTH, dest.monthRepeat, options);

    // day of week
    GetCronCommandConvertedList(dayOfWeekStr, ScheduleJson::CronFormat::DOW, dest.dowList, options);

    // set options
    dest.options = options;
}

//========================================================
void JsonStringConverter::CronCommandStringToDateRepeat(const QDate& startDate,
                                                        const QDate& endDate,
                                                        ScheduleJson::Cron& dest)
//========================================================
{
    const int startYear = startDate.year();
    const int endYear = endDate.year();

    const int startMonth = startDate.month();
    const int endMonth = endDate.month();

    const int startDay = startDate.day();
    const int endDay = endDate.day();

    // year
    for(int idx = startYear; idx <= endYear; idx++)
    {
        dest.yearList.push_back(idx);
    }

    // month
    if(startMonth > endMonth)
    {
        for(int idx = startMonth; idx <= 12; idx++)
        {
            dest.monthList.push_back(idx);
        }
        for(int idx = 1; idx <= endMonth; idx++)
        {
            dest.monthList.push_back(idx);
        }
    }
    else
    {
        for(int idx = startMonth; idx <= endMonth; idx++)
        {
            dest.monthList.push_back(idx);
        }
    }

    // day
    if(startDay > endDay)
    {
        const int daysInMonth = startDate.daysInMonth();
        for(int idx = startDay; idx <= daysInMonth; idx++)
        {
            dest.dayList.push_back(idx);
        }
        for(int idx = 1; idx <= endDay; idx++)
        {
            dest.dayList.push_back(idx);
        }
    }
    else
    {
        for(int idx = startDay; idx <= endDay; idx++)
        {
            dest.dayList.push_back(idx);
        }
    }
}

//========================================================
void JsonStringConverter::GetCronCommandConvertedList(const QString& src, const ScheduleJson::CronFormat format,
                                                      QVector<int>& dest, ScheduleJson::CronOption& options)
//========================================================
{
    const int limitValue = GetCronFormatLimitValue(format);

    if(src.end() != std::find(src.begin(), src.end() , "*"))
    {
        if(src.end() != std::find(src.begin(), src.end(), "/"))
        {
            const int plusValue = src.split("/")[1].toInt();
            for(int val = 0; val <= limitValue; )
            {
                if(ScheduleJson::CronFormat::DOW == format)
                {
                    const int qtDow = GetConvertedDayOfWeek(val);
                    dest.push_back(qtDow);
                }
                else
                {
                    dest.push_back(val);
                }

                val += plusValue;
            }
        }
        else
        {
            for(int val = 0; val <= limitValue; val++)
            {
                if(ScheduleJson::CronFormat::DOW == format)
                {
                    const int qtDow = GetConvertedDayOfWeek(val);
                    dest.push_back(qtDow);
                }
                else
                {
                    dest.push_back(val);
                }
            }
        }
    }
    else if(src.end() != std::find(src.begin(), src.end(), "/"))
    {
        const QStringList dividedSplit = src.split("/");
        const int startVal = dividedSplit[0].toInt();
        const int plusValue = dividedSplit[1].toInt();

        for(int val = startVal; val <= limitValue; )
        {
            if(ScheduleJson::CronFormat::DOW == format)
            {
                const int qtDow = GetConvertedDayOfWeek(val);
                dest.push_back(qtDow);
            }
            else
            {
                dest.push_back(val);
            }

            val += plusValue;
        }
    }
    else if(src.end() != std::find(src.begin(), src.end(), "-"))
    {
        const QStringList dividedSplit = src.split("-");
        const int startVal = dividedSplit[0].toInt();
        const int endVal = dividedSplit[1].toInt();

        for(int val = startVal; val <= endVal; val++)
        {
            if(limitValue < val)
            {
                break;
            }

            if(ScheduleJson::CronFormat::DOW == format)
            {
                const int qtDow = GetConvertedDayOfWeek(val);
                dest.push_back(qtDow);
            }
            else
            {
                dest.push_back(val);
            }

        }
    }
    else if(src.end() != std::find(src.begin(), src.end(), ","))
    {
        const QStringList dividedSplit = src.split(",");
        foreach(const QString& valStr, dividedSplit)
        {
            const int val = valStr.toInt();

            if(ScheduleJson::CronFormat::DOW == format)
            {
                const int qtDow = GetConvertedDayOfWeek(val);
                dest.push_back(qtDow);
            }
            else
            {
                dest.push_back(val);
            }
        }
    }
    else if( (ScheduleJson::CronFormat::DOW == format) &&
            (src.end() != std::find(src.begin(), src.end(), "#")) )
    {
        const QStringList dividedSplit = src.split("#");
        const int numOfWeek = dividedSplit[1].toInt();

        options.numOfWeek = numOfWeek;
    }
    else if( (ScheduleJson::CronFormat::DAY == format ||
              ScheduleJson::CronFormat::DOW == format) &&
             (src.end() != std::find(src.begin(), src.end(), "L")) )
    {
        options.last.push_back(format);
    }
    else if( (ScheduleJson::CronFormat::DAY == format) &&
             (src.end() != std::find(src.begin(), src.end(), "W")) )
    {
        const QStringList dividedSplit = src.split("W");
        const int weekday = dividedSplit[0].toInt();

        options.weekday = weekday;
    }
    else
    {
        std::stringstream strStream(src.toStdString());
        int number = 0;
        if( false == (strStream >> number).fail())
        {
            if(ScheduleJson::CronFormat::DOW == format)
            {
                const int qtDow = GetConvertedDayOfWeek(src.toInt());
                dest.push_back(qtDow);
            }
            else
            {
                dest.push_back(src.toInt());
            }
        }
        else
        {
            ELGO_CONTROL_LOG("Error - %s rule : %s",
                             ScheduleJson::CronFormatEnumToStr[format].toStdString().c_str(),
                             src.toUtf8().constData());
        }
    }
}

//========================================================
int JsonStringConverter::GetCronFormatLimitValue(const ScheduleJson::CronFormat format)
//========================================================
{
    int retValue = 0;

    if( (ScheduleJson::CronFormat::SEC == format) ||
        (ScheduleJson::CronFormat::MIN == format) )
    {
        retValue = 60;
    }
    else if(ScheduleJson::CronFormat::HOUR == format)
    {
        retValue = 24;
    }
    else if(ScheduleJson::CronFormat::DAY == format)
    {
        retValue = 31;
    }
    else if(ScheduleJson::CronFormat::MONTH == format)
    {
        retValue = 12;
    }
    else if(ScheduleJson::CronFormat::DOW == format)
    {
        /**
         * @note 0 - Sunday, ... , 6 - Saturday
         */
        retValue = 6;
    }
    else
    {
        ELGO_CONTROL_LOG("Error - Unknwon Format : %d", format);
    }

    return retValue;
}

//========================================================
void JsonStringConverter::PrintConvertedCron(const ScheduleJson::Cron& src)
//========================================================
{
    // year
    PrintConvertedCronDataList(ScheduleJson::CronFormat::YEAR, src.yearList);

    // month
    PrintConvertedCronDataList(ScheduleJson::CronFormat::MONTH, src.monthList);

    // day
    PrintConvertedCronDataList(ScheduleJson::CronFormat::DAY, src.dayList);

    // DOW
    PrintConvertedCronDataList(ScheduleJson::CronFormat::DOW, src.dowList);

    // hour
    PrintConvertedCronDataList(ScheduleJson::CronFormat::HOUR, src.hourList);

    // min
    PrintConvertedCronDataList(ScheduleJson::CronFormat::MIN, src.minList);

    // sec - not use
}

//========================================================
void JsonStringConverter::PrintConvertedCronDataList(const ScheduleJson::CronFormat type, const QVector<int>& src)
//========================================================
{
    // print
    QString printStr = QString("%1 List - [ ").arg(ScheduleJson::CronFormatEnumToStr[type]);
    const int listSize = src.size();
    for(int idx = 0; idx < listSize; idx++)
    {
        printStr.append(QString::number(src[idx]));
        if((listSize - 1) != idx)
        {
            printStr.append(", ");
        }
    }
    printStr.append(" ]");
    ELGO_CONTROL_LOG("%s", printStr.toStdString().c_str());
}

//========================================================
int JsonStringConverter::GetConvertedDayOfWeek(const int cronDow)
//========================================================
{
    int retValue = -1;

    if(0 == cronDow)
    {
        // cron - sunday
        retValue = 7;
    }
    else
    {
        retValue = cronDow;
    }

    return retValue;
}

//========================================================
void JsonStringConverter::GetSchedulePlayDataIdInName(const QString& src, QString& playDataId,
                                                      QString& playDataType)
//========================================================
{
    QStringList nameSplit = src.split("_");
    if(2 < nameSplit.size())
    {
        playDataId = nameSplit[1];
        playDataType = nameSplit[2];
    }
    else
    {
        ELGO_CONTROL_LOG("Error - %s, split size : %d", src.toUtf8().constData(), nameSplit.size());
    }
}
