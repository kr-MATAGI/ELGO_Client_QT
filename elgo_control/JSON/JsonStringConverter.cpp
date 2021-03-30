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
        retValue = ContentSchema::Event::PLAY_SCHEDULES;
    }
    else if(0 == strcmp("powerSchedule", src.toStdString().c_str()))
    {
        retValue = ContentSchema::Event::POWER_SCHEDULES;
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
        ELGO_CONTROL_LOG("Error - Unkwon Event %s", src.toUtf8().constData());
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
        ELGO_CONTROL_LOG("Error - Unkwon payload type : %s", src.toUtf8().constData());
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
        ELGO_CONTROL_LOG("Error - Unkwon resource type : %s", src.toUtf8().constData());
    }

    return retValue;
}

//========================================================
ObjectJson::PlayDataType JsonStringConverter::PlayDataTypeStringToEnum(const QString& src)
//========================================================
{
    ObjectJson::PlayDataType retValue = ObjectJson::PlayDataType::NONE_PLAY_DATA_TYPE;

    if(0 == strcmp("custom", src.toStdString().c_str()))
    {
        retValue = ObjectJson::PlayDataType::CUSTOM;
    }
    else if(0 == strcmp("fixed", src.toStdString().c_str()))
    {
        retValue = ObjectJson::PlayDataType::FIXED;
    }
    else
    {
        ELGO_CONTROL_LOG("Error - Unkwon playdata type : %s", src.toUtf8().constData());
    }

    return retValue;
}

//========================================================
ObjectJson::Orientation JsonStringConverter::OrientationTypeStringToEnum(const QString& src)
//========================================================
{
    ObjectJson::Orientation retValue = ObjectJson::Orientation::NONE_ORIENTATION;

    if(0 == strcmp("landscape", src.toStdString().c_str()))
    {
        retValue = ObjectJson::Orientation::HORIZONTAL;
    }
    else if(0 == strcmp("portrait", src.toStdString().c_str()))
    {
        retValue = ObjectJson::Orientation::VERTICAL;
    }
    else
    {
        ELGO_CONTROL_LOG("Error - Unkwon orientation type : %s", src.toUtf8().constData());
    }

    return retValue;
}

//========================================================
ObjectJson::ContentType JsonStringConverter::ContentTypeStringToEnum(const QString& src)
//========================================================
{
    ObjectJson::ContentType retValue = ObjectJson::ContentType::NONE_CONTENT;

    if(0 == strcmp("file", src.toStdString().c_str()))
    {
        retValue = ObjectJson::ContentType::FILE;
    }
    else if(0 == strcmp("widget", src.toStdString().c_str()))
    {
        retValue = ObjectJson::ContentType::WIDGET;
    }
    else
    {
        ELGO_CONTROL_LOG("Error - Unkwon content type : %s", src.toUtf8().constData());
    }

    return retValue;
}

//========================================================
ObjectJson::MediaType JsonStringConverter::MediaTypeStringToEunum(const QString& src)
//========================================================
{
    ObjectJson::MediaType retValue = ObjectJson::MediaType::NONE_MEDIA;

    if(0 == strcmp("image", src.toStdString().c_str()))
    {
        retValue = ObjectJson::MediaType::IMAGE;
    }
    else if(0 == strcmp("video", src.toStdString().c_str()))
    {
        retValue = ObjectJson::MediaType::VIDEO;
    }
    else if(0 == strcmp("clock", src.toStdString().c_str()))
    {
        retValue = ObjectJson::MediaType::CLOCK;
    }
    else if(0 == strcmp("date", src.toStdString().c_str()))
    {
        retValue = ObjectJson::MediaType::DATE;
    }
    else if(0 == strcmp("weather", src.toStdString().c_str()))
    {
        retValue = ObjectJson::MediaType::WEATHER;
    }
    else if(0 == strcmp("news", src.toStdString().c_str()))
    {
        retValue = ObjectJson::MediaType::NEWS;
    }
    else
    {
        ELGO_CONTROL_LOG("Error - Unkwon media type : %s", src.toUtf8().constData());
    }

    return retValue;
}

//========================================================
ObjectJson::HourType JsonStringConverter::HourTypeStringToEnum(const QString& src)
//========================================================
{
    ObjectJson::HourType retValue = ObjectJson::HourType::NONE_HOUR_TYPE;

    if(0 == strcmp("12h", src.toStdString().c_str()))
    {
        retValue = ObjectJson::HourType::HOUR_12;
    }
    else if(0 == strcmp("24h", src.toStdString().c_str()))
    {
        retValue = ObjectJson::HourType::HOUR_24;
    }
    else
    {
        ELGO_CONTROL_LOG("Error - Unkwon hour type : %s", src.toUtf8().constData());
    }

    return retValue;
}

//========================================================
ObjectJson::DateType JsonStringConverter::DateTypeStringToEnum(const QString& src)
//========================================================
{
    ObjectJson::DateType retValue = ObjectJson::DateType::NONE_DATE_TYPE;

    if(0 == strcmp("yes", src.toStdString().c_str()))
    {
        retValue = ObjectJson::DateType::YES;
    }
    else if(0 == strcmp("no", src.toStdString().c_str()))
    {
        retValue = ObjectJson::DateType::NO;
    }
    else
    {
        ELGO_CONTROL_LOG("Error - Unkwon date type : %s", src.toUtf8().constData());
    }

    return retValue;
}

//========================================================
ObjectJson::NewsCategory JsonStringConverter::NewsCategoryStringToEnum(const QString& src)
//========================================================
{
    ObjectJson::NewsCategory retValue = ObjectJson::NewsCategory::NONE_NEWS_CATEGORY;

    if(0 == strcmp("browse", src.toStdString().c_str()))
    {
        retValue = ObjectJson::NewsCategory::LATEST;
    }
    else if(0 == strcmp("headline", src.toStdString().c_str()))
    {
        retValue = ObjectJson::NewsCategory::HEADLINE;
    }
    else if(0 == strcmp("politics", src.toStdString().c_str()))
    {
        retValue = ObjectJson::NewsCategory::POLITICS;
    }
    else if(0 == strcmp("economy", src.toStdString().c_str()))
    {
        retValue = ObjectJson::NewsCategory::ECONOMY;
    }
    else if(0 == strcmp("society", src.toStdString().c_str()))
    {
        retValue = ObjectJson::NewsCategory::SOCIETY;
    }
    else if(0 == strcmp("local", src.toStdString().c_str()))
    {
        retValue = ObjectJson::NewsCategory::LOCAL;
    }
    else if(0 == strcmp("international", src.toStdString().c_str()))
    {
        retValue = ObjectJson::NewsCategory::GLBOAL;
    }
    else if(0 == strcmp("culture", src.toStdString().c_str()))
    {
        retValue = ObjectJson::NewsCategory::CULTURE;
    }
    else if(0 == strcmp("sports", src.toStdString().c_str()))
    {
        retValue = ObjectJson::NewsCategory::SPORT;
    }
    else if(0 == strcmp("weather", src.toStdString().c_str()))
    {
        retValue = ObjectJson::NewsCategory::NEWS_WEATHER;
    }
    else
    {
        ELGO_CONTROL_LOG("Error - Unkwon news category type : %s", src.toUtf8().constData());
    }

    return retValue;
}

//========================================================
ObjectJson::SubtitleDirection JsonStringConverter::SubtitleDirectionStringToEnum(const QString& src)
//========================================================
{
    ObjectJson::SubtitleDirection retValue = ObjectJson::SubtitleDirection::NONE_DIRECTION;

    if(0 == strcmp("null", src.toStdString().c_str()))
    {
        retValue = ObjectJson::SubtitleDirection::FIXED_SUBTITLE;
    }
    else if(0 == strcmp("left", src.toStdString().c_str()))
    {
        retValue = ObjectJson::SubtitleDirection::LEFT_TO_RIGHT;
    }
    else if(0 == strcmp("right", src.toStdString().c_str()))
    {
        retValue = ObjectJson::SubtitleDirection::RIGHT_TO_LEFT;
    }
    else if(0 == strcmp("up", src.toStdString().c_str()))
    {
        retValue = ObjectJson::SubtitleDirection::BOTTOM_TO_TOP;
    }
    else if(0 == strcmp("down", src.toStdString().c_str()))
    {
        retValue = ObjectJson::SubtitleDirection::TOP_TO_BOTTOM;
    }
    else
    {
        ELGO_CONTROL_LOG("Error - Unkwon subtitle direction : %s", src.toUtf8().constData());
    }

    return retValue;
}

