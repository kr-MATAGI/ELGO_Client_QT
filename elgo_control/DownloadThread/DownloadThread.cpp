
// EFC
#include "LocalSocketEvent/EFCEvent.h"

// Control
#include "DownloadThread.h"
#include "CurlDownload.h"
#include "JSON/JsonParser.h"
#include "JSON/JsonWriter.h"
#include "XML/XmlParser.h"
#include "Logger/ControlLogger.h"
#include "Utils/VideoInfoHelper.h"

// Common
#include "Common/Interface/ContentsPlayDataImpl.h"
#include "Common/Interface/ScheduleImpl.h"


//========================================================
DownloadThread::DownloadThread()
    : m_action(DownloadDef::Action::NONE_KIND)
//========================================================
{

}

//========================================================
DownloadThread::~DownloadThread()
//========================================================
{

}

//========================================================
void DownloadThread::run()
//========================================================
{
    if(DownloadDef::Action::SINGLE_PLAY == m_action)
    {
        ExecDownloadSinglePlayData();
    }
    else if(DownloadDef::Action::PLAY_SCHEDULES == m_action)
    {
        ExecDownloadPlaySchedules();
    }
    else
    {
        ELGO_CONTROL_LOG("Error - Unknwon action : %d", m_action);
    }
}

//========================================================
void DownloadThread::ExecDownloadSinglePlayData()
//========================================================
{
    const QString& resourceUrl = m_serverJson.payload.url;

    QString response;
    const bool bIsResponse = CurlDownload::DownloadResourceList(resourceUrl, response);
    if(true == bIsResponse)
    {
        // elgo_control -> elgo_main
        /**
         *  @note
         *          ELGO_CONTROL -> ELGO_MAIN
         *          Clear All Play Schedule List
         *          Cause by single play event
         *  @param
         *          NONE
         */
        QByteArray mainBytes;
        const bool bSendMain = EFCEvent::SendEvent(ELGO_SYS::Proc::ELGO_MAIN,
                                                   MAIN_EVENT::Event::CLEARE_ALL_PLAY_SCHEDULE_LIST,
                                                   mainBytes);
        if(false == bSendMain)
        {
            ELGO_CONTROL_LOG("Error - Send Event: %d", MAIN_EVENT::Event::CLEARE_ALL_PLAY_SCHEDULE_LIST);
        }

        PlayJson::CustomPlayDataJson customPlayData;
        PlayJson::FixedPlayDataJson fixedPlayData;
        PlayJson::PlayData playData;

        QVector<ResourceJson::Resource> resource;
        JsonParser::ParseResourceResponse(response, resource);

        // video duration info
        QVector<QPair<QString, qint64>> videoInfoList;

        const int resourceSize = resource.size();
        for(int idx = 0; idx < resourceSize; idx++)
        {
            if(ResourceJson::ResourceType::OBJECT == resource[idx].type)
            {
                QString url = resource[idx].url;
                url += "/";
                url += resource[idx].name;

                QString objectResponse;
                const bool bIsObjectDownload = CurlDownload::DownloadObjectJson(url, objectResponse);
                if(true == bIsObjectDownload)
                {
                    // object.json, elgo_control -> elgo_viewer
                    JsonParser::ParsePlayDataJson(objectResponse, playData);

                    if(PlayJson::PlayDataType::CUSTOM == playData.playDataType)
                    {
                        customPlayData.playData = playData;
                        JsonParser::ParseCustomPlayDataJson(objectResponse, customPlayData);

                        SearchCustomDataWidgetType(customPlayData.pageDataList);
                    }
                    else if(PlayJson::PlayDataType::FIXED == playData.playDataType)
                    {
                        fixedPlayData.playData = playData;
                        JsonParser::ParseFixedPlayDataJson(objectResponse, fixedPlayData);

                        SearchFixedDataWidgetType(fixedPlayData.layerDataList);
                    }
                    else
                    {
                        ELGO_CONTROL_LOG("Error - Unknwon playDataType : %d", playData.playDataType);
                    }
                }
                else
                {
                    ELGO_CONTROL_LOG("Error - Failed Object Download : %s", url.toUtf8().constData());
                }
            }
            else if(ResourceJson::ResourceType::DATA == resource[idx].type)
            {
                // Nothing - schedule.json
            }
            else
            {
                const bool bIsDownload = CurlDownload::DownloadResourceData(resource[idx]);
                if(false == bIsDownload)
                {
                    ELGO_CONTROL_LOG("Error - Failed Download : %s", resource[idx].name.toStdString().c_str());
                }
                else
                {
                    if(ResourceJson::ResourceType::VIDEO == resource[idx].type)
                    {
                        const qint64 duration = VideoInfoHelper::GetVideoDuration(resource[idx].name);
                        videoInfoList.push_back(QPair<QString, qint64>(resource[idx].name, duration));
                    }
                }
            }
        }

        // elgo_control -> elgo_viewer
        if(PlayJson::PlayDataType::CUSTOM == playData.playDataType)
        {
            // Get Video Duration
            const int pageDataListSize = customPlayData.pageDataList.size();
            for(int idx = 0; idx < pageDataListSize; idx++)
            {
                VideoInfoHelper::MatchVideoDuration(videoInfoList, customPlayData.pageDataList[idx].layerDataList);
            }

            /**
             * @note
             *       ELGO_CONTROL -> ELGO_VIEWER
             *       Send custom play data information
             * @param
             *       CustomPlayDataJson customPlayData
             */
            QByteArray bytes;
            QDataStream stream(&bytes, QIODevice::WriteOnly);
            stream << customPlayData;
            const bool bIsSendEvent = EFCEvent::SendEvent(ELGO_SYS::Proc::ELGO_VIEWER,
                                                          VIEWER_EVENT::Event::PLAY_CUSTOM_PLAY_DATA, bytes);
            if(false == bIsSendEvent)
            {
                ELGO_CONTROL_LOG("Error - Send Event : %d", VIEWER_EVENT::Event::PLAY_CUSTOM_PLAY_DATA);
            }
        }
        else if(PlayJson::PlayDataType::FIXED == playData.playDataType)
        {
            // Get Video Duration
            VideoInfoHelper::MatchVideoDuration(videoInfoList, fixedPlayData.layerDataList);
            /**
             * @note
             *       ELGO_CONTROL -> ELGO_VIEWER
             *       Send fixed play data information
             * @param
             *       FixedPlayDataJson customPlayData
             */
            QByteArray bytes;
            QDataStream stream(&bytes, QIODevice::WriteOnly);
            stream << fixedPlayData;
            const bool bIsSendEvent = EFCEvent::SendEvent(ELGO_SYS::Proc::ELGO_VIEWER,
                                                          VIEWER_EVENT::Event::PLAY_FIXED_PLAY_DATA, bytes);
            if(false == bIsSendEvent)
            {
                ELGO_CONTROL_LOG("Error - Send Event : %d", VIEWER_EVENT::Event::PLAY_FIXED_PLAY_DATA);
            }
        }
        else
        {
            ELGO_CONTROL_LOG("Error - Unknwon playDataType : %d", playData.playDataType);
        }

        // send response to content server
        QString responseJson;
        ContentSchema::Summary modifiedJson = m_serverJson;
        modifiedJson.payload.src = m_serverJson.payload.dest;
        modifiedJson.payload.dest = m_serverJson.payload.src;
        modifiedJson.payload.type = ContentSchema::PayloadType::RESPONSE;

        JsonWriter::WriteContentServerSinglePlayEvent(modifiedJson, responseJson);
        NetworkController::GetInstance()->GetNetworkCtrl().GetContentWebSocket().SendTextMessageToServer(responseJson);
    }
    else
    {
        ELGO_CONTROL_LOG("Error - Failed Downloaded : %s", resourceUrl.toStdString().c_str());
    }
}

