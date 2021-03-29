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
        PLAY_SCHEDULES = 5,
        POWER_SCHEDULES = 6,
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

#if 0
namespace ScheduleJson
{
    // Schedule
    /** @brief */
    enum CronFormat
    {
        SEC = 1,
        MIN = 2,
        HOUR = 3,
        DAY = 4,
        MONTH = 5,
        DOW = 6
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
#endif

#if 0
namespace ObjectJson
{
    // Object
    /** @brief */
    enum Orientation
    {
        NONE_ORIENTATION = 0,
        HORIZONTAL = 1,
        VERTICAL = 2
    };

    /** @brief */
    enum PlayDataType
    {
        NONE_PLAY_DATA_TYPE = 0,
        CUSTOM = 1,
        FIXED = 2
    };

    /** @brief */
    struct PlayData
    {
        int id;
        QString name;
        QString memo;
        int width;
        int height;
        Orientation orientation;
        PlayDataType playDataType;
    };

    /** @brief */
    enum HourType
    {
        NONE_HOUR_TYPE = 0,
        HOUR_12 = 1,
        HOUR_24 = 2,
    };

    /** @brief */
    enum ContentType
    {
        NONE_CONTENT = 0,
        FILE = 1,
        WIDGET = 2,
    };

    /** @brief */
    enum MediaType
    {
        NONE_MEDIA = 0,
        IMAGE = 1,
        VIDEO = 2,
        CLOCK = 3,
        DATE = 4,
        WEATHER = 5,
        NEWS = 6,
    };

    /** @brief */
    enum DateType
    {
        NONE_DATE_TYPE = 0,
        YES =1,
        NO = 2
    };

    enum NewsCategory
    {
        NONE_NEWS_CATEGORY = 0,
        LATEST = 1,
        HEADLINE = 2,
        POLITICS = 3,
        ECONOMY = 4,
        SOCIETY = 5,
        LOCAL = 6,
        GLBOAL = 7,
        CULTURE = 8,
        SPORT = 9,
        NEWS_WEATHER = 10,
    };

    /** @brief */
    struct LayerContentType
    {
        ContentType contentType;
        MediaType mediaType;
    };

    /** @brief */
    struct LayerContent
    {
        LayerContentType contentType;
        QString name;
        qint64 duration;

        // weather
        /** @note   below Area info will be replace nx, ny */
        QString metropolCity; // area
        QString metropolCityName;
        QString city; // area2
        QString cityName;
        QString fontColor;
        QString backgroundColor;
        bool bBackgroundOpacity;
        int nx;
        int ny;

        // clock
        HourType hourType;

        // date
        DateType dateType;

        // news
        NewsCategory newsCategory;
        int newsfontSize;
        int newsBoxCount;
        QString newsBoxColor;
        bool bNewsBoxOpacity;
    };

    /** @brief */
    struct LayerData
    {
        int left;
        int top;
        int width;
        int height;
        LayerContent layContent;
    };

    /** @brief */
    enum SubtitleDirection
    {
        NONE_DIRECTION = 0,
        LEFT_TO_RIGHT = 1,
        RIGHT_TO_LEFT = 2,
        TOP_TO_BOTTOM = 3,
        BOTTOM_TO_TOP = 4,
        FIXED_SUBTITLE = 5,
    };

    /** @brief */
    enum SubtitleAction
    {
        NONE_ACTION = 0,
        SCROLL = 1,
        LOOP = 2,
    };

    /** @brief */
    struct SubtitleData
    {
        int left;
        int top;
        int width;
        int height;
        QString text;
        bool bIsFixed;
        SubtitleDirection direction;
        Orientation orientation;
        SubtitleAction action;
        int speed;
        QString backgroundColor;
        QString fontColor;
        int fontSize;
    };

    /** @brief */
    struct PageData
    {
        int duration;
        QList<LayerData> layerDataList;
        QList<SubtitleData> subtitleDataList;
    };

    /** @brief  Custom Play */
    struct CustomPlayDataJson
    {
        PlayData playData;
        QList<PageData> pageDataList;
    };

    /** @brief  Fixed Play */
    struct FixedPlayDataJson
    {
        PlayData playData;
        QList<LayerData> layerDataList;
        QList<SubtitleData> subtitleDataList;
    };
}
#endif

#endif // CONTENTSERVERSCHEMA_H
