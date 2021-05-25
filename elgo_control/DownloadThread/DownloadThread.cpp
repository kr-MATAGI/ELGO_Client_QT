
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
    else if(DownloadDef::Action::PLAY_SCHEDULE == m_action)
    {
        ExecDownloadPlaySchedule();
    }
    else if(DownloadDef::Action::POWER_SCHEDULE == m_action)
    {
        ExecDownloadPowerSchedule();
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
        PlayJson::CustomPlayDataJson customPlayData;
        PlayJson::FixedPlayDataJson fixedPlayData;
        PlayJson::PlayData playData;

        QVector<PlayJson::UpdateWidgetInfo> updateWidgetList;
        QVector<ResourceJson::Resource> resource;
        JsonParser::ParseResourceResponse(response, resource);

        // video duration info
        QVector<QPair<QString, qint64>> videoInfoList;

        bool bIsError = false;;
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

                        const bool bIsDownloaded = SearchCustomDataWidgetType(customPlayData.playData.id,
                                                                              customPlayData.playData.playDataType,
                                                                              customPlayData.pageDataList,
                                                                              updateWidgetList);
                        if(false == bIsDownloaded)
                        {
                            bIsError = true;
                            break;
                        }
                    }
                    else if(PlayJson::PlayDataType::FIXED == playData.playDataType)
                    {
                        fixedPlayData.playData = playData;
                        JsonParser::ParseFixedPlayDataJson(objectResponse, fixedPlayData);

                        const bool bIsDownloaded = SearchFixedDataWidgetType(fixedPlayData.playData.id,
                                                                             fixedPlayData.playData.playDataType,
                                                                             fixedPlayData.layerDataList,
                                                                             updateWidgetList);
                        if(false == bIsDownloaded)
                        {
                            bIsError = true;
                            break;
                        }
                    }
                    else
                    {
                        ELGO_CONTROL_LOG("Error - Unknwon playDataType : %d", playData.playDataType);
                    }
                }
                else
                {
                    bIsError = true;
                    ELGO_CONTROL_LOG("Error - Failed Object Download : %s", url.toUtf8().constData());

                    break;
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
                    bIsError = true;
                    ELGO_CONTROL_LOG("Error - Failed Download : %s", resource[idx].name.toStdString().c_str());

                    break;
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

        if(false == bIsError)
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
                QByteArray viewerBytes;
                QDataStream viewerStream(&viewerBytes, QIODevice::WriteOnly);
                viewerStream << customPlayData;
                const bool bViewerEvent = EFCEvent::SendEvent(ELGO_SYS::Proc::ELGO_VIEWER,
                                                              VIEWER_EVENT::Event::PLAY_CUSTOM_PLAY_DATA,
                                                              viewerBytes);
                if(false == bViewerEvent)
                {
                    ELGO_CONTROL_LOG("Error - Send Event : %d", VIEWER_EVENT::Event::PLAY_CUSTOM_PLAY_DATA);
                }

                /**
                 *  @note
                 *          ELGO_CONTROL -> ELGO_MAIN
                 *          Add PlayData to DB
                 *          Cause by single play event
                 *  @param
                 *          PlayJson::PlayDataType  playJson
                 *          [ CustomPlayDataJson  customPlayData ||
                 *            FixedPlayDataJson   fixedPlayData ]
                 */
                QByteArray mainBytes;
                QDataStream mainStream(&mainBytes, QIODevice::WriteOnly);
                mainStream << customPlayData.playData.playDataType;
                mainStream << customPlayData;
                const bool bMainEvent = EFCEvent::SendEvent(ELGO_SYS::Proc::ELGO_MAIN,
                                                            MAIN_EVENT::Event::ADD_PLAY_DATA_TO_DB,
                                                            mainBytes);
                if(false == bMainEvent)
                {
                    ELGO_CONTROL_LOG("Error - Send Event: %d", MAIN_EVENT::Event::ADD_PLAY_DATA_TO_DB);
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
                QByteArray viewerBytes;
                QDataStream viewerStream(&viewerBytes, QIODevice::WriteOnly);
                viewerStream << fixedPlayData;
                const bool bViewerEvent = EFCEvent::SendEvent(ELGO_SYS::Proc::ELGO_VIEWER,
                                                              VIEWER_EVENT::Event::PLAY_FIXED_PLAY_DATA,
                                                              viewerBytes);
                if(false == bViewerEvent)
                {
                    ELGO_CONTROL_LOG("Error - Send Event : %d", VIEWER_EVENT::Event::PLAY_FIXED_PLAY_DATA);
                }

                QByteArray mainBytes;
                QDataStream mainStream(&mainBytes, QIODevice::WriteOnly);
                mainStream << fixedPlayData.playData.playDataType;
                mainStream << fixedPlayData;
                const bool bMainEvent = EFCEvent::SendEvent(ELGO_SYS::Proc::ELGO_MAIN,
                                                            MAIN_EVENT::Event::ADD_PLAY_DATA_TO_DB,
                                                            mainBytes);
                if(false == bMainEvent)
                {
                    ELGO_CONTROL_LOG("Error - Send Event: %d", MAIN_EVENT::Event::ADD_PLAY_DATA_TO_DB);
                }
            }
            else
            {
                ELGO_CONTROL_LOG("Error - Unknwon playDataType : %d", playData.playDataType);
            }

            // Send Response
            emit NetworkController::GetInstance()->GetNetworkCtrl().GetContentWebSocket().StopProgressTimerSignal();

            QString responseJson;
            ContentSchema::Summary modifiedJson = m_serverJson;
            modifiedJson.payload.src = m_serverJson.payload.dest;
            modifiedJson.payload.dest = m_serverJson.payload.src;
            modifiedJson.payload.type = ContentSchema::PayloadType::RESPONSE;

            JsonWriter::WriteContentServerSinglePlayEvent(modifiedJson, responseJson);
            NetworkController::GetInstance()->GetNetworkCtrl().GetContentWebSocket().SendTextMessageToServer(responseJson);

            // Start News/Weather Timer
            emit NetworkController::GetInstance()->GetUpdateWidgetTimer().CleaerUpdateWidgetListSignal();
            emit NetworkController::GetInstance()->GetUpdateWidgetTimer().UpdateWidgetInfoSignal(updateWidgetList);
            emit NetworkController::GetInstance()->GetUpdateWidgetTimer().StartUpdateWidgetTimer();
        }
        else
        {
            // Send Error Response
            emit NetworkController::GetInstance()->GetNetworkCtrl().GetContentWebSocket().StopProgressTimerSignal();

            QString responseJson;
            ContentSchema::Summary modifiedJson = m_serverJson;
            modifiedJson.payload.src = m_serverJson.payload.dest;
            modifiedJson.payload.dest = m_serverJson.payload.src;
            modifiedJson.payload.type = ContentSchema::PayloadType::RESPONSE;

            QString errorStr = "Failed - Download Resource";
            JsonWriter::WriteContentServerErrorResponse(modifiedJson, responseJson, errorStr);
            NetworkController::GetInstance()->GetNetworkCtrl().GetContentWebSocket().SendTextMessageToServer(responseJson);

        }
    }
    else
    {
        ELGO_CONTROL_LOG("Error - Failed Downloaded : %s", resourceUrl.toStdString().c_str());

        // Send Error Response
        emit NetworkController::GetInstance()->GetNetworkCtrl().GetContentWebSocket().StopProgressTimerSignal();

        QString responseJson;
        ContentSchema::Summary modifiedJson = m_serverJson;
        modifiedJson.payload.src = m_serverJson.payload.dest;
        modifiedJson.payload.dest = m_serverJson.payload.src;
        modifiedJson.payload.type = ContentSchema::PayloadType::RESPONSE;

        QString errorStr = "Failed - Download Resource List";
        JsonWriter::WriteContentServerErrorResponse(modifiedJson, responseJson, errorStr);
        NetworkController::GetInstance()->GetNetworkCtrl().GetContentWebSocket().SendTextMessageToServer(responseJson);
    }
}

