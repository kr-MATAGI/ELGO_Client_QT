
// EFC
#include "LocalSocketEvent/EFCEvent.h"

// Control
#include "DownloadThread.h"
#include "CurlDownload.h"
#include "JSON/JsonParser.h"
#include "JSON/JsonWriter.h"
#include "Logger/ControlLogger.h"

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
        ELGO_CONTROL_LOG("Error - Unkwon action : %d", m_action);
    }
}

//========================================================
void DownloadThread::ExecDownloadSinglePlayData()
//========================================================
{
    QString resourceUrl;
    QDataStream readStream(&m_bytes, QIODevice::ReadOnly);
    readStream >> resourceUrl;

    QString response;
    const bool bIsResponse = CurlDownload::DownloadResourceList(resourceUrl, response);
    if(true == bIsResponse)
    {
        QList<ResourceJson::Resource> resource;
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
                    // object.json, elgo_control -> elgo_viewer
                    ObjectJson::PlayData playData;
                    JsonParser::ParsePlayDataJson(objectResponse, playData);

                    if(ObjectJson::PlayDataType::CUSTOM == playData.playDataType)
                    {
                        ObjectJson::CustomPlayDataJson customPlayData;
                        customPlayData.playData = playData;
                        JsonParser::ParseCustomPlayDataJson(objectResponse, customPlayData);

                        /**
                        * @note
                        *       ELGO_CONTROL -> ELGO_VIEWER
                        *       Send custom play data information
                        * @param
                        *       CustomPlayDataJson customPlayData
                        */
                        QByteArray bytes;
                        QDataStream outStream(&bytes, QIODevice::WriteOnly);
                        outStream << customPlayData;

                        const bool bIsSendEvent = EFCEvent::SendEvent(ELGO_SYS::Proc::ELGO_VIEWER,
                                                                      VIEWER_EVENT::Event::CUSTOM_PLAY_DATA, bytes);
                        if(false == bIsSendEvent)
                        {
                            ELGO_CONTROL_LOG("Error - Send Event : %d", VIEWER_EVENT::Event::CUSTOM_PLAY_DATA);
                        }
                    }
                    else if(ObjectJson::PlayDataType::FIXED == playData.playDataType)
                    {
                        ObjectJson::FixedPlayDataJson fixedPlayData;
                        fixedPlayData.playData = playData;
                        JsonParser::ParseFixedPlayDataJson(objectResponse, fixedPlayData);

                        /**
                        * @note
                        *       ELGO_CONTROL -> ELGO_VIEWER
                        *       Send fixed play data information
                        * @param
                        *       FixedPlayDataJson customPlayData
                        */
                        QByteArray bytes;
                        QDataStream outStream(&bytes, QIODevice::WriteOnly);
                        outStream << fixedPlayData;

                        const bool bIsSendEvent = EFCEvent::SendEvent(ELGO_SYS::Proc::ELGO_VIEWER,
                                                                      VIEWER_EVENT::Event::FIXED_PLAY_DATA, bytes);
                        if(false == bIsSendEvent)
                        {
                            ELGO_CONTROL_LOG("Error - Send Event : %d", VIEWER_EVENT::Event::FIXED_PLAY_DATA);
                        }
                    }
                    else
                    {
                        ELGO_CONTROL_LOG("Error - Unkwon playDataType : %d", playData.playDataType);
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
            }
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
    QString scheduleListUrl;
    QDataStream readStream(&m_bytes, QIODevice::ReadOnly);
    readStream >> scheduleListUrl;

    QString response;
    const bool bIsResponse = CurlDownload::DownloadResourceList(scheduleListUrl, response);
    if(true == bIsResponse)
    {
        ObjectJson::PlayData playData;
        ObjectJson::CustomPlayDataJson customPlayData;
        ObjectJson::FixedPlayDataJson fixedPlayData;
        QList<ScheduleJson::PlaySchedules> playScheduleList;

        QList<ResourceJson::Resource> resource;
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

                    if(ObjectJson::PlayDataType::CUSTOM == playData.playDataType)
                    {
                        customPlayData.playData = playData;
                        JsonParser::ParseCustomPlayDataJson(objectResponse, customPlayData);
                    }
                    else if(ObjectJson::PlayDataType::FIXED == playData.playDataType)
                    {
                        fixedPlayData.playData = playData;
                        JsonParser::ParseFixedPlayDataJson(objectResponse, fixedPlayData);
                    }
                    else
                    {
                        ELGO_CONTROL_LOG("Error - Unkwon playDataType : %d", playData.playDataType);
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
            }
        }

        // elgo_control -> elgo_viewer
        if(ObjectJson::PlayDataType::CUSTOM == playData.playDataType)
        {
            /**
            * @note
            *       ELGO_CONTROL -> ELGO_VIEWER
            *       Send fixed play data information
            *       with schedules
            * @param
            *       FixedPlayDataJson customPlayData
            *       int scheduleCount
            *       PlaySchedules schedules
            */

            QByteArray bytes;
            QDataStream stream(&bytes, QIODevice::WriteOnly);
            stream << customPlayData;
            stream << playScheduleList.size();
            stream << playScheduleList;
            const bool bIsSendEvent = EFCEvent::SendEvent(ELGO_SYS::Proc::ELGO_VIEWER,
                                                          VIEWER_EVENT::Event::CUSTOM_PLAY_DATA_SCHEDULES, bytes);
            if(false == bIsSendEvent)
            {
                ELGO_CONTROL_LOG("Error - Send Event : %d", VIEWER_EVENT::Event::CUSTOM_PLAY_DATA_SCHEDULES);
            }
        }
        else if(ObjectJson::PlayDataType::FIXED == playData.playDataType)
        {
            /**
            * @note
            *       ELGO_CONTROL -> ELGO_VIEWER
            *       Send fixed play data information
            *       with schedules
            * @param
            *       FixedPlayDataJson customPlayData
            *       int scheduleCount
            *       PlaySchedules schedules
            */

            QByteArray bytes;
            QDataStream stream(&bytes, QIODevice::WriteOnly);
            stream << fixedPlayData;
            stream << playScheduleList.size();
            stream << playScheduleList;
            const bool bIsSendEvent = EFCEvent::SendEvent(ELGO_SYS::Proc::ELGO_VIEWER,
                                                          VIEWER_EVENT::Event::FIXED_PLAY_DATA_SCHEDULES, bytes);
            if(false == bIsSendEvent)
            {
                ELGO_CONTROL_LOG("Error - Send Event : %d", VIEWER_EVENT::Event::FIXED_PLAY_DATA_SCHEDULES);
            }
        }
        else
        {
            ELGO_CONTROL_LOG("Error - Unkwon Play data type : %d", playData.playDataType);
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
