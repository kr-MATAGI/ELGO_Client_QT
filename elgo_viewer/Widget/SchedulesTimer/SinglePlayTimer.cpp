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
    , m_currPlayDataType(PlayJson::PlayDataType::NONE_PLAY_DATA_TYPE)
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
void SinglePlayTimer::ExecPlayData(const ScheduleTimer::PlayDataInfo& playDataInfo)
//========================================================
{
    if( ((PlayJson::PlayDataType::CUSTOM == m_currPlayDataType) &&
         (m_customPlayDataInfo.playDataInfo != playDataInfo)) ||
        ((PlayJson::PlayDataType::FIXED == m_currPlayDataType) &&
         (m_fixedPlayDataInfo.playDataInfo != playDataInfo)) ||
        (PlayJson::PlayDataType::NONE_PLAY_DATA_TYPE == m_currPlayDataType))
    {
        ELGO_VIEWER_LOG("Exec - {id: %d, type: %d}", playDataInfo.id, playDataInfo.type);

        // Make Custom or Fixed Contents
        ScheduleTimer::PlayDataIndexInfo playDataIdxInfo;
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
        m_currPlayDataType = playDataInfo.type;
    }
    else
    {
        ELGO_VIEWER_LOG("Already Played Data - {id : %d, type : %d}",
                        playDataInfo.id, playDataInfo.type);
    }
}

//========================================================
void SinglePlayTimer::UpdateCustomPlayDataIndexInfo(const ScheduleTimer::PlayDataIndexInfo& playDataIndexInfo)
//========================================================
{
    m_customPlayDataInfo = playDataIndexInfo;
}

//========================================================
void SinglePlayTimer::ClearPlayData()
//========================================================
{
    this->stop();

    const int prevCustomPlayDataListSize = m_customPlayDataList.size();
    const int prevFixedPlayDataListSize = m_fixedPlayDataList.size();
    ELGO_VIEWER_LOG("Prev Play Data List Size - {custom: %d, fixed: %d}",
                    prevCustomPlayDataListSize, prevFixedPlayDataListSize);

    foreach(auto item, m_customPlayDataList)
    {
        ELGO_VIEWER_LOG("Ready Delete(custom) - {id: %d}", item.playData.id);
    }

    foreach(auto item, m_fixedPlayDataList)
    {
        ELGO_VIEWER_LOG("Ready Delete(fixed) - {id: %d}", item.playData.id);
    }

    m_customPlayDataList.clear();
    m_fixedPlayDataList.clear();
}

//========================================================
void SinglePlayTimer::MakeCustomPlayDataContents(ScheduleTimer::PlayDataIndexInfo& playDataIdxInfo)
//========================================================
{
    const ScheduleTimer::PlayDataInfo& playDataInfo = playDataIdxInfo.playDataInfo;

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
    ScheduleTimer::CountdownInfo maxCountDown;
    maxCountDown.playDataInfo = playDataInfo;
    maxCountDown.maxPage = customPlayData.pageDataList.size();

    // Make Content Data - Widget / File
    const int pageDataListSize = customPlayData.pageDataList.size();
    for(int pageIdx = 0; pageIdx < pageDataListSize; pageIdx++)
    {
        const PlayJson::PageData& pageData = customPlayData.pageDataList[pageIdx];

        // timecount
        maxCountDown.pageTimecount.push_back(0);

        // Content Index Info
        ScheduleTimer::PlayDataIndexInfo contentIdxInfo;
        contentIdxInfo.playDataInfo = playDataInfo;
        contentIdxInfo.pageIdx = pageIdx;

        const int layerDataListSize = pageData.layerDataList.size();
        for(int layIdx = 0; layIdx < layerDataListSize; layIdx++)
        {
            const PlayJson::CustomLayerData& layerData = pageData.layerDataList[layIdx];

            // pos, size
            StyleSheet::PosSizeInfo posSizeInfo;
            posSizeInfo.pos = QPointF(QPoint(layerData.left, layerData.top));
            posSizeInfo.size = QSize(layerData.width, layerData.height);

            // make content data
            if(PlayJson::ContentType::FILE == layerData.layerContent.contentInfo.contentType)
            {
                emit ContentsPlayer::GetInstance()->MakeFileTypeItemSignal(contentIdxInfo, layerData.layerContent, posSizeInfo);
            }
            else
            {
                emit ContentsPlayer::GetInstance()->MakeWidgetTypeItemSingal(contentIdxInfo, layerData.layerContent, posSizeInfo);
            }
        }

        // Subtitle Data
        const int subtitleDataListSize = pageData.subtitleDataList.size();
        for(int subIdx = 0; subIdx < subtitleDataListSize; subIdx++)
        {
            emit ContentsPlayer::GetInstance()->MakeSubtitleWidgetSignal(contentIdxInfo, pageData.subtitleDataList[subIdx]);
        }
    }

    m_countdownInfo = maxCountDown;
    playDataIdxInfo.pageIdx = 0;
}

