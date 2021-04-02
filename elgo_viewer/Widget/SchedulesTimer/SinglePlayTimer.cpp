
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
void SinglePlayTimer::ExecPlayData(const SchedulerDef::PlayDataInfo& playDataInfo)
//========================================================
{
    if(m_currPlayInfo != playDataInfo)
    {
        QGraphicsScene *newScene = new QGraphicsScene;

        // Make Custom or Fixed Contents
        if(PlayJson::PlayDataType::CUSTOM == playDataInfo.type)
        {
            MakeCustomPlayDataContents(playDataInfo, *newScene);
        }
        else
        {
            MakeFixedPlayDataContents(playDataInfo, *newScene);
        }

//        ContentsPlayer::GetInstance()->UpdatePlayerScene(*newScene);
    }
    else
    {
        ELGO_VIEWER_LOG("Already Played Data - {id : %d, type : %d}",
                        playDataInfo.id, playDataInfo.type);
    }
}

//========================================================
void SinglePlayTimer::MakeCustomPlayDataContents(const SchedulerDef::PlayDataInfo& playDataInfo, QGraphicsScene& scene)
//========================================================
{
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
        ELGO_VIEWER_LOG("Failed found custom play data - id: %d", playDataInfo.id);

        return;
    }

    const PlayJson::CustomPlayDataJson& customPlayData = m_customPlayDataList[customPlayIdx];

    // Make Page CountDown Info
    SchedulerDef::CountDownInfo countDownInfo;
    countDownInfo.playDataInfo = playDataInfo;
    countDownInfo.maxPage = customPlayData.pageDataList.size();

    // Make Content Data - Widget / File
    const int pageDataListSize = customPlayData.pageDataList.size();
    for(int pageIdx = 0; pageIdx < pageDataListSize; pageIdx++)
    {
        const PlayJson::PageData& pageData = customPlayData.pageDataList[pageIdx];

        const int layerDataListSize = pageData.layerDataList.size();
        for(int layIdx = 0; layIdx < layerDataListSize; layIdx++)
        {
            const PlayJson::CustomLayerData& layerData = pageData.layerDataList[layIdx];

            // Content Index Info
            SchedulerDef::ContentIndxInfo contentIdxInfo;
            contentIdxInfo.playDataInfo = playDataInfo;
            contentIdxInfo.pageIdx = pageIdx;

            // pos, size
            StyleSheet::PosSizeInfo posSizeInfo;
            posSizeInfo.pos = QPointF(QPoint(layerData.left, layerData.top));
            posSizeInfo.size = QSize(layerData.width, layerData.height);

            // make content data
            if(PlayJson::ContentType::FILE == layerData.layerContent.contentInfo.contentType)
            {
                MakeFileTypeContent(contentIdxInfo, layerData.layerContent, posSizeInfo, scene);
            }
            else
            {
                MakeWidgetTypeContent(contentIdxInfo, layerData.layerContent, posSizeInfo, scene);
            }
        }

        // SubTitle Data
        pageData.subtitleDataList;
    }
}

//========================================================
void SinglePlayTimer::MakeFixedPlayDataContents(const SchedulerDef::PlayDataInfo& playDataInfo, QGraphicsScene& scene)
//========================================================
{

}

//========================================================
void SinglePlayTimer::MakeFileTypeContent(const SchedulerDef::ContentIndxInfo& contentIndxInfo,
                                          const PlayJson::ContentData& contentData,
                                          const StyleSheet::PosSizeInfo& posSizeInfo,
                                          QGraphicsScene& scene)
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
            scene.addItem(imageItem);
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

            scene.addItem(videoItem);
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
void SinglePlayTimer::MakeWidgetTypeContent(const SchedulerDef::ContentIndxInfo& contentIndxInfo,
                                            const PlayJson::ContentData& contentData,
                                            const StyleSheet::PosSizeInfo& posSizeInfo,
                                            QGraphicsScene& scene)
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
    // Check PlayData Page or fixed Content timeout by duration
}
