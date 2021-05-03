#ifndef SCHEDULERDEF_H
#define SCHEDULERDEF_H

// QT
#include <QGraphicsProxyWidget>

// Viewer
#include "Widget/Definition/ContentsDef.h"
#include "Widget/Image/ImageItem.h"
#include "Widget/Video/VideoItem.h"
#include "Widget/Clock/ClockWidget.h"
#include "Widget/Date/DateWidget.h"
#include "Widget/News/NewsFeedWidget.h"
#include "Widget/Weather/WeatherWidget.h"
#include "Widget/Subtitle/SubtitleWidget.h"

// Common
#include "Common/Interface/ContentsPlayDataDef.h"
#include "Common/Interface/ScheduleJsonDef.h"

#define NOT_EXISTED_DATA    -1

namespace PlayScheduleTimer
{
    /** @brief */
    struct PlayingIndex
    {
        PlayingIndex()
            : mediaType(PlayJson::MediaType::NONE_MEDIA)
            , pageIdx(0)
            , layerIdx(0)
            , contentIdx(0)
        { }
        PlayJson::PlayData playData;
        PlayJson::MediaType mediaType;

        // custom
        int pageIdx;

        // fixed
        int layerIdx;
        int contentIdx;
    };

    /** @brief  for countdown */
    struct FixedLayerTimecount
    {
        FixedLayerTimecount()
            : maxContent(0)
        {
        }
        int maxContent;
        QVector<int> contentTimeout;
    };

    /** @brief */
    struct CountdownInfo
    {
        CountdownInfo()
            : id(0)
            , type(PlayJson::PlayDataType::NONE_PLAY_DATA_TYPE)
            , maxPage(0)
            , maxLayer(0)
        {
        }
        int id;
        PlayJson::PlayDataType type;

        // custom
        int maxPage;
        QVector<int> pageTimecount;

        // fixed
        int maxLayer;
        QVector<int> layerContentIdxList; // content index per layer
        QVector<FixedLayerTimecount> layerTimecountList;
    };


    /**
     *  @brief  operator ==
     */
    inline bool operator==(const PlayingIndex& lhs, const PlayingIndex& rhs)
    {
        if(!(lhs.playData == rhs.playData))
            return false;
        if(!(lhs.mediaType == rhs.mediaType))
            return false;
        if(!(lhs.pageIdx == rhs.pageIdx))
            return false;

        return true;
    }

    /**
     *  @brief  operator !=
     */
    inline bool operator!=(const PlayingIndex& lhs, const PlayingIndex& rhs)
    {
        return !(lhs == rhs);
    }
}


/**
 *  @brief  std::pair<PlayDataId, PlayJson::PlayDataType>
 */
typedef std::pair<PlayScheduleTimer::PlayingIndex, QGraphicsScene*> SceneInfo;
typedef std::pair<PlayScheduleTimer::PlayingIndex, VideoItem*> VideoItemInfo;
typedef std::pair<PlayScheduleTimer::PlayingIndex, ImageItem*> ImageItemInfo;
typedef std::pair<PlayScheduleTimer::PlayingIndex, ClockWidget*> ClockWidgetInfo;
typedef std::pair<PlayScheduleTimer::PlayingIndex, DateWidget*> DateWidgetInfo;
typedef std::pair<PlayScheduleTimer::PlayingIndex, NewsFeedWidget*> NewsFeedWidgetInfo;
typedef std::pair<PlayScheduleTimer::PlayingIndex, WeatherWidget*> WeatherWidgetInfo;
typedef std::pair<PlayScheduleTimer::PlayingIndex, SubtitleWidget*> SubtitleWidgetInfo;

typedef std::pair<PlayScheduleTimer::PlayingIndex, QGraphicsProxyWidget*> ProxyWidgetInfo;

#endif // SCHEDULERDEF_H
