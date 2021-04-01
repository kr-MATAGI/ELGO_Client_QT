
// Common
#include "Common/CommonDef.h"

// Viewer
#include "SchedulesTimer.h"

SchedulesTimer* SchedulesTimer::pInstance = nullptr;

//========================================================
SchedulesTimer::SchedulesTimer(QObject *parent)
    : QTimer(parent)
    , m_bIsSchedule(false)
    , m_bRecvSingePlay(false)
    , m_timeCount(0)
//========================================================
{
    // init
    m_currentPlayData = std::make_pair(0, PlayJson::PlayDataType::NONE_PLAY_DATA_TYPE);
    m_prevSinglePlayData = std::make_pair(0, PlayJson::PlayDataType::NONE_PLAY_DATA_TYPE);

    // connect
    connect(this, SIGNAL(timeout()), this, SLOT(SchedulerTimeoutSlot()));
}

//========================================================
SchedulesTimer::~SchedulesTimer()
//========================================================
{

}

//========================================================
SchedulesTimer* SchedulesTimer::GetInstance()
//========================================================
{
    if(nullptr == pInstance)
    {
        pInstance = new SchedulesTimer();
    }
    return pInstance;
}

//========================================================
void SchedulesTimer::DestoryInstance()
//========================================================
{
    if(nullptr != pInstance)
    {
        return;
    }
    delete pInstance;
    pInstance = NULL;
}

//========================================================
void SchedulesTimer::AddPlayDataList(const PlayJson::FixedPlayDataJson& src,
                                     const bool bIsSiglePlay)
//========================================================
{
    m_bRecvSingePlay = bIsSiglePlay;
    if(true == m_bRecvSingePlay)
    {
        ClearAllSchedulesBySinglePlay();
        m_currentPlayData = std::make_pair(src.playData.id, PlayJson::PlayDataType::FIXED);
    }
    if(m_fixedPlayDataList.end() == std::find(m_fixedPlayDataList.begin(),
                                              m_fixedPlayDataList.end(),
                                              src))
    {
        m_fixedPlayDataList.push_back(src);
        ELGO_VIEWER_LOG("ADD Play Data - {id : %d, type : %d}",
                        src.playData.id, src.playData.playDataType);
    }

    if(true == m_bRecvSingePlay)
    {
        ExecSinglePlay();
    }
}

//========================================================
void SchedulesTimer::AddPlayDataList(const PlayJson::CustomPlayDataJson& src,
                                     const bool bIsSiglePlay)
//========================================================
{
    m_bRecvSingePlay = bIsSiglePlay;
    if(true == m_bRecvSingePlay)
    {
        ClearAllSchedulesBySinglePlay();
        m_currentPlayData = std::make_pair(src.playData.id, PlayJson::PlayDataType::CUSTOM);
    }
    if(m_customPlayDataList.end() == std::find(m_customPlayDataList.begin(),
                                               m_customPlayDataList.end(),
                                               src))
    {
        m_customPlayDataList.push_back(src);
        ELGO_VIEWER_LOG("ADD Play Data - {id : %d, type : %d}",
                        src.playData.id, src.playData.playDataType);
    }

    if(true == m_bRecvSingePlay)
    {
        ExecSinglePlay();
    }
}

//========================================================
void SchedulesTimer::AddFileContentData(const PlayDataInfo& playDataInfo, const PlayJson::ContentData& contentData,
                                        const StyleSheet::WidgetInfo& widgetInfo, QGraphicsScene& scene)
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
        ImageItem *imageItem = new ImageItem();
        const bool bIsSetItem = imageItem->SetImageItem(fullPath, widgetInfo);
        if(true == bIsSetItem)
        {
            m_imageItemList.push_back(std::make_pair(playDataInfo, imageItem));
            scene.addItem(imageItem);
        }
        else
        {
            ELGO_VIEWER_LOG("Error - Not Added Image Item : %s",
                            widgetInfo.fileName.toUtf8().constData());
        }
    }
    else if(PlayJson::MediaType::VIDEO == contentData.contentInfo.mediaType)
    {
        VideoItem *videoItem = new VideoItem;
        VideoInfo::MetaData videoInfo;
        videoInfo.fileName = contentData.name;
        videoInfo.duration.user = contentData.userDuration;
        videoInfo.duration.file = contentData.fileDuration;

        const bool bIsSetItem = videoItem->SetVideoFileToBuffer(fullPath, videoInfo);
        if(true == bIsSetItem)
        {
            m_videoItemList.push_back(std::make_pair(playDataInfo, videoItem));
            scene.addItem(videoItem);
        }
        else
        {
            ELGO_VIEWER_LOG("Error - Not Added Video Item : %s",
                            widgetInfo.fileName.toUtf8().constData());
        }
    }
    else
    {
        ELGO_VIEWER_LOG("Error - MediaType : %d", contentData.contentInfo.mediaType);
    }
}