//========================================================
void DownloadThread::ExecDownloadPlaySchedule()
//========================================================
{
    const QString& payloadUrl = m_serverJson.payload.url;

    QString response;
    const bool bIsResponse = CurlDownload::DownloadResourceList(payloadUrl, response);
    if(true == bIsResponse)
    {
        PlayJson::PlayData playData;
        PlayJson::CustomPlayDataJson customPlayData;
        PlayJson::FixedPlayDataJson fixedPlayData;
        QVector<ScheduleJson::PlaySchedule> playScheduleList;

        // video duration info
        QVector<QPair<QString, qint64>> videoInfoList;

        QVector<PlayJson::UpdateWidgetInfo> updateWidgetList;
        QVector<ResourceJson::Resource> resource;
        JsonParser::ParseResourceResponse(response, resource);

        bool bIsError = false;
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

                        const bool bIsDownload = SearchCustomDataWidgetType(customPlayData.playData.id,
                                                                            customPlayData.playData.playDataType,
                                                                            customPlayData.pageDataList,
                                                                            updateWidgetList);
                        if(false == bIsDownload)
                        {
                            bIsError = true;
                            break;
                        }
                    }
                    else if(PlayJson::PlayDataType::FIXED == playData.playDataType)
                    {
                        fixedPlayData.playData = playData;
                        JsonParser::ParseFixedPlayDataJson(objectResponse, fixedPlayData);

                        const bool bIsDownload = SearchFixedDataWidgetType(fixedPlayData.playData.id,
                                                                           fixedPlayData.playData.playDataType,
                                                                           fixedPlayData.layerDataList,
                                                                           updateWidgetList);
                        if(false == bIsDownload)
                        {
                            bIsError = true;
                            break;
                        }
                    }
                    else
                    {
                        ELGO_CONTROL_LOG("Error - Unknwon playDataType : %d", playData.playDataType);
                    }
                }
                else
                {
                    bIsError = true;
                    ELGO_CONTROL_LOG("Error - Failed Object Download : %s", url.toUtf8().constData());

                    break;
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
                    bIsError = true;
                    ELGO_CONTROL_LOG("Error - Failed Data Download : %s", url.toUtf8().constData());

                    break;
                }
            }
            else
            {
                const bool bIsDownload = CurlDownload::DownloadResourceData(resource[idx]);
                if(false == bIsDownload)
                {
                    bIsError = true;
                    ELGO_CONTROL_LOG("Error - Failed Download : %s", resource[idx].name.toStdString().c_str());

                    break;
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

        if(false == bIsError)
        {
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
                 *  @note
                 *          ELGO_CONTROL -> ELGO_MAIN
                 *          For manage custom/fixed play schedule
                 *  @param
                 *          PlayDataType    playDataType
                 *          [ CustomPlayDataJson  customPlayData ||
                 *            FixedPlayDataJson   fixedPlayData ]
                 *          QVector<ScheduleJson::PlaySchedule> playScheduleList
                 */
                QByteArray mainBytes;
                QDataStream mainStream(&mainBytes, QIODevice::WriteOnly);
                mainStream << customPlayData.playData.playDataType;
                mainStream << customPlayData;
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
                 *  @note
                 *          ELGO_CONTROL -> ELGO_MAIN
                 *          For manage custom/fixed play schedule
                 *  @param
                 *          PlayDataType    playDataType
                 *          [ CustomPlayDataJson  customPlayData ||
                 *            FixedPlayDataJson   fixedPlayData ]
                 *          QVector<ScheduleJson::PlaySchedule> playScheduleList
                 */
                QByteArray mainBytes;
                QDataStream mainStream(&mainBytes, QIODevice::WriteOnly);
                mainStream << fixedPlayData.playData.playDataType;
                mainStream << fixedPlayData;
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

            // Send Response
            emit NetworkController::GetInstance()->GetNetworkCtrl().GetContentWebSocket().StopProgressTimerSignal();

            QString clientJsonStr;
            ContentSchema::Summary modifiedJson = m_serverJson;
            modifiedJson.payload.src = m_serverJson.payload.dest;
            modifiedJson.payload.dest = m_serverJson.payload.src;
            modifiedJson.payload.type = ContentSchema::PayloadType::RESPONSE;

            JsonWriter::WriteContentServerPlayScheduleEvent(modifiedJson, clientJsonStr);
            NetworkController::GetInstance()->GetNetworkCtrl().GetContentWebSocket().SendTextMessageToServer(clientJsonStr);

            // Start News / Weather Timer
            emit NetworkController::GetInstance()->GetUpdateWidgetTimer().AddWidgetInfoSignal(updateWidgetList);
            emit NetworkController::GetInstance()->GetUpdateWidgetTimer().StartUpdateWidgetTimer();
        }
        else
        {
            // Send Error Response
            emit NetworkController::GetInstance()->GetNetworkCtrl().GetContentWebSocket().StopProgressTimerSignal();

            QString responseJson;
            ContentSchema::Summary modifiedJson = m_serverJson;
            modifiedJson.payload.src = m_serverJson.payload.dest;
            modifiedJson.payload.dest = m_serverJson.payload.src;
            modifiedJson.payload.type = ContentSchema::PayloadType::RESPONSE;

            QString errorStr = "Failed - Download Resource";
            JsonWriter::WriteContentServerErrorResponse(modifiedJson, responseJson, errorStr);
            NetworkController::GetInstance()->GetNetworkCtrl().GetContentWebSocket().SendTextMessageToServer(responseJson);
        }
    }
    else
    {
        ELGO_CONTROL_LOG("Error - Failed Download : %s", payloadUrl.toStdString().c_str());

        // Send Error Response
        emit NetworkController::GetInstance()->GetNetworkCtrl().GetContentWebSocket().StopProgressTimerSignal();

        QString responseJson;
        ContentSchema::Summary modifiedJson = m_serverJson;
        modifiedJson.payload.src = m_serverJson.payload.dest;
        modifiedJson.payload.dest = m_serverJson.payload.src;
        modifiedJson.payload.type = ContentSchema::PayloadType::RESPONSE;

        QString errorStr = "Failed - Download Resource List";
        JsonWriter::WriteContentServerErrorResponse(modifiedJson, responseJson, errorStr);
        NetworkController::GetInstance()->GetNetworkCtrl().GetContentWebSocket().SendTextMessageToServer(responseJson);
    }
}