//========================================================
void DownloadThread::ExecDownloadPlaySchedules()
//========================================================
{
    const QString& scheduleListUrl = m_serverJson.payload.url;

    QString response;
    const bool bIsResponse = CurlDownload::DownloadResourceList(scheduleListUrl, response);
    if(true == bIsResponse)
    {
        PlayJson::PlayData playData;
        PlayJson::CustomPlayDataJson customPlayData;
        PlayJson::FixedPlayDataJson fixedPlayData;
        QVector<ScheduleJson::PlaySchedule> playScheduleList;

        // video duration info
        QVector<QPair<QString, qint64>> videoInfoList;

        QVector<ResourceJson::Resource> resource;
        JsonParser::ParseResourceResponse(response, resource);
        const int resourceSize = resource.size();
        for(int idx = 0; idx < resourceSize; idx++)
        {
            if(ResourceJson::ResourceType::OBJECT == resource[idx].type)
            {
                QString url = resource[idx].url;
                url += "/";
                url += resource[idx].name;

                QString objectResponse;
                const bool bIsObjectDownload = CurlDownload::DownloadObjectJson(url, objectResponse);
                if(true == bIsObjectDownload)
                {
                    JsonParser::ParsePlayDataJson(objectResponse, playData);

                    if(PlayJson::PlayDataType::CUSTOM == playData.playDataType)
                    {
                        customPlayData.playData = playData;
                        JsonParser::ParseCustomPlayDataJson(objectResponse, customPlayData);

                        SearchCustomDataWidgetType(customPlayData.pageDataList);
                    }
                    else if(PlayJson::PlayDataType::FIXED == playData.playDataType)
                    {
                        fixedPlayData.playData = playData;
                        JsonParser::ParseFixedPlayDataJson(objectResponse, fixedPlayData);

                        SearchFixedDataWidgetType(fixedPlayData.layerDataList);
                    }
                    else
                    {
                        ELGO_CONTROL_LOG("Error - Unknwon playDataType : %d", playData.playDataType);
                    }
                }
                else
                {
                    ELGO_CONTROL_LOG("Error - Failed Object Download : %s", url.toUtf8().constData());
                }
            }
            else if(ResourceJson::ResourceType::DATA == resource[idx].type)
            {
                QString url = resource[idx].url;
                url += "/";
                url += resource[idx].name;

                QString dataResponse;
                const bool bIsDataDownload = CurlDownload::DownloadObjectJson(url, dataResponse);
                if(true == bIsDataDownload)
                {
                    JsonParser::ParseSchedulesResponse(dataResponse, playScheduleList);
                }
                else
                {
                    ELGO_CONTROL_LOG("Error - Failed Data Download : %s", url.toUtf8().constData());
                }
            }
            else
            {
                const bool bIsDownload = CurlDownload::DownloadResourceData(resource[idx]);
                if(false == bIsDownload)
                {
                    ELGO_CONTROL_LOG("Error - Failed Download : %s", resource[idx].name.toStdString().c_str());
                }
                else
                {
                    if(ResourceJson::ResourceType::VIDEO == resource[idx].type)
                    {
                        const qint64 duration = VideoInfoHelper::GetVideoDuration(resource[idx].name);
                        videoInfoList.push_back(QPair<QString, qint64>(resource[idx].name, duration));
                    }
                }
            }
        }

        // elgo_control -> elgo_viewer
        if(PlayJson::PlayDataType::CUSTOM == playData.playDataType)
        {
            // Get Video Duration
            const int pageDataListSize = customPlayData.pageDataList.size();
            for(int idx = 0; idx < pageDataListSize; idx++)
            {
                VideoInfoHelper::MatchVideoDuration(videoInfoList, customPlayData.pageDataList[idx].layerDataList);
            }

            /**
             * @note
             *       ELGO_CONTROL -> ELGO_VIEWER
             *       Add custom play data
             * @param
             *       CustomPlayDataJson customPlayData
             */
            QByteArray viewerBytes;
            QDataStream viewerStream(&viewerBytes, QIODevice::WriteOnly);
            viewerStream << customPlayData;

            const bool bViewerSendEvent = EFCEvent::SendEvent(ELGO_SYS::Proc::ELGO_VIEWER,
                                                              VIEWER_EVENT::Event::ADD_CUSTOM_PLAY_DATA,
                                                              viewerBytes);
            if(false == bViewerSendEvent)
            {
                ELGO_CONTROL_LOG("Error - Send Event : %d", VIEWER_EVENT::Event::ADD_CUSTOM_PLAY_DATA);
            }


            /**
             *  @note
             *          ELGO_CONTROL -> ELGO_MAIN
             *          For manage custom/fixed play schedule
             *  @param
             *          QVector<ScheduleJson::PlaySchedule> playScheduleList
             */
            QByteArray mainBytes;
            QDataStream mainStream(&mainBytes, QIODevice::WriteOnly);
            mainStream << playScheduleList;

            const bool bMainSendEvent = EFCEvent::SendEvent(ELGO_SYS::Proc::ELGO_MAIN,
                                                            MAIN_EVENT::Event::UPDATE_PLAY_SCHEDULE_LIST,
                                                            mainBytes);
            if(false == bMainSendEvent)
            {
                ELGO_CONTROL_LOG("Error - Send Event: %d", MAIN_EVENT::Event::UPDATE_PLAY_SCHEDULE_LIST);
            }
        }
        else if(PlayJson::PlayDataType::FIXED == playData.playDataType)
        {
            // Get Video Duration
            VideoInfoHelper::MatchVideoDuration(videoInfoList, fixedPlayData.layerDataList);

            /**
             * @note
             *       ELGO_CONTROL -> ELGO_VIEWER
             *       Add fixed play data
             * @param
             *       FixedPlayDataJson fixedPlayData
             */
            QByteArray viewerBytes;
            QDataStream viewerStream(&viewerBytes, QIODevice::WriteOnly);
            viewerStream << fixedPlayData;

            const bool bViewerSendEvent = EFCEvent::SendEvent(ELGO_SYS::Proc::ELGO_VIEWER,
                                                              VIEWER_EVENT::Event::ADD_FIXED_PLAY_DATA,
                                                              viewerBytes);
            if(false == bViewerSendEvent)
            {
                ELGO_CONTROL_LOG("Error - Send Event : %d", VIEWER_EVENT::Event::ADD_FIXED_PLAY_DATA);
            }

            /**
             *  @note
             *          ELGO_CONTROL -> ELGO_MAIN
             *          For manage custom/fixed play schedule
             *  @param
             *          QVector<ScheduleJson::PlaySchedule> playScheduleList
             */
            QByteArray mainBytes;
            QDataStream mainStream(&mainBytes, QIODevice::WriteOnly);
            mainStream << playScheduleList;

            const bool bMainSendEvent = EFCEvent::SendEvent(ELGO_SYS::ELGO_MAIN,
                                                            MAIN_EVENT::Event::UPDATE_PLAY_SCHEDULE_LIST,
                                                            mainBytes);
            if(false == bMainSendEvent)
            {
                ELGO_CONTROL_LOG("Error - Send Event: %d", MAIN_EVENT::Event::UPDATE_PLAY_SCHEDULE_LIST);
            }
        }
        else
        {
            ELGO_CONTROL_LOG("Error - Unknwon playDataType : %d", playData.playDataType);
        }

        // send response to content server
        QString responseJson;
        ContentSchema::Summary modifiedJson = m_serverJson;
        modifiedJson.payload.src = m_serverJson.payload.dest;
        modifiedJson.payload.dest = m_serverJson.payload.src;
        modifiedJson.payload.type = ContentSchema::PayloadType::RESPONSE;

        JsonWriter::WriteContentServerPlayScheduleEvent(modifiedJson, responseJson);
        NetworkController::GetInstance()->GetNetworkCtrl().GetContentWebSocket().SendTextMessageToServer(responseJson);
    }
    else
    {
        ELGO_CONTROL_LOG("Error - Failed Downloaded : %s", scheduleListUrl.toStdString().c_str());
    }
}