//========================================================
void SchedulesTimer::AddWidgetContentData(const PlayDataInfo& playDataInfo, const PlayJson::ContentData& contentData,
                                          const StyleSheet::WidgetInfo& widgetInfo, QGraphicsScene& scene)
//========================================================
{

}

//========================================================
void SchedulesTimer::AddSubtitleData(const PlayDataInfo& playDataInfo,
                                     const PlayJson::SubtitleData& subtitleData, QGraphicsScene& scene)
//========================================================
{

}

//========================================================
void SchedulesTimer::AddPlaySchedule(const ScheduleJson::PlaySchedules& src)
//========================================================
{
    m_bRecvSingePlay = false;
    m_playScheduleList.push_back(src);
}

//========================================================
void SchedulesTimer::ClearAllSchedulesBySinglePlay()
//========================================================
{
    m_customPlayDataList.clear();
    m_fixedPlayDataList.clear();
    m_playScheduleList.clear();
    m_currentScheduleId.clear();

    m_currentPlayData = std::make_pair(0, PlayJson::PlayDataType::NONE_PLAY_DATA_TYPE);
    m_prevPlayData = std::make_pair(0, PlayJson::PlayDataType::NONE_PLAY_DATA_TYPE);
    m_prevSinglePlayData = std::make_pair(0, PlayJson::PlayDataType::NONE_PLAY_DATA_TYPE);
}

//========================================================
void SchedulesTimer::SchedulerTimeoutSlot()
//========================================================
{
    QDateTime currentTime = QDateTime::currentDateTime();

    if(false == m_bRecvSingePlay)
    {
        CheckPlayDateTimeSchedules(currentTime);
    }
}

//========================================================
void SchedulesTimer::CheckPlayDateTimeSchedules(const QDateTime& currentDateTime)
//========================================================
{
    // Check start, end
    const qint64 currSecEpoch = currentDateTime.toSecsSinceEpoch();

    QVector<ScheduleJson::PlaySchedules>::iterator playScheduleIter = m_playScheduleList.begin();
    for(; playScheduleIter != m_playScheduleList.end(); ++playScheduleIter)
    {
        QVector<ScheduleJson::PlayScheduleData>::iterator scheduleDataIter = playScheduleIter->scheduleList.begin();
        for(; scheduleDataIter != playScheduleIter->scheduleList.end(); ++scheduleDataIter)
        {
            // check end dateTime
            if(currSecEpoch >= scheduleDataIter->endTime.toSecsSinceEpoch())
            {
                ELGO_VIEWER_LOG("Expired - id : %s, start : %s, end : %s",
                                playScheduleIter->id.toUtf8().constData(),
                                ConvertDateTimeFormat(scheduleDataIter->startTime).toStdString().c_str(),
                                ConvertDateTimeFormat(scheduleDataIter->endTime).toStdString().c_str());

                playScheduleIter->scheduleList.erase(scheduleDataIter);


                // TODO : DB Control
            }
            else if(currSecEpoch >= scheduleDataIter->startTime.toSecsSinceEpoch() &&
                    currSecEpoch < scheduleDataIter->endTime.toSecsSinceEpoch())
            {
                // Check Cron format value (rule)
                const bool bIsValidCron = IsValidCronRuleValue(currentDateTime, scheduleDataIter->cron);
                if( (true == bIsValidCron) &&
                    (scheduleDataIter->playDataId != m_currentPlayData.first) &&
                    (scheduleDataIter->type != m_currentPlayData.second) )
                {
                    // Exec Schedule
                    ELGO_VIEWER_LOG("Exec Schedule {id : %d, type : %d}",
                                    scheduleDataIter->playDataId, scheduleDataIter->type);
                    QGraphicsScene *scene = new QGraphicsScene();
                    PlayDataInfo schedulePlayData(scheduleDataIter->playDataId, scheduleDataIter->type);

                    ExecSchedule(playScheduleIter->id, schedulePlayData, *scene);
                    m_sceneList.push_back(std::make_pair(schedulePlayData, scene));

                    return;
                }
            }
        }

        // shceduleData is empty
        if(true == playScheduleIter->scheduleList.empty())
        {
            ELGO_VIEWER_LOG("Empty - id : %s", playScheduleIter->id.toUtf8().constData());
            m_playScheduleList.erase(playScheduleIter);

            // TODO : DB Control
        }
    }

    // Not matched schedule
    UpdatePlayerScene(m_prevSinglePlayData);
}

