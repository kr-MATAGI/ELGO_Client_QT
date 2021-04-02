#ifndef SCHEDULERDEF_H
#define SCHEDULERDEF_H

// Viewer
#include "Widget/Definition/ContentsDef.h"
#include "Widget/Image/ImageItem.h"
#include "Widget/Video/VideoItem.h"

// Common
#include "Common/Interface/ContentsPlayDataDef.h"
#include "Common/Interface/ScheduleDef.h"

#define NOT_EXISTED_DATA    -1

namespace SchedulerDef
{
    /** @brief */
    struct PlayDataInfo
    {
        int id;
        PlayJson::PlayDataType type;
    };

    /** @brief */
    struct ContentIndxInfo
    {
        PlayDataInfo playDataInfo;

        // custom
        int pageIdx;

        // fixed
        int layerIdx;
        int contentIdx;
    };

    /** @brief */
    struct CustomTimeCnt
    {
        CustomTimeCnt()
            : pageIdx(0)
            , timeCount(0)
        { }
        int pageIdx;
        int timeCount;
    };

    /** @brief  for countdown */
    struct FixedLayerTimeCnt
    {
        int maxContent;
        QVector<int> contentTimeout;
    };

    /** @brief */
    struct CountDownInfo
    {
        PlayDataInfo playDataInfo;

        // custom
        int maxPage;
        CustomTimeCnt customTimeCnt;

        // fixed
        int maxLayer;
        QVector<FixedLayerTimeCnt> layerTimeCntList;
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

    /**
     *  @brief  operator !=
     */
    inline bool operator!=(const PlayDataInfo& lhs, const PlayDataInfo& rhs) { return !(rhs == lhs); }
}


/**
 *  @brief  std::pair<PlayDataId, PlayJson::PlayDataType>
 */
typedef std::pair<SchedulerDef::ContentIndxInfo, QGraphicsScene *> SceneInfo;
typedef std::pair<SchedulerDef::ContentIndxInfo, VideoItem *> VideoItemInfo;
typedef std::pair<SchedulerDef::ContentIndxInfo, ImageItem *> ImageItemInfo;


#endif // SCHEDULERDEF_H