//========================================================
ObjectJson::SubtitleAction JsonStringConverter::SubtitleActionStringToEnum(const QString& src)
//========================================================
{
    ObjectJson::SubtitleAction retValue = ObjectJson::SubtitleAction::NONE_ACTION;

    if(0 == strcmp("scroll", src.toStdString().c_str()))
    {
        retValue = ObjectJson::SubtitleAction::SCROLL;
    }
    else if(0 == strcmp("alternate", src.toStdString().c_str()))
    {
        retValue = ObjectJson::SubtitleAction::LOOP;
    }
    else
    {
        ELGO_CONTROL_LOG("Error - Unkwon subtitle action : %s", src.toUtf8().constData());
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
    else if(ContentSchema::Event::PLAY_SCHEDULES == event)
    {
        dest = "playSchedule";
    }
    else if(ContentSchema::Event::POWER_SCHEDULES == event)
    {
        dest = "powerSchedule";
    }
    else if(ContentSchema::Event::CLEAR_PLAY_SCHEDULE == event)
    {
        dest = "clearPlaySchedule";
    }
    else if(ContentSchema::Event::POWER_SCHEDULES == event)
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
void JsonStringConverter::PlayDataTypeEnumToString(const ObjectJson::PlayDataType type, QString& dest)
//========================================================
{
    if(ObjectJson::PlayDataType::CUSTOM == type)
    {
        dest = "custom";
    }
    else if(ObjectJson::PlayDataType::FIXED == type)
    {
        dest = "fixed";
    }
    else
    {
        dest = "none";
    }
}

//========================================================
void JsonStringConverter::OrientationTypeEnumToString(const ObjectJson::Orientation type, QString& dest)
//========================================================
{
    if(ObjectJson::Orientation::HORIZONTAL == type)
    {
        dest = "landscape";
    }
    else if(ObjectJson::Orientation::VERTICAL == type)
    {
        dest = "portrait";
    }
    else
    {
        dest = "none";
    }
}

//========================================================
void JsonStringConverter::ContentTypeEnumToString(const ObjectJson::ContentType type, QString& dest)
//========================================================
{
    if(ObjectJson::ContentType::FILE == type)
    {
        dest = "file";
    }
    else if(ObjectJson::ContentType::WIDGET == type)
    {
        dest = "widget";
    }
    else
    {
        dest = "none";
    }
}

//========================================================
void JsonStringConverter::MediaTypeEnumToString(const ObjectJson::MediaType type, QString& dest)
//========================================================
{
    if(ObjectJson::MediaType::IMAGE == type)
    {
        dest = "image";
    }
    else if(ObjectJson::MediaType::VIDEO == type)
    {
        dest = "video";
    }
    else if(ObjectJson::MediaType::CLOCK == type)
    {
        dest = "clock";
    }
    else if(ObjectJson::MediaType::DATE == type)
    {
        dest = "date";
    }
    else if(ObjectJson::MediaType::WEATHER == type)
    {
        dest = "weather";
    }
    else if(ObjectJson::MediaType::NEWS == type)
    {
        dest = "news";
    }
    else
    {
        dest = "none";
    }
}

//========================================================
void JsonStringConverter::HourTypeEnumToString(const ObjectJson::HourType type, QString& dest)
//========================================================
{
    if(ObjectJson::HourType::HOUR_12 == type)
    {
        dest = "12h";
    }
    else if(ObjectJson::HourType::HOUR_24 == type)
    {
        dest = "24h";
    }
    else
    {
        dest = "none";
    }
}

//========================================================
void JsonStringConverter::DateTypeEnumToString(const ObjectJson::DateType type, QString& dest)
//========================================================
{
    if(ObjectJson::DateType::YES == type)
    {
        dest = "yes";
    }
    else if(ObjectJson::DateType::NO == type)
    {
        dest = "no";
    }
    else
    {
        dest = "none";
    }
}

//========================================================
void JsonStringConverter::NewsCategoryEnumToString(const ObjectJson::NewsCategory type, QString& dest)
//========================================================
{
    if(ObjectJson::NewsCategory::LATEST == type)
    {
        dest = "browse";
    }
    else if(ObjectJson::NewsCategory::HEADLINE == type)
    {
        dest = "headline";
    }
    else if(ObjectJson::NewsCategory::POLITICS == type)
    {
        dest = "politics";
    }
    else if(ObjectJson::NewsCategory::ECONOMY == type)
    {
        dest = "economy";
    }
    else if(ObjectJson::NewsCategory::SOCIETY == type)
    {
        dest = "society";
    }
    else if(ObjectJson::NewsCategory::LOCAL == type)
    {
        dest = "local";
    }
    else if(ObjectJson::NewsCategory::GLBOAL == type)
    {
        dest = "international";
    }
    else if(ObjectJson::NewsCategory::CULTURE == type)
    {
        dest = "culture";
    }
    else if(ObjectJson::NewsCategory::SPORT == type)
    {
        dest = "sports";
    }
    else if(ObjectJson::NewsCategory::NEWS_WEATHER == type)
    {
        dest = "weather";
    }
}

//========================================================
void JsonStringConverter::SubtitleDirectionEnumToString(const ObjectJson::SubtitleDirection type, QString& dest)
//========================================================
{
    if(ObjectJson::SubtitleDirection::FIXED_SUBTITLE == type)
    {
        dest = "fixed";
    }
    else if(ObjectJson::SubtitleDirection::LEFT_TO_RIGHT == type)
    {
        dest = "left";
    }
    else if(ObjectJson::SubtitleDirection::RIGHT_TO_LEFT == type)
    {
        dest = "right";
    }
    else if(ObjectJson::SubtitleDirection::TOP_TO_BOTTOM == type)
    {
        dest = "down";
    }
    else if(ObjectJson::SubtitleDirection::BOTTOM_TO_TOP == type)
    {
        dest = "up";
    }
    else
    {
        dest = "null";
    }
}

//========================================================
void JsonStringConverter::SubtitleActionEnumToString(const ObjectJson::SubtitleAction type, QString& dest)
//========================================================
{
    if(ObjectJson::SubtitleAction::SCROLL == type)
    {
        dest = "scroll";
    }
    else if(ObjectJson::SubtitleAction::LOOP == type)
    {
        dest = "alternate";
    }
    else
    {
        dest = "null";
    }
}

//========================================================
void JsonStringConverter::ScheduleDateTimeStringToQDateTime(const QString& src, QDateTime &dest)
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
    const int hour = timeSplit[0].toInt();
    const int min = timeSplit[1].toInt();

    const QString secMsecStr = timeSplit[2];
    const QStringList secMsecSplit = secMsecStr.split(".");
    const int sec = secMsecSplit[0].toInt();
    const int msec = secMsecSplit[1].split("Z")[0].toInt();
    time.setHMS(hour, min, sec, msec);

    dest.setTime(time);
    dest.setDate(date);
}

//========================================================
void JsonStringConverter::CronCommandStringToStruct(const QString& src, ScheduleJson::Cron& dest)
//========================================================
{
    ScheduleJson::CronOption options;

    QStringList cronStrSplit = src.split(" ");
    QString secStr = cronStrSplit[0];
    QString minStr = cronStrSplit[1];
    QString hourStr = cronStrSplit[2];

    QString dayStr = cronStrSplit[3];
    QString monthStr = cronStrSplit[4];
    QString dayOfWeekStr = cronStrSplit[5];

    /// time
    // sec
    GetCronCommandConvertedList(secStr, ScheduleJson::CronFormat::SEC, dest.secRepeat, options);

    // min
    GetCronCommandConvertedList(minStr, ScheduleJson::CronFormat::MIN, dest.minRepeat, options);

    // hour()
    GetCronCommandConvertedList(hourStr, ScheduleJson::CronFormat::HOUR, dest.hourRepeat, options);

    // day
    GetCronCommandConvertedList(dayStr, ScheduleJson::CronFormat::DAY, dest.dayRepeat, options);

    // month
    GetCronCommandConvertedList(monthStr, ScheduleJson::CronFormat::MONTH, dest.monthRepeat, options);

    // day of week
    GetCronCommandConvertedList(dayOfWeekStr, ScheduleJson::CronFormat::DOW, dest.dowRepeat, options);

    // set options
    dest.options = options;
}

//========================================================
void JsonStringConverter::GetCronCommandConvertedList(const QString& src, const ScheduleJson::CronFormat format,
                                                QList<int>& dest, ScheduleJson::CronOption& options)
//========================================================
{
    const int limitValue = GetCronFormatLimitValue(format);

    if(src.end() != std::find(src.begin(), src.end() , "*"))
    {
        if(src.end() != std::find(src.begin(), src.end(), "/"))
        {
            const int plusValue = src.split("/")[1].toInt();
            for(int val = 0; val < limitValue; )
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
                             ScheduleJson::CronFormatEnumToStr[format], src.toUtf8().constData());
        }
    }
}

//========================================================
int JsonStringConverter::GetCronFormatLimitValue(const ScheduleJson::CronFormat format)
//========================================================
{
    int retValue = 0;

    if(ScheduleJson::CronFormat::SEC == format ||
        ScheduleJson::CronFormat::MIN == format)
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
        ELGO_CONTROL_LOG("Error - Unkwon Format : %d", format);
    }

    return retValue;
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
void JsonStringConverter::GetSchedulePlayDataIdInName(const QString& src, QString& dest)
//========================================================
{
    QStringList nameSplit = src.split("_");
    if(2 < nameSplit.size())
    {
        dest = nameSplit[1];
    }
    else
    {
        ELGO_CONTROL_LOG("Error - %s, split size : %d", src.toUtf8().constData(), nameSplit.size());
    }
}
