
// QT
#include <QGraphicsScene>

// Viewer
#include "SinglePlayTimer.h"
#include "Logger/ViewerLogger.h"

// Common
#include "Common/CommonDef.h"

//========================================================
SinglePlayTimer::SinglePlayTimer(QObject *parent)
    : QTimer(parent)
//========================================================
{

    // connect
    connect(this, SIGNAL(timeout()),
            this, SLOT(SinglePlayTimeout()));
}

//========================================================
SinglePlayTimer::~SinglePlayTimer()
//========================================================
{

}

//========================================================
void SinglePlayTimer::AddPlayData(const PlayJson::CustomPlayDataJson& src)
//========================================================
{
    if(m_customPlayDataList.end() == std::find(m_customPlayDataList.begin(),
                                               m_customPlayDataList.end(),
                                               src))
    {
        m_customPlayDataList.push_back(src);
        ELGO_VIEWER_LOG("ADD PlayData - {id : %d, type : %d}",
                        src.playData.id, src.playData.playDataType);
    }
    else
    {
        ELGO_VIEWER_LOG("Existed PlayData - {id : %d, type : %d}",
                        src.playData.id, src.playData.playDataType);
    }
}

//========================================================
void SinglePlayTimer::AddPlayData(const PlayJson::FixedPlayDataJson& src)
//========================================================
{
    if(m_fixedPlayDataList.end() == std::find(m_fixedPlayDataList.begin(),
                                              m_fixedPlayDataList.end(),
                                              src))
    {
        m_fixedPlayDataList.push_back(src);
        ELGO_VIEWER_LOG("ADD PlayData - {id : %d, type : %d}",
                        src.playData.id, src.playData.playDataType);
    }
    else
    {
        ELGO_VIEWER_LOG("Existed PlayData - {id : %d, type : %d}",
                        src.playData.id, src.playData.playDataType);
    }
}

//========================================================
void SinglePlayTimer::ExecPlayData(SchedulerDef::PlayDataInfo& playDataInfo)
//========================================================
{
    if(m_customPlayDataInfo.playDataInfo != playDataInfo)
    {
        // Make Custom or Fixed Contents
        SchedulerDef::PlayDataIndexInfo playDataIdxInfo;
        playDataIdxInfo.playDataInfo = playDataInfo;

        if(PlayJson::PlayDataType::CUSTOM == playDataInfo.type)
        {
            MakeCustomPlayDataContents(playDataIdxInfo);
        }
        else
        {
            MakeFixedPlayDataContents(playDataIdxInfo);
        }

        // Update Scene
        UpdatePlayerNewScene(playDataIdxInfo);
    }
    else
    {
        ELGO_VIEWER_LOG("Already Played Data - {id : %d, type : %d}",
                        playDataInfo.id, playDataInfo.type);
    }
}