//========================================================
void DownloadThread::SearchCustomDataWidgetType(QVector<PlayJson::PageData>& pageDataList)
//========================================================
{
    QVector<PlayJson::PageData>::iterator pageIter = pageDataList.begin();
    for(; pageIter != pageDataList.end(); ++pageIter)
    {
        QVector<PlayJson::CustomLayerData>::iterator layerDataIter = pageIter->layerDataList.begin();
        for(; layerDataIter != pageIter->layerDataList.end(); ++layerDataIter)
        {
            if(PlayJson::ContentType::WIDGET == layerDataIter->layerContent.contentInfo.contentType)
            {
                if( (PlayJson::MediaType::NEWS == layerDataIter->layerContent.contentInfo.mediaType) ||
                    (PlayJson::MediaType::WEATHER == layerDataIter->layerContent.contentInfo.mediaType) )
                {
                    DownloadAdditionalWidgetInfo(layerDataIter->layerContent);
                }
            }
        }
    }
}

//========================================================
void DownloadThread::SearchFixedDataWidgetType(QVector<PlayJson::FixedLayerData>& layerDataList)
//========================================================
{
    QVector<PlayJson::FixedLayerData>::iterator layerDataIter = layerDataList.begin();
    for(; layerDataIter != layerDataList.end(); ++layerDataIter)
    {
        QVector<PlayJson::ContentData>::iterator contentIter = layerDataIter->contentDataList.begin();
        for(; contentIter != layerDataIter->contentDataList.end(); ++contentIter)
        {
            if( (PlayJson::MediaType::NEWS == contentIter->contentInfo.mediaType) ||
                (PlayJson::MediaType::WEATHER == contentIter->contentInfo.mediaType) )
            {
                DownloadAdditionalWidgetInfo(*contentIter);
            }
        }
    }
}