//========================================================
void SinglePlayTimer::MakeFixedPlayDataContents(ScheduleTimer::PlayDataIndexInfo& playDataIdxInfo)
//========================================================
{
    const ScheduleTimer::PlayDataInfo& playDataInfo = playDataIdxInfo.playDataInfo;

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
    ScheduleTimer::CountdownInfo maxCountdownInfo;
    ScheduleTimer::FixedPlayIndexInfo fixdPlayContentIdxInfo;

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
    ScheduleTimer::PlayDataIndexInfo contentIdxInfo;
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
                emit ContentsPlayer::GetInstance()->MakeFileTypeItemSignal(contentIdxInfo, contentData, posSizeInfo);
            }
            else
            {
                emit ContentsPlayer::GetInstance()->MakeWidgetTypeItemSingal(contentIdxInfo, contentData, posSizeInfo);
            }
        }
    }

    // Subtitle Data
    const int subtitleDataListSize = fixedPlayData.subtitleDataList.size();
    for(int subIdx = 0; subIdx < subtitleDataListSize; subIdx++)
    {
        emit ContentsPlayer::GetInstance()->MakeSubtitleWidgetSignal(contentIdxInfo, fixedPlayData.subtitleDataList[subIdx]);
    }
}

//========================================================
void SinglePlayTimer::SinglePlayTimeout()
//========================================================
{
    // Check Current Played Page or Content Index
    if(PlayJson::PlayDataType::CUSTOM == m_currPlayDataType)
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

                    ScheduleTimer::PlayDataIndexInfo nextPagePlayData = m_customPlayDataInfo;
                    nextPagePlayData.pageIdx = nextPageIdx;
                    ELGO_VIEWER_LOG("Update NextPage - id: %d, pageIdx: %d, currPageTimeCnt{ %d : %d }",
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

                        ScheduleTimer::PlayDataIndexInfo prevPlayContentData;
                        prevPlayContentData.playDataInfo = m_fixedPlayDataInfo.playDataInfo;
                        prevPlayContentData.layerIdx = layIdx;
                        prevPlayContentData.contentIdx = currContnetIdx;

                        ScheduleTimer::PlayDataIndexInfo nextPlayContentData;
                        nextPlayContentData.playDataInfo = m_fixedPlayDataInfo.playDataInfo;
                        nextPlayContentData.layerIdx = layIdx;
                        nextPlayContentData.contentIdx = nextContentIdx;

                        // Update
                        emit ContentsPlayer::GetInstance()->UpdatePlayerFixedLayerContentSignal(prevPlayContentData, nextPlayContentData);
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
void SinglePlayTimer::UpdatePlayerNewScene(ScheduleTimer::PlayDataIndexInfo& playDataIdxInfo)
//========================================================
{
    if(PlayJson::PlayDataType::CUSTOM == playDataIdxInfo.playDataInfo.type)
    {
        ScheduleTimer::PlayDataIndexInfo prevPlayData = m_customPlayDataInfo;
        emit ContentsPlayer::GetInstance()->UpdatePlayerNewCustomSceneSignal(playDataIdxInfo);
        emit ContentsPlayer::GetInstance()->PausePrevPlayDataSignal(prevPlayData);
    }
    else
    {
        emit ContentsPlayer::GetInstance()->UpdatePlayerNewFixedSceneSignal(playDataIdxInfo, m_countdownInfo.maxLayer);
    }
}