//========================================================
void SinglePlayTimer::MakeCustomPlayDataContents(SchedulerDef::PlayDataIndexInfo& playDataIdxInfo)
//========================================================
{
    const SchedulerDef::PlayDataInfo& playDataInfo = playDataIdxInfo.playDataInfo;

    int customPlayIdx = NOT_EXISTED_DATA;
    const int customPlayListSize = m_customPlayDataList.size();
    for(int idx = 0; idx < customPlayListSize; idx++)
    {
        if(playDataInfo.id == m_customPlayDataList[idx].playData.id)
        {
            customPlayIdx = idx;
        }
    }

    if(NOT_EXISTED_DATA == customPlayIdx)
    {
        ELGO_VIEWER_LOG("Not Existed customPlayData - id: %d", playDataInfo.id);

        return;
    }

    const PlayJson::CustomPlayDataJson& customPlayData = m_customPlayDataList[customPlayIdx];

    // Make Page CountDown Info
    SchedulerDef::CountdownInfo maxCountDown;
    maxCountDown.playDataInfo = playDataInfo;
    maxCountDown.maxPage = customPlayData.pageDataList.size();

    // Make Content Data - Widget / File
    const int pageDataListSize = customPlayData.pageDataList.size();
    for(int pageIdx = 0; pageIdx < pageDataListSize; pageIdx++)
    {
        const PlayJson::PageData& pageData = customPlayData.pageDataList[pageIdx];

        // timecount
        maxCountDown.pageTimecount.push_back(0);

        const int layerDataListSize = pageData.layerDataList.size();
        for(int layIdx = 0; layIdx < layerDataListSize; layIdx++)
        {
            const PlayJson::CustomLayerData& layerData = pageData.layerDataList[layIdx];

            // Content Index Info
            SchedulerDef::PlayDataIndexInfo contentIdxInfo;
            contentIdxInfo.playDataInfo = playDataInfo;
            contentIdxInfo.pageIdx = pageIdx;

            // pos, size
            StyleSheet::PosSizeInfo posSizeInfo;
            posSizeInfo.pos = QPointF(QPoint(layerData.left, layerData.top));
            posSizeInfo.size = QSize(layerData.width, layerData.height);

            // make content data
            if(PlayJson::ContentType::FILE == layerData.layerContent.contentInfo.contentType)
            {
                MakeFileTypeContent(contentIdxInfo, layerData.layerContent, posSizeInfo);
            }
            else
            {
                MakeWidgetTypeContent(contentIdxInfo, layerData.layerContent, posSizeInfo);
            }
        }

        // Subtitle Data
        pageData.subtitleDataList;
    }

    m_countdownInfo = maxCountDown;
    playDataIdxInfo.pageIdx = 0;
}

//========================================================
void SinglePlayTimer::MakeFixedPlayDataContents(SchedulerDef::PlayDataIndexInfo& playDataIdxInfo)
//========================================================
{
    const SchedulerDef::PlayDataInfo& playDataInfo = playDataIdxInfo.playDataInfo;

    int fixedPlayIdx = NOT_EXISTED_DATA;
    const int fixedPlayListSize = m_fixedPlayDataList.size();
    for(int idx = 0; idx < fixedPlayListSize; idx++)
    {
        if(playDataInfo.id == m_fixedPlayDataList[idx].playData.id)
        {
            fixedPlayIdx = idx;
        }
    }

    if(NOT_EXISTED_DATA == fixedPlayIdx)
    {
        ELGO_VIEWER_LOG("Not Existed fixedPlayData - id: %d", playDataInfo.id);

        return;
    }

    const PlayJson::FixedPlayDataJson& fixedPlayData = m_fixedPlayDataList[fixedPlayIdx];

    // Make Layer Countdon Info
    SchedulerDef::CountdownInfo maxCountdownInfo;
    SchedulerDef::CurrFixedPlayInfo fixdPlayContentIdxInfo;

    maxCountdownInfo.playDataInfo = playDataInfo;
    fixdPlayContentIdxInfo.playDataInfo = playDataInfo;

    maxCountdownInfo.maxLayer = fixedPlayData.layerDataList.size();
    maxCountdownInfo.layerTimecountList.resize(maxCountdownInfo.maxLayer);
    fixdPlayContentIdxInfo.layerInfo.resize(maxCountdownInfo.maxLayer);

    for(int layIdx = 0; layIdx < maxCountdownInfo.maxLayer; layIdx++)
    {
        const int maxContentNum = fixedPlayData.layerDataList[layIdx].contentDataList.size();
        maxCountdownInfo.layerTimecountList[layIdx].maxContent = maxContentNum;
        fixdPlayContentIdxInfo.layerInfo[layIdx] = 0;

        for(int cdIdx = 0; cdIdx < maxContentNum; cdIdx++)
        {
            maxCountdownInfo.layerTimecountList[layIdx].contentTimeout.push_back(0);
        }
    }
    m_countdownInfo = maxCountdownInfo;
    m_fixedPlayDataInfo = fixdPlayContentIdxInfo;

    // Conetnt Index Info
    SchedulerDef::PlayDataIndexInfo contentIdxInfo;
    contentIdxInfo.playDataInfo = playDataInfo;

    // Make Content Data - Widget / File
    const int layerListSize = fixedPlayData.layerDataList.size();
    for(int layIdx = 0; layIdx < layerListSize; layIdx++)
    {
        const PlayJson::FixedLayerData& layerData = fixedPlayData.layerDataList[layIdx];
        contentIdxInfo.layerIdx = layIdx;

        // pos, size
        StyleSheet::PosSizeInfo posSizeInfo;
        posSizeInfo.pos = QPointF(QPoint(layerData.left, layerData.top));
        posSizeInfo.size = QSize(layerData.width, layerData.height);

        const int contentListSize = layerData.contentDataList.size();
        for(int cdIdx = 0; cdIdx < contentListSize; cdIdx++)
        {
            contentIdxInfo.contentIdx = cdIdx;

            // make content data
            const PlayJson::ContentData& contentData = layerData.contentDataList[cdIdx];
            if(PlayJson::ContentType::FILE == contentData.contentInfo.contentType)
            {
                MakeFileTypeContent(contentIdxInfo, contentData, posSizeInfo);
            }
            else
            {
                MakeWidgetTypeContent(contentIdxInfo, contentData, posSizeInfo);
            }
        }
    }

    // Subtitle Data
    fixedPlayData.subtitleDataList;
}