//========================================================
bool SchedulesTimer::IsValidCronRuleValue(const QDateTime& currentDateTime, const ScheduleJson::Cron& cron)
//========================================================
{
    bool retValue = false;

    const int month = currentDateTime.date().month();
    const int day = currentDateTime.date().day();
    const int dow = currentDateTime.date().dayOfWeek();

    const int hour = currentDateTime.time().hour();
    const int min = currentDateTime.time().minute();
    const int sec = currentDateTime.time().second();

    if(cron.monthRepeat.end() == std::find(cron.monthRepeat.begin(), cron.monthRepeat.end(), month))
    {
        return retValue;
    }

    if(cron.dayRepeat.end() == std::find(cron.dayRepeat.begin(), cron.dayRepeat.end(), day))
    {
        return retValue;
    }

    if(cron.dowRepeat.end() == std::find(cron.dowRepeat.begin(), cron.dowRepeat.end(), dow))
    {
        return retValue;
    }

    if(cron.hourRepeat.end() == std::find(cron.hourRepeat.begin(), cron.hourRepeat.end(), hour))
    {
        return retValue;
    }

    if(cron.minRepeat.end() == std::find(cron.minRepeat.begin(), cron.minRepeat.end(), min))
    {
        return retValue;
    }

    if(cron.secRepeat.end() == std::find(cron.secRepeat.begin(), cron.secRepeat.end(), sec))
    {
        return retValue;
    }

    retValue = true;

    // TODO : Cron options

    return retValue;
}

//========================================================
void SchedulesTimer::ExecSchedule(const QString& scheduleId, const PlayDataInfo& playDataInfo, QGraphicsScene& scene)
//========================================================
{
    SetPlayDataContentToScene(playDataInfo, scene);

    UpdatePlayerScene(playDataInfo, true);
    m_currentScheduleId = scheduleId;
    ELGO_VIEWER_LOG("FOR TEST : Success Update Content Player Scene");
}

//========================================================
void SchedulesTimer::ExecSinglePlay()
//========================================================
{
    QGraphicsScene *scene = new QGraphicsScene();
    SetPlayDataContentToScene(m_currentPlayData, *scene);
    m_sceneList.push_back(std::make_pair(m_currentPlayData, scene));

    UpdatePlayerScene(m_currentPlayData);
    ELGO_VIEWER_LOG("FOR TEST : Success Update Content Player Scene");
}

//========================================================
void SchedulesTimer::SetPlayDataContentToScene(const PlayDataInfo& playDataInfo, QGraphicsScene& scene)
//========================================================
{
    if(PlayJson::PlayDataType::CUSTOM == playDataInfo.second)
    {
        const int customListsize = m_customPlayDataList.size();
        for(int pdIdx = 0; pdIdx < customListsize; pdIdx++)
        {
            if(playDataInfo.first == m_customPlayDataList[pdIdx].playData.id)
            {
                // Page Data
                const QVector<PlayJson::PageData>& pageDataList = m_customPlayDataList[pdIdx].pageDataList;

                const int pageDataListSize = pageDataList.size();
                for(int pageIdx = 0; pageIdx < pageDataListSize; pageIdx++)
                {
                    // Content Data
                    const QVector<PlayJson::CustomLayerData>& customLayerList = pageDataList[pageIdx].layerDataList;

                    const int customLayerListSize = customLayerList.size();
                    for(int layIdx = 0; layIdx < customLayerListSize; layIdx++)
                    {
                        const PlayJson::ContentData& contentData = customLayerList[layIdx].layerContent;

                        StyleSheet::WidgetInfo widgetInfo;
                        widgetInfo.fileName = contentData.name;
                        widgetInfo.pos = QPointF(QPoint(customLayerList[layIdx].left, customLayerList[layIdx].top));
                        widgetInfo.size = QSize(customLayerList[layIdx].width, customLayerList[layIdx].height);

                        if(PlayJson::ContentType::FILE == contentData.contentInfo.contentType)
                        {
                            AddFileContentData(playDataInfo, contentData, widgetInfo, scene);
                        }
                        else
                        {
                            // AddWidgetContentData();
                        }
                    }

                    // Subtitle Data
                    const QVector<PlayJson::SubtitleData>& subtitleList = pageDataList[pageIdx].subtitleDataList;

                    const int subtitleListSize = subtitleList.size();
                    for(int subIdx = 0; subIdx < subtitleListSize; subIdx++)
                    {
                        const PlayJson::SubtitleData& subtitleData = subtitleList[subIdx];
                        AddSubtitleData(playDataInfo, subtitleData, scene);
                    }
                }
            }
        }
    }
    else if(PlayJson::PlayDataType::FIXED == playDataInfo.second)
    {
        const int fixedListSize = m_fixedPlayDataList.size();
        for(int pdIdx = 0; pdIdx < fixedListSize; pdIdx++)
        {
            if(playDataInfo.first == m_fixedPlayDataList[pdIdx].playData.id)
            {
                // Fixed Layer Data
                const QVector<PlayJson::FixedLayerData>& fixedLayerList = m_fixedPlayDataList[pdIdx].layerDataList;

                const int fixedLayerListSize = fixedLayerList.size();
                for(int layIdx = 0; layIdx < fixedLayerListSize; layIdx++)
                {
                    const PlayJson::FixedLayerData& fixedLayer = fixedLayerList[layIdx];
                    const QVector<PlayJson::ContentData> contentDataList = fixedLayer.contentDataList;

                    // Content Data
                    const int contentDataListSize = contentDataList.size();
                    for(int cdIdx = 0; cdIdx < contentDataListSize; cdIdx++)
                    {
                        const PlayJson::ContentData& contentData = contentDataList[cdIdx];

                        StyleSheet::WidgetInfo widgetInfo;
                        widgetInfo.fileName = contentData.name;
                        widgetInfo.pos = QPointF(QPoint(fixedLayer.left, fixedLayer.top));
                        widgetInfo.size = QSize(fixedLayer.width, fixedLayer.height);

                        if(PlayJson::ContentType::FILE == contentData.contentInfo.contentType)
                        {
                            AddFileContentData(playDataInfo, contentData, widgetInfo, scene);
                        }
                        else
                        {
                            // AddWidgetContentData()
                        }
                    }
                }

                // subtitle
                const QVector<PlayJson::SubtitleData>& subtitleList = m_fixedPlayDataList[pdIdx].subtitleDataList;

                const int subtitleListSize = subtitleList.size();
                for(int subIdx = 0; subIdx < subtitleListSize; subIdx++)
                {
                    AddSubtitleData(playDataInfo, subtitleList[subIdx], scene);
                }
            }
        }
    }
}