//========================================================
void DownloadThread::ExecDownloadPowerSchedule()
//========================================================
{
    const QString payloadUrl = m_serverJson.payload.url;

    QString response;
    const bool bIsResponse = CurlDownload::DownloadResourceList(payloadUrl, response);
    if(true == bIsResponse)
    {
        ScheduleJson::PowerSchedule powerScheduleList;
        JsonParser::ParsePowerSchedulesJson(response, powerScheduleList);

        /**
         *  @note
         *          ELGO_CONTROL -> ELGO_MAIN
         *          Update Power Schedule
         *  @param
         *          ScheduleJson::PowerSchedule powerSchedule
         */
        QByteArray mainBytes;
        QDataStream mainStream(&mainBytes, QIODevice::WriteOnly);
        mainStream << powerScheduleList;

        const bool bMainSend = EFCEvent::SendEvent(ELGO_SYS::Proc::ELGO_MAIN,
                                                   MAIN_EVENT::Event::UPDATE_POWER_SCHEDULE_LIST,
                                                   mainBytes);
        if(false == bMainSend)
        {
            ELGO_CONTROL_LOG("Error - Send Event: %d", MAIN_EVENT::Event::UPDATE_POWER_SCHEDULE_LIST);
        }

        // Send response to content server
        QString clientJsonStr;
        ContentSchema::Summary modifiedJson = m_serverJson;
        modifiedJson.payload.src = m_serverJson.payload.dest;
        modifiedJson.payload.dest = m_serverJson.payload.src;
        modifiedJson.payload.type = ContentSchema::PayloadType::RESPONSE;

        JsonWriter::WriteContentServerPlayScheduleEvent(modifiedJson, clientJsonStr);
        NetworkController::GetInstance()->GetNetworkCtrl().GetContentWebSocket().SendTextMessageToServer(clientJsonStr);
    }
    else
    {
        ELGO_CONTROL_LOG("Error - Failed Download: %s", payloadUrl.toStdString().c_str());
    }
}