//========================================================
void SinglePlayTimer::MakeFileTypeContent(const SchedulerDef::PlayDataIndexInfo& contentIndxInfo,
                                          const PlayJson::ContentData& contentData,
                                          const StyleSheet::PosSizeInfo& posSizeInfo)
//========================================================
{
    QString fullPath = RESOURCE_SAVE_PATH;
    const QString& mediaTypeStr = ConvertMediaTypeEnumToString(contentData.contentInfo.mediaType);
    if(0 >= mediaTypeStr.length())
    {
        ELGO_VIEWER_LOG("Error - MediaTypeStr.length() == 0");

        return;
    }
    fullPath += mediaTypeStr;
    fullPath += contentData.name;

    if(PlayJson::MediaType::IMAGE == contentData.contentInfo.mediaType)
    {
        ImageItem *imageItem = new ImageItem;
        const bool bIsSetItem = imageItem->SetImageItem(fullPath, posSizeInfo);
        if(true == bIsSetItem)
        {
            m_imageItemList.push_back(std::make_pair(contentIndxInfo, imageItem));
        }
        else
        {
            ELGO_VIEWER_LOG("Error - Not Add Image Item : %s",
                            contentData.name.toUtf8().constData());
        }
    }
    else
    {
        VideoItem *videoItem = new VideoItem;

        VideoInfo::MetaData videoInfo;
        videoInfo.fileName = contentData.name;
        videoInfo.duration.user = contentData.userDuration;
        videoInfo.duration.file = contentData.fileDuration;

        const bool bIsSetItem = videoItem->SetVideoFileToBuffer(fullPath, videoInfo);
        if(true == bIsSetItem)
        {
            videoItem->SetVideoPosAndSize(posSizeInfo);
            m_videoItemList.push_back(std::make_pair(contentIndxInfo, videoItem));
        }
        else
        {
            ELGO_VIEWER_LOG("Error - Not Add Video Item : %s",
                            contentData.name.toUtf8().constData());
        }
    }
}

//========================================================
void SinglePlayTimer::MakeWidgetTypeContent(const SchedulerDef::PlayDataIndexInfo& contentIndxInfo,
                                            const PlayJson::ContentData& contentData,
                                            const StyleSheet::PosSizeInfo& posSizeInfo)
//========================================================
{

}

//========================================================
QString SinglePlayTimer::ConvertMediaTypeEnumToString(const PlayJson::MediaType type)
//========================================================
{
    QString retValue;

    if(PlayJson::MediaType::VIDEO == type)
    {
        retValue = "video/";
    }
    else if(PlayJson::MediaType::IMAGE == type)
    {
        retValue = "image/";
    }
    else
    {
        ELGO_VIEWER_LOG("Error - Unkwon MediaType : %d", type);
    }

    return retValue;
}