//========================================================
void DownloadThread::DownloadAdditionalWidgetInfo(PlayJson::ContentData& contentData)
//========================================================
{
    if(PlayJson::MediaType::NEWS == contentData.contentInfo.mediaType)
    {
        QString recvJsonStr;
        const bool bIsDownload = CurlDownload::DownloadNewsFeedXml(contentData.newsCategory, recvJsonStr);
        if(true == bIsDownload)
        {
            const bool bIsParsed = XmlParser::ParseRssNewsFeedResponse(recvJsonStr, contentData.newsCount, contentData.newsFeedList);
            if(true == bIsParsed)
            {
                ELGO_CONTROL_LOG("Success - Parsing, category : %d", contentData.newsCategory);
            }
            else
            {
                ELGO_CONTROL_LOG("Error - Parsing Failed, category : %d", contentData.newsCategory);
            }
        }
        else
        {
            ELGO_CONTROL_LOG("Error - Download Failed, category : %d", contentData.newsCategory);
        }
    }
    else if(PlayJson::MediaType::WEATHER == contentData.contentInfo.mediaType)
    {
        QString recvJsonStr;

        // TEST 96, 74 - sahagu
        contentData.nx = 96;
        contentData.ny = 74;
        const bool bIsDownload = CurlDownload::DownloadWeatherInfoJson(contentData, recvJsonStr);
        if(true == bIsDownload)
        {
            const bool bIsParsed = JsonParser::ParseWeatherInfoJsonResponse(recvJsonStr, contentData);
            if(true == bIsParsed)
            {
                ELGO_CONTROL_LOG("Success - Weather Json Parsing, {areaName : %s %s, nx: %d, ny: %d}",
                                 contentData.metropolCityName.toStdString().c_str(), contentData.cityName.toStdString().c_str(),
                                 contentData.nx, contentData.ny);
            }
            else
            {
                ELGO_CONTROL_LOG("Error - Weather Json Parsing, {areaName : %s %s, nx: %d, ny: %d}",
                                 contentData.metropolCityName.toStdString().c_str(), contentData.cityName.toStdString().c_str(),
                                 contentData.nx, contentData.ny);
            }
        }
        else
        {
            ELGO_CONTROL_LOG("Error - Weather Info Download Failed, {areaName : %s %s, nx: %d, ny: %d}",
                             contentData.metropolCityName.toStdString().c_str(), contentData.cityName.toStdString().c_str(),
                             contentData.nx, contentData.ny);
        }
    }
    else
    {
        ELGO_CONTROL_LOG("Error - Media type : %d", contentData.contentInfo.mediaType);
    }
}

//========================================================
void DownloadThread::SetContentSchema(const ContentSchema::Summary& src)
//========================================================
{
    m_serverJson = src;
}

//========================================================
void DownloadThread::SetDownloadAction(const DownloadDef::Action value)
//========================================================
{
    m_action = value;
}

//========================================================
void DownloadThread::SetDownloadBytes(const QByteArray& src)
//========================================================
{
    m_bytes = src;
}
