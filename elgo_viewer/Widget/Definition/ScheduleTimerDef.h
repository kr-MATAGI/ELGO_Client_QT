#ifndef SCHEDULERDEF_H
#define SCHEDULERDEF_H

// Viewer
#include "Widget/Definition/ContentsDef.h"
#include "Widget/Image/ImageItem.h"
#include "Widget/Video/VideoItem.h"
#include "Widget/Clock/ClockWidget.h"

// Common
#include "Common/Interface/ContentsPlayDataDef.h"
#include "Common/Interface/ScheduleDef.h"

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
    struct PlayDataIndexInfo
    {
        PlayDataIndexInfo()
            : mediaType(PlayJson::MediaType::NONE_MEDIA)
            , pageIdx(0)
            , layerIdx(0)
            , contentIdx(0)
        { }
        PlayDataInfo playDataInfo;
        PlayJson::MediaType mediaType;

        // custom
        int pageIdx;

        // fixed
        int layerIdx;
        int contentIdx;
    };

    /** @brief */
    struct FixedPlayIndexInfo
    {
        PlayDataInfo playDataInfo;

        // content index per layer
        QVector<int> layerInfo;
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
    inline bool operator==(const PlayDataIndexInfo& lhs, const PlayDataIndexInfo& rhs)
    {
        if(!(lhs.playDataInfo == rhs.playDataInfo))
            return false;
        if(!(lhs.pageIdx == rhs.pageIdx))
            return false;
        if(!(lhs.mediaType == rhs.mediaType))
            return false;
        if(!(lhs.layerIdx == rhs.layerIdx))
            return false;
        if(!(lhs.contentIdx == rhs.contentIdx))
            return false;

        return true;
    }

    /**
     *  @brief  operator !=
     */
    inline bool operator!=(const PlayDataInfo& lhs, const PlayDataInfo& rhs) { return !(rhs == lhs); }
}


/**
 *  @brief  std::pair<PlayDataId, PlayJson::PlayDataType>
 */
typedef std::pair<ScheduleTimer::PlayDataIndexInfo, QGraphicsScene*> SceneInfo;
typedef std::pair<ScheduleTimer::PlayDataIndexInfo, VideoItem*> VideoItemInfo;
typedef std::pair<ScheduleTimer::PlayDataIndexInfo, ImageItem*> ImageItemInfo;
typedef std::pair<ScheduleTimer::PlayDataIndexInfo, ClockWidget*> ClockWidgetInfo;


#endif // SCHEDULERDEF_H