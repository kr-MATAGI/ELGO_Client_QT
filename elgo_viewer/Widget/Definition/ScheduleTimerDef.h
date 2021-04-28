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

namespace ScheduleTimer
{
    /** @brief */
    struct PlayDataInfo
    {
        int id;
        PlayJson::PlayDataType type;
    };

    /** @brief */
    struct PlayingIndex
    {
        PlayingIndex()
            : pageIdx(0)
        { }
        PlayDataInfo playDataInfo;

        // custom
        int pageIdx;

        // fixed - content index per layer
        QVector<int> layerIdxList;
    };

    /** @brief  for countdown */
    struct FixedLayerTimeCnt
    {
        FixedLayerTimeCnt()
            : maxContent(0)
        { }
        int maxContent;
        QVector<int> contentTimeout;
    };

    /** @brief */
    struct CountdownInfo
    {
        PlayDataInfo playDataInfo;

        // custom
        int maxPage;
        QVector<int> pageTimecount;

        // fixed
        int maxLayer;
        QVector<FixedLayerTimeCnt> layerTimecountList;
    };


    /**
     *  @brief  operator ==
     */
    inline bool operator==(const PlayDataInfo& lhs, const PlayDataInfo& rhs)
    {
        if(!(lhs.id == rhs.id))
            return false;
        if(!(lhs.type == rhs.type))
            return false;

        return true;
    }

    inline bool operator==(const PlayingIndex& lhs, const PlayingIndex& rhs)
    {
        if(!(lhs.playDataInfo == rhs.playDataInfo))
            return false;
        if(!(lhs.pageIdx == rhs.pageIdx))
            return false;

        return true;
    }

    /**
     *  @brief  operator !=
     */
    inline bool operator!=(const PlayDataInfo& lhs, const PlayDataInfo& rhs)
    {
        return !(lhs == rhs);
    }

    inline bool operator!=(const PlayingIndex& lhs, const PlayingIndex& rhs)
    {
        return !(lhs == rhs);
    }
}


/**
 *  @brief  std::pair<PlayDataId, PlayJson::PlayDataType>
 */
typedef std::pair<ScheduleTimer::PlayingIndex, QGraphicsScene*> SceneInfo;
typedef std::pair<ScheduleTimer::PlayingIndex, VideoItem*> VideoItemInfo;
typedef std::pair<ScheduleTimer::PlayingIndex, ImageItem*> ImageItemInfo;
typedef std::pair<ScheduleTimer::PlayingIndex, ClockWidget*> ClockWidgetInfo;
typedef std::pair<ScheduleTimer::PlayingIndex, DateWidget*> DateWidgetInfo;
typedef std::pair<ScheduleTimer::PlayingIndex, NewsFeedWidget*> NewsFeedWidgetInfo;
typedef std::pair<ScheduleTimer::PlayingIndex, WeatherWidget*> WeatherWidgetInfo;
typedef std::pair<ScheduleTimer::PlayingIndex, SubtitleWidget*> SubtitleWidgetInfo;

typedef std::pair<PlayJson::MediaType, ScheduleTimer::PlayingIndex> ProxyDataInfo;
typedef std::pair<ProxyDataInfo, QGraphicsProxyWidget*> ProxyWidgetInfo;

#endif // SCHEDULERDEF_H
