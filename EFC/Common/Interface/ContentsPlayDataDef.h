#ifndef CONETNSPLAYDATADEF_H
#define CONETNSPLAYDATADEF_H

// QT
#include <QString>
#include <QVector>
#include <QMetaType>

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
        PlayData()
            : id(0)
            , width(0)
            , height(0)
            , orientation(Orientation::NONE_ORIENTATION)
            , playDataType(PlayDataType::NONE_PLAY_DATA_TYPE)
        {}
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

    /** @note
     *          JSON data which made by server is not included SUBTITLE type.
     *          Just using in ContentsPlayer.h,.cpp.
     */
    enum MediaType
    {
        NONE_MEDIA = 0,
        IMAGE = 1,
        VIDEO = 2,
        CLOCK = 3,
        DATE = 4,
        WEATHER = 5,
        NEWS = 6,
        SUBTITLE = 7,
    };

    /** @brief */
    enum DateType
    {
        NONE_DATE_TYPE = 0,
        YES = 1,
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

    static const QString newscategoryEnumToStr[] = { "none", "latest", "headline", "politics",
                                                    "economy", "society", "local",
                                                    "international", "culture", "sports", "weather"};

    /** @brief */
    struct ContentInfo
    {
        ContentType contentType;
        MediaType mediaType;
    };

    /** @brief  Weather Widget */
    enum PTY
    {
        NO_RAIN = 0,
        RAIN = 1,
        SLEET = 2,
        SNOW = 3,
        SHOWER = 4,
        RAIN_DROP = 5,
        RAIN_DROP_SNOW = 6,
        SNOW_DRIFT = 7
    };

    /** @brief */
    enum SKY
    {
        SUNNY = 1,
        MUCH_CLOUDY = 2,
        CLOUDY = 3,
        GRAY = 4,
    };


    /** @brief */
    struct ContentData
    {
        ContentData()
            : fileDuration(0)
            , userDuration(0)
            , zIndex(0.0)
            , bBackgroundOpacity(false)
            , nx(0)
            , ny(0)
            , PTY(NO_RAIN)
            , SKY(SUNNY)
            , LGT(false)
            , newsCategory(NewsCategory::NONE_NEWS_CATEGORY)
            , newsCount(0)
            , newsfontSize(0)
            , bNewsBoxOpacity(false)
        {
        }
        ContentInfo contentInfo;
        QString name;
        qint64 fileDuration; // msec
        int userDuration; // sec
        double zIndex;
        QString fontColor;
        QString backgroundColor;
        bool bBackgroundOpacity;

        // weather
        /** @note   below Area info will be replace nx, ny */
        QString metropolCity; // area
        QString metropolCityName;
        QString city; // area2
        QString cityName;
        int nx;
        int ny;

        /**
         *  @note
         *          pty - precipitation form
         *          sky - sky condition
         *          t1h - temperature
         *          rn1 - rainfall (1 hour)
         *          reh - humidity
         *          vec - direction of the wind
         *          wsd - wind speed
         *          lgt - falling of a thunderbolt
         */
        PlayJson::PTY PTY;
        PlayJson::SKY SKY;
        QString T1H;
        int RN1;
        int REH;
        QString VEC;
        QString WSD;
        bool LGT;

        // clock
        HourType hourType;

        // date
        DateType dateType;

        // news
        NewsCategory newsCategory;
        int newsCount;
        QVector<QString> newsFeedList;
        int newsfontSize;
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
    enum AniFlowDirection
    {
        NONE_DIRECTION = 0,
        LEFT_TO_RIGHT = 1,
        RIGHT_TO_LEFT = 2,
        TOP_TO_BOTTOM = 3,
        BOTTOM_TO_TOP = 4,
    };

    /** @brief  Behavior on Server JSON */
    enum SubtitleAction
    {
        NONE_ACTION = 0,
        SCROLL = 1,
        LOOP = 2,
    };

    /** @brief */
    struct SubtitleData
    {
        SubtitleData()
            : left(0)
            , top(0)
            , width(0)
            , height(0)
            , fontSize(0)
            , bBackgroundOpacity(false)
            , bIsFixed(true)
            , direction(AniFlowDirection::NONE_DIRECTION)
            , orientation(Orientation::NONE_ORIENTATION)
            , action(SubtitleAction::NONE_ACTION)
            , speed(0)
        {

        }
        QString text;

        int left;
        int top;
        int width;
        int height;

        QString backgroundColor;
        QString fontColor;
        int fontSize;
        bool bBackgroundOpacity;

        bool bIsFixed;
        AniFlowDirection direction;
        Orientation orientation;
        SubtitleAction action;
        int speed;
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


    /** @brief */
    struct UpdateWidgetInfo
    {
       UpdateWidgetInfo()
           : playDataId(0)
           , playType(PlayJson::PlayDataType::NONE_PLAY_DATA_TYPE)
           , mediaType(PlayJson::MediaType::NONE_MEDIA)
           , pageIndex(-1)
           , layerIndex(-1)
           , contentIndex(-1)
           , newsCategory(PlayJson::NewsCategory::NONE_NEWS_CATEGORY)
           , newsCount(0)
           , nx(0)
           , ny(0)
           , PTY(PlayJson::PTY::NO_RAIN)
           , SKY(PlayJson::SKY::SUNNY)
           , LGT(false)
       {
       }

       int playDataId;
       PlayJson::PlayDataType playType;
       PlayJson::MediaType mediaType;

       // Custom
       int pageIndex;

       // Custom / Fixed
       int layerIndex;

       // Fixed
       int contentIndex;

       // news
       PlayJson::NewsCategory newsCategory;
       int newsCount;
       QVector<QString> newsFeedList;

       /**
        *  @note
        *          pty - precipitation form
        *          sky - sky condition
        *          t1h - temperature
        *          rn1 - rainfall (1 hour)
        *          reh - humidity
        *          vec - direction of the wind
        *          wsd - wind speed
        *          lgt - falling of a thunderbolt
        */

       // weather
       int nx;
       int ny;

       PlayJson::PTY PTY;
       PlayJson::SKY SKY;
       QString T1H;
       int RN1;
       int REH;
       QString VEC;
       QString WSD;
       bool LGT;
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

    /** @brief */
    bool operator==(const PlayJson::UpdateWidgetInfo& lhs, const PlayJson::UpdateWidgetInfo& rhs);
}

#endif // CONETNSPLAYDATADEF_H