//========================================================
void SinglePlayTimer::SinglePlayTimeout()
//========================================================
{
    // Check Current Played Page or Content Index
    if(PlayJson::PlayDataType::CUSTOM == m_customPlayDataInfo.playDataInfo.type)
    {
        // Custom
        QVector<PlayJson::CustomPlayDataJson>::const_iterator cusIter = m_customPlayDataList.constBegin();
        for(; cusIter != m_customPlayDataList.constEnd(); ++cusIter)
        {
            if(cusIter->playData.id == m_customPlayDataInfo.playDataInfo.id)
            {
                // Get currPage
                const int currPageIdx = m_customPlayDataInfo.pageIdx;
                const PlayJson::PageData& currPageData = cusIter->pageDataList[currPageIdx];

                if( 1 ==  m_countdownInfo.maxPage)
                {
                    ELGO_VIEWER_LOG("Not Update Scene - Max Page Size : %d", m_countdownInfo.maxPage);
                    return;
                }

                if( m_countdownInfo.pageTimecount[currPageIdx] == currPageData.duration )
                {
                    // currPage timecount init
                    m_countdownInfo.pageTimecount[currPageIdx] = 0;

                    // nextPage
                    int nextPageIdx = currPageIdx + 1;
                    if(m_countdownInfo.maxPage <= nextPageIdx)
                    {
                        nextPageIdx = 0;
                    }

                    SchedulerDef::PlayDataIndexInfo nextPagePlayData = m_customPlayDataInfo;
                    nextPagePlayData.pageIdx = nextPageIdx;
                    ELGO_VIEWER_LOG("Update NextPage- id: %d, pageIdx: %d, currPageTimeCnt{ %d : %d }",
                                    nextPagePlayData.playDataInfo.id, nextPageIdx,
                                    m_countdownInfo.pageTimecount[currPageIdx], currPageData.duration);

                    UpdatePlayerNewScene(nextPagePlayData);
                }
                else
                {
                    // Plus Timecount
                    m_countdownInfo.pageTimecount[currPageIdx]++;
                    ELGO_VIEWER_LOG("Custom Page[%d] Countdown : %d", currPageIdx, m_countdownInfo.pageTimecount[currPageIdx]);
                }
            }
        }
    }
    else
    {
        // Fixed
        QVector<PlayJson::FixedPlayDataJson>::const_iterator fixIter = m_fixedPlayDataList.constBegin();
        for(; fixIter != m_fixedPlayDataList.constEnd(); ++fixIter)
        {
            if(fixIter->playData.id == m_fixedPlayDataInfo.playDataInfo.id)
            {
                const int layerListSize = m_fixedPlayDataInfo.layerInfo.size();
                if(1 == layerListSize && 1 == fixIter->layerDataList[0].contentDataList.size())
                {
                    ELGO_VIEWER_LOG("Not Update Scene - List Size {layer:%d content: %d}",
                                    layerListSize, fixIter->layerDataList[0].contentDataList.size());
                    return;
                }

                for(int layIdx = 0; layIdx < layerListSize; layIdx++)
                {
                    const int currContnetIdx = m_fixedPlayDataInfo.layerInfo[layIdx];
                    const int currContentTimeout = m_countdownInfo.layerTimecountList[layIdx].contentTimeout[currContnetIdx];

                    if(currContentTimeout == fixIter->layerDataList[layIdx].contentDataList[currContnetIdx].userDuration)
                    {
                        // change content in layer
                        m_countdownInfo.layerTimecountList[layIdx].contentTimeout[currContnetIdx] = 0;

                        // next content
                        int nextContentIdx = currContnetIdx + 1;
                        if(m_countdownInfo.layerTimecountList[layIdx].maxContent <= nextContentIdx)
                        {
                            nextContentIdx = 0;
                        }

                        SchedulerDef::PlayDataIndexInfo prevPlayContentData;
                        prevPlayContentData.playDataInfo = m_customPlayDataInfo.playDataInfo;
                        prevPlayContentData.layerIdx = layIdx;
                        prevPlayContentData.contentIdx = currContnetIdx;

                        SchedulerDef::PlayDataIndexInfo nextPlayContentData;
                        nextPlayContentData.playDataInfo = m_customPlayDataInfo.playDataInfo;
                        nextPlayContentData.layerIdx = layIdx;
                        nextPlayContentData.contentIdx = nextContentIdx;

                        // Update
                        UpdateFixedContentToScene(prevPlayContentData, nextPlayContentData);
                        m_fixedPlayDataInfo.layerInfo[layIdx] = nextContentIdx;
                    }
                    else
                    {
                        // plus content timeout
                        m_countdownInfo.layerTimecountList[layIdx].contentTimeout[currContnetIdx]++;
                        ELGO_VIEWER_LOG("Fixd Layer[%d] Content[%d] Countdown : %d, userDuration : %d",
                                        layIdx, currContnetIdx,
                                        m_countdownInfo.layerTimecountList[layIdx].contentTimeout[currContnetIdx],
                                        fixIter->layerDataList[layIdx].contentDataList[currContnetIdx].userDuration);
                    }
                }
            }
        }
    }
}