//========================================================
bool DownloadThread::SearchCustomDataWidgetType(const int playDataId, const PlayJson::PlayDataType playType,
                                                QVector<PlayJson::PageData>& pageDataList,
                                                QVector<PlayJson::UpdateWidgetInfo>& updateWidgetList)
//========================================================
{
    bool retValue = true;
    QVector<PlayJson::PageData>::iterator pageIter = pageDataList.begin();
    for(; pageIter != pageDataList.end(); ++pageIter)
    {
        if(false == retValue)
        {
            break;
        }

        QVector<PlayJson::CustomLayerData>::iterator layerDataIter = pageIter->layerDataList.begin();
        for(; layerDataIter != pageIter->layerDataList.end(); ++layerDataIter)
        {
            if(PlayJson::ContentType::WIDGET == layerDataIter->layerContent.contentInfo.contentType)
            {
                if( (PlayJson::MediaType::NEWS == layerDataIter->layerContent.contentInfo.mediaType) ||
                    (PlayJson::MediaType::WEATHER == layerDataIter->layerContent.contentInfo.mediaType) )
                {
                    const bool bIsDownloaded = DownloadAdditionalWidgetInfo(layerDataIter->layerContent);
                    if(false == bIsDownloaded)
                    {
                        retValue = false;
                        break;
                    }
                    else
                    {
                        // Update timer after display viewer
                        PlayJson::UpdateWidgetInfo widgetInfo;

                        //id, playType
                        widgetInfo.playDataId = playDataId;
                        widgetInfo.playType = playType;
                        widgetInfo.mediaType = layerDataIter->layerContent.contentInfo.mediaType;

                        widgetInfo.newsCategory = layerDataIter->layerContent.newsCategory;
                        widgetInfo.newsCount = layerDataIter->layerContent.newsCount;

                        widgetInfo.nx = layerDataIter->layerContent.nx;
                        widgetInfo.ny = layerDataIter->layerContent.ny;

                        widgetInfo.pageIndex = pageIter - pageDataList.begin();
                        widgetInfo.layerIndex = layerDataIter - pageIter->layerDataList.begin();

                        updateWidgetList.push_back(widgetInfo);
                    }
                }
            }
        }
    }

    return retValue;
}