//========================================================
void SchedulesTimer::UpdatePlayerScene(const PlayDataInfo& playDataInfo, const bool bIsSchedule)
//========================================================
{
    // Find scene
    QVector<SceneInfo>::iterator sceneIter = m_sceneList.begin();
    for(; sceneIter != m_sceneList.end(); ++sceneIter)
    {
        if( (playDataInfo.first == sceneIter->first.first) &&
            (playDataInfo.second == sceneIter->first.second) )
        {
            // Update scene
            ContentsPlayer::GetInstance()->UpdatePlayerScene(*(sceneIter->second));

            break;
        }
    }

    // Prev Video Item Stop and Erase
    QVector<VideoItemInfo>::iterator videoIter = m_videoItemList.begin();
    for(; videoIter != m_videoItemList.end(); ++videoIter)
    {
        if( (m_prevPlayData.first == videoIter->first.first) &&
            (m_prevPlayData.second == videoIter->first.second))
        {
            videoIter->second->StopVideoItem();

            delete videoIter->second;
            videoIter->second = NULL;

            m_videoItemList.erase(videoIter);
        }
    }

    // Erase Image Item
    QVector<ImageItemInfo>::iterator imageIter = m_imageItemList.begin();
    for(; imageIter != m_imageItemList.end(); ++imageIter)
    {
        if( (m_prevPlayData.first == imageIter->first.first) &&
            (m_prevPlayData.second == imageIter->first.second) )

        {
            delete imageIter->second;
            imageIter->second = NULL;

            m_imageItemList.erase(imageIter);
        }
    }

    // earse widget
    // earse subtitle
    // erase scene
    sceneIter = m_sceneList.begin();
    for(; sceneIter != m_sceneList.end(); ++sceneIter)
    {
        if( (m_prevPlayData.first == sceneIter->first.first) &&
            (m_prevPlayData.second == sceneIter->first.second) )
        {
            delete sceneIter->second;
            sceneIter->second = NULL;

            m_sceneList.erase(sceneIter);
        }
    }

    // Update value
    m_bIsSchedule = bIsSchedule;
    if(true == m_bIsSchedule)
    {
        m_prevSinglePlayData = m_currentPlayData;
        m_prevPlayData = m_currentPlayData;
        m_currentPlayData = playDataInfo;
    }
}

//========================================================
QString SchedulesTimer::ConvertDateTimeFormat(const QDateTime &src)
//========================================================
{
    // hh:mm::ss.msec
    QString timeStr = src.time().toString();
    timeStr.append(".");

    //date
    QDate date = src.date();
    QString year = QString::number(date.year());
    QString month = QString::number(date.month());
    QString day = QString::number(date.day());

    // yyyy-mm-dd
    QString dateStr;
    dateStr.append(year);
    dateStr.append("-");
    dateStr.append(month);
    dateStr.append("-");
    dateStr.append(day);

    QString dateTimeStr = dateStr + ":" + timeStr;
    return dateTimeStr;
}

//========================================================
QString SchedulesTimer::ConvertMediaTypeEnumToString(const PlayJson::MediaType type)
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