//========================================================
void SinglePlayTimer::UpdatePlayerNewScene(SchedulerDef::PlayDataIndexInfo& playDataIdxInfo)
//========================================================
{
    if(PlayJson::PlayDataType::CUSTOM == playDataIdxInfo.playDataInfo.type)
    {
        bool bIsFindScene = false;

        QVector<SceneInfo>::iterator sceneIter = m_sceneList.begin();
        for(; sceneIter != m_sceneList.end(); ++sceneIter)
        {
            if(sceneIter->first == playDataIdxInfo)
            {
                bIsFindScene = true;
                ELGO_VIEWER_LOG("Success Find Scene {id: %d, pageIdx: %d, itemSize: %d",
                                sceneIter->first.playDataInfo.id, sceneIter->first.pageIdx,
                                sceneIter->second->items().size());
                ContentsPlayer::GetInstance()->UpdatePlayerScene(sceneIter->second);
                break;
            }
        }

        if(false == bIsFindScene)
        {
            QGraphicsScene *newScene = new QGraphicsScene;
            ELGO_VIEWER_LOG("Made New Custom Scene {id: %d, pageIdx: %d}",
                            playDataIdxInfo.playDataInfo.id, playDataIdxInfo.pageIdx);

            SearchItemAndAddToScene(playDataIdxInfo, newScene);

            m_sceneList.push_back(std::make_pair(playDataIdxInfo, newScene));
            ELGO_VIEWER_LOG("ScencList Size : %d", m_sceneList.size());
            ContentsPlayer::GetInstance()->UpdatePlayerScene(newScene);
        }
    }
    else
    {
        // Fixed Play Data
        QGraphicsScene *newScene = new QGraphicsScene;

        // Find Image Item
        for(int idx =0 ;idx < m_countdownInfo.maxLayer; idx++)
        {
            SearchItemAndAddToScene(playDataIdxInfo, newScene);

            playDataIdxInfo.layerIdx++;
        }

        m_sceneList.push_back(std::make_pair(playDataIdxInfo, newScene));
        ELGO_VIEWER_LOG("ScencList Size : %d", m_sceneList.size());
        ContentsPlayer::GetInstance()->UpdatePlayerScene(newScene);
    }

    m_customPlayDataInfo = playDataIdxInfo;
}

//========================================================
void SinglePlayTimer::UpdateFixedContentToScene(const SchedulerDef::PlayDataIndexInfo& prevDataIdxInfo,
                                                const SchedulerDef::PlayDataIndexInfo& newDataIdxInfo)