//========================================================
bool DownloadThread::SearchFixedDataWidgetType(const int playDataId, const PlayJson::PlayDataType playType,
                                               QVector<PlayJson::FixedLayerData>& layerDataList,
                                               QVector<PlayJson::UpdateWidgetInfo>& updateWidgetList)
//========================================================
{
    bool retValue = true;

    QVector<PlayJson::FixedLayerData>::iterator layerDataIter = layerDataList.begin();
    for(; layerDataIter != layerDataList.end(); ++layerDataIter)
    {
        if(false == retValue)
        {
            break;
        }

        QVector<PlayJson::ContentData>::iterator contentIter = layerDataIter->contentDataList.begin();
        for(; contentIter != layerDataIter->contentDataList.end(); ++contentIter)
        {
            if( (PlayJson::MediaType::NEWS == contentIter->contentInfo.mediaType) ||
                (PlayJson::MediaType::WEATHER == contentIter->contentInfo.mediaType) )
            {
                const bool bIsDownloaded = DownloadAdditionalWidgetInfo(*contentIter);
                if(false == bIsDownloaded)
                {
                    retValue = false;
                    break;
                }
                else
                {
                    // Update timer after display viewer
                    PlayJson::UpdateWidgetInfo widgetInfo;

                    //id, playType
                    widgetInfo.playDataId = playDataId;
                    widgetInfo.playType = playType;
                    widgetInfo.mediaType = contentIter->contentInfo.mediaType;

                    widgetInfo.newsCategory = contentIter->newsCategory;
                    widgetInfo.newsCount = contentIter->newsCount;

                    widgetInfo.nx = contentIter->nx;
                    widgetInfo.ny = contentIter->ny;

                    widgetInfo.layerIndex = layerDataIter - layerDataList.begin();
                    widgetInfo.contentIndex = contentIter - layerDataIter->contentDataList.begin();

                    updateWidgetList.push_back(widgetInfo);
                }
            }
        }
    }

    return retValue;
}

//========================================================
bool DownloadThread::DownloadAdditionalWidgetInfo(PlayJson::ContentData& contentData)
//========================================================
{
    bool bIsDownloaded = true;
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
            bIsDownloaded = false;
            ELGO_CONTROL_LOG("Error - Download Failed, category : %d", contentData.newsCategory);
        }
    }
    else if(PlayJson::MediaType::WEATHER == contentData.contentInfo.mediaType)
    {
        QString recvJsonStr;

        // TEST 96, 74 - sahagu
        contentData.nx = 96;
        contentData.ny = 74;
        const bool bIsDownload = CurlDownload::DownloadWeatherInfoJson(contentData.nx, contentData.ny, recvJsonStr);
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
            bIsDownloaded = false;
            ELGO_CONTROL_LOG("Error - Weather Info Download Failed, {areaName : %s %s, nx: %d, ny: %d}",
                             contentData.metropolCityName.toStdString().c_str(), contentData.cityName.toStdString().c_str(),
                             contentData.nx, contentData.ny);
        }
    }
    else
    {
        ELGO_CONTROL_LOG("Error - Media type : %d", contentData.contentInfo.mediaType);
    }

    return bIsDownloaded;
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
