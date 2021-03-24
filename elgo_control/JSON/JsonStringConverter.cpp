
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
    else if(0 == strcmp("singlePlay", src.toStdString().c_str()))
    {
        retValue = ContentSchema::Event::SINGLE_PLAY;
    }
    else if(0 == strcmp("playSchedules", src.toStdString().c_str()))
    {
        retValue = ContentSchema::Event::SINGLE_PLAY;
    }
    else if(0 == strcmp("powerSchedules", src.toStdString().c_str()))
    {
        retValue = ContentSchema::Event::POWER_SCHEDULES;
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
ContentSchema::ResourceType JsonStringConverter::ResourceTypeStringToEnum(const QString& src)
//========================================================
{
    ContentSchema::ResourceType retValue = ContentSchema::ResourceType::NONE_RSRC;

    if(0 == strcmp("js", src.toStdString().c_str()))
    {
        retValue = ContentSchema::ResourceType::JS;
    }
    else if(0 == strcmp("icon", src.toStdString().c_str()))
    {
        retValue = ContentSchema::ResourceType::ICON;
    }
    else if(0 == strcmp("data", src.toStdString().c_str()))
    {
        retValue = ContentSchema::ResourceType::DATA;
    }
    else if(0 == strcmp("page", src.toStdString().c_str()))
    {
        retValue = ContentSchema::ResourceType::PAGE;
    }
    else if(0 == strcmp("image", src.toStdString().c_str()))
    {
        retValue = ContentSchema::ResourceType::IMAGE;
    }
    else
    {
        ELGO_CONTROL_LOG("Error - Unkwon resource type : %s", src.toUtf8().constData());
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
    else if(ContentSchema::Event::ERROR == event)
    {
        dest = "error";
    }
    else if(ContentSchema::Event::READY == event)
    {
        dest = "ready";
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
void JsonStringConverter::ResourceTypeEnumToString(const ContentSchema::ResourceType type, QString& dest)
//========================================================
{
    if(ContentSchema::ResourceType::JS == type)
    {
        dest = "js";
    }
    else if(ContentSchema::ResourceType::ICON == type)
    {
        dest = "icon";
    }
    else if(ContentSchema::ResourceType::DATA == type)
    {
        dest = "data";
    }
    else if(ContentSchema::ResourceType::PAGE == type)
    {
        dest = "page";
    }
    else if(ContentSchema::ResourceType::IMAGE == type)
    {
        dest = "image";
    }
    else
    {
        dest = "none";
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
void JsonStringConverter::CronCommandStringToStruct(const QString& src, ContentSchema::Cron& dest)
//========================================================
{
    ContentSchema::CronOption options;

    QStringList cronStrSplit = src.split(" ");
    QString secStr = cronStrSplit[0];
    QString minStr = cronStrSplit[1];
    QString hourStr = cronStrSplit[2];

    QString dayStr = cronStrSplit[3];
    QString monthStr = cronStrSplit[4];
    QString dayOfWeekStr = cronStrSplit[5];

    /// time
    // sec
    GetCronCommandConvertedList(secStr, ContentSchema::CronFormat::e_SEC, dest.secRepeat, options);

    // min
    GetCronCommandConvertedList(minStr, ContentSchema::CronFormat::e_MIN, dest.minRepeat, options);

    // hour()
    GetCronCommandConvertedList(hourStr, ContentSchema::CronFormat::e_HOUR, dest.hourRepeat, options);

    // day
    GetCronCommandConvertedList(dayStr, ContentSchema::CronFormat::e_DAY, dest.dayRepeat, options);

    // month
    GetCronCommandConvertedList(monthStr, ContentSchema::CronFormat::e_MONTH, dest.monthRepeat, options);

    // day of week
    GetCronCommandConvertedList(dayOfWeekStr, ContentSchema::CronFormat::e_DOW, dest.dowRepeat, options);

    // set options
    dest.options = options;
}

//========================================================
void JsonStringConverter::GetCronCommandConvertedList(const QString& src, const ContentSchema::CronFormat format,
                                                QList<int>& dest, ContentSchema::CronOption& options)
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
                if(ContentSchema::CronFormat::e_DOW == format)
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
                if(ContentSchema::CronFormat::e_DOW == format)
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
            if(ContentSchema::CronFormat::e_DOW == format)
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

            if(ContentSchema::CronFormat::e_DOW == format)
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

            if(ContentSchema::CronFormat::e_DOW == format)
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
    else if( (ContentSchema::CronFormat::e_DOW == format) &&
            (src.end() != std::find(src.begin(), src.end(), "#")) )
    {
        const QStringList dividedSplit = src.split("#");
        const int numOfWeek = dividedSplit[1].toInt();

        options.numOfWeek = numOfWeek;
    }
    else if( (ContentSchema::CronFormat::e_DAY == format ||
              ContentSchema::CronFormat::e_DOW == format) &&
             (src.end() != std::find(src.begin(), src.end(), "L")) )
    {
        options.last.push_back(format);
    }
    else if( (ContentSchema::CronFormat::e_DAY == format) &&
             (src.end() != std::find(src.begin(), src.end(), "W")) )
    {
        const QStringList dividedSplit = src.split("W");
        const int weekday = dividedSplit[0].toInt();

        options.weekday = weekday;
    }
    else
    {
        ELGO_CONTROL_LOG("Error - sec rule : %s", src.toUtf8().constData());
    }
}

//========================================================
int JsonStringConverter::GetCronFormatLimitValue(const ContentSchema::CronFormat format)
//========================================================
{
    int retValue = 0;

    if(ContentSchema::CronFormat::e_SEC == format ||
        ContentSchema::CronFormat::e_MIN == format)
    {
        retValue = 60;
    }
    else if(ContentSchema::CronFormat::e_HOUR == format)
    {
        retValue = 24;
    }
    else if(ContentSchema::CronFormat::e_DAY == format)
    {
        retValue = 31;
    }
    else if(ContentSchema::CronFormat::e_MONTH == format)
    {
        retValue = 12;
    }
    else if(ContentSchema::CronFormat::e_DOW == format)
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