//========================================================
{
    // Find Scene
    QVector<SceneInfo>::iterator sceneIter = m_sceneList.begin();
    for(; sceneIter != m_sceneList.end(); ++sceneIter)
    {
        if( (sceneIter->first.playDataInfo.id == newDataIdxInfo.playDataInfo.id) &&
            (sceneIter->first.playDataInfo.type == newDataIdxInfo.playDataInfo.type) )
        {
            // ADD New Image
            QVector<ImageItemInfo>::iterator imageIter = m_imageItemList.begin();
            for(; imageIter != m_imageItemList.end(); ++imageIter)
            {
                if(imageIter->first == newDataIdxInfo)
                {
                    sceneIter->second->addItem(imageIter->second);

                    ELGO_VIEWER_LOG("ADD Image Item to Scene : %s, Index {layer: %d, content: %d}",
                                    imageIter->second->GetImageFileName().toUtf8().constData(),
                                    newDataIdxInfo.layerIdx, newDataIdxInfo.contentIdx);
                }
            }

            // Remove Prev Image
            imageIter = m_imageItemList.begin();
            for(; imageIter != m_imageItemList.end(); ++imageIter)
            {
                if(imageIter->first == prevDataIdxInfo)
                {
                    sceneIter->second->removeItem(imageIter->second);
                    ContentsPlayer::GetInstance()->DeletePlayerSceneItem(imageIter->second);

                    ELGO_VIEWER_LOG("Remove Image Item to Scene : %s, Index {layer: %d, content: %d}",
                                    imageIter->second->GetImageFileName().toUtf8().constData(),
                                    prevDataIdxInfo.layerIdx, prevDataIdxInfo.contentIdx);
                }
            }

            // ADD New Video
            QVector<VideoItemInfo>::iterator videoIter = m_videoItemList.begin();
            for(; videoIter != m_videoItemList.end(); ++videoIter)
            {
                if(videoIter->first == newDataIdxInfo)
                {
                    sceneIter->second->addItem(videoIter->second);

                    ELGO_VIEWER_LOG("ADD Video Item to Scene : %s, Index {layer: %d, content: %d}",
                                    videoIter->second->GetVideoFileName().toUtf8().constData(),
                                    newDataIdxInfo.layerIdx, newDataIdxInfo.contentIdx);
                }
            }

            // Remove Prev Video
            videoIter = m_videoItemList.begin();
            for(; videoIter != m_videoItemList.end(); ++videoIter)
            {
                if(videoIter->first == prevDataIdxInfo)
                {
                    sceneIter->second->removeItem(videoIter->second);

                    ELGO_VIEWER_LOG("ADD Video Item to Scene : %s, Index {layer: %d, content: %d}",
                                    videoIter->second->GetVideoFileName().toUtf8().constData(),
                                    prevDataIdxInfo.layerIdx, prevDataIdxInfo.contentIdx);
                }
            }

            break;
        }
    }
}

//========================================================
void SinglePlayTimer::SearchItemAndAddToScene(const SchedulerDef::PlayDataIndexInfo& playDataIdxInfo,
                                              QGraphicsScene* scene)
//========================================================
{
    // Find Image Item
    QVector<ImageItemInfo>::iterator imageIter = m_imageItemList.begin();
    for(; imageIter != m_imageItemList.end(); ++imageIter)
    {
        if(imageIter->first == playDataIdxInfo)
        {
            scene->addItem(imageIter->second);
            ELGO_VIEWER_LOG("ADD Image Item to Scene : %s",
                            imageIter->second->GetImageFileName().toUtf8().constData());
        }
    }

    // Find Widget Item

    // Find Subtitle Item

    // Find Video Item
    QVector<VideoItemInfo>::iterator videoIter = m_videoItemList.begin();
    for(; videoIter != m_videoItemList.end(); ++videoIter)
    {
        if(videoIter->first == playDataIdxInfo)
        {
            scene->addItem(videoIter->second);
            ELGO_VIEWER_LOG("ADD Video Item to Scene : %s",
                            videoIter->second->GetVideoFileName().toUtf8().constData());
        }
    }
}
