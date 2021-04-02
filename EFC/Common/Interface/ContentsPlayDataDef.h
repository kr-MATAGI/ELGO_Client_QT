#ifndef CONETNSPLAYDATADEF_H
#define CONETNSPLAYDATADEF_H

#include <QString>
#include <QVector>

namespace PlayJson
{
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
    struct ContentInfo
    {
        ContentType contentType;
        MediaType mediaType;
    };

    /** @brief */
    struct ContentData
    {
        ContentData()
            : fileDuration(0)
            , userDuration(0)
            , bBackgroundOpacity(false)
            , nx(0)
            , ny(0)
            , newsfontSize(0)
            , newsBoxCount(0)
            , bNewsBoxOpacity(false)
        {
        }
        ContentInfo contentInfo;
        QString name;
        qint64 fileDuration;
        qint64 userDuration;

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
    struct CustomLayerData
    {
        int left;
        int top;
        int width;
        int height;
        ContentData layerContent;
    };

    /** @brief */
    struct FixedLayerData
    {
        int left;
        int top;
        int width;
        int height;
        QVector<ContentData> contentDataList;
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
        qint64 duration;
        QVector<CustomLayerData> layerDataList;
        QVector<SubtitleData> subtitleDataList;
    };

    /** @brief  Custom Play */
    struct CustomPlayDataJson
    {
        PlayData playData;
        QVector<PageData> pageDataList;
    };

    /** @brief  Fixed Play */
    struct FixedPlayDataJson
    {
        PlayData playData;
        QVector<FixedLayerData> layerDataList;
        QVector<SubtitleData> subtitleDataList;
    };


    /**
     *  @note   operator ==
     */
    /** @brief */
    bool operator==(const PlayJson::FixedPlayDataJson& lhs, const PlayJson::FixedPlayDataJson& rhs);
    /** @brief */
    bool operator==(const PlayJson::CustomPlayDataJson& lhs, const PlayJson::CustomPlayDataJson& rhs);

    /** @brief */
    bool operator==(const PlayJson::PlayData& lhs, const PlayJson::PlayData& rhs);
    /** @brief */
    bool operator==(const PlayJson::FixedLayerData& lhs, const PlayJson::FixedLayerData& rhs);
    /** @brief */
    bool operator==(const PlayJson::ContentData& lhs, const PlayJson::ContentData& rhs);
    /** @brief */
    bool operator==(const PlayJson::ContentInfo& lhs, const PlayJson::ContentInfo& rhs);
    /** @brief */
    bool operator==(const PlayJson::SubtitleData& lhs, const PlayJson::SubtitleData& rhs);
    /** @brief */
    bool operator==(const PlayJson::PageData& lhs, const PlayJson::PageData& rhs);
    /** @brief */
    bool operator==(const PlayJson::CustomLayerData& lhs, const PlayJson::CustomLayerData& rhs);
}

#endif // CONETNSPLAYDATADEF_H
