// Common
#include "LocalSocketEvent/EFCEvent.h"
#include "Common/Interface/ContentsPlayDataImpl.h"

// Control
#include "UpdateWidgetTimer.h"
#include "Logger/ControlLogger.h"
#include "DownloadThread/CurlDownload.h"
#include "XML/XmlParser.h"
#include "JSON/JsonParser.h"

//========================================================
UpdateWidgetTimer::UpdateWidgetTimer(QObject *parent)
    : QTimer(parent)
//========================================================
{
    // Register meta type
    qRegisterMetaType<QVector<PlayJson::UpdateWidgetInfo>>("QVector<PlayJson::UpdateWidgetInfo>");

    // connect
    connect(this, &UpdateWidgetTimer::UpdateWidgetInfoSignal,
            this, &UpdateWidgetTimer::UpdateWidgetInfoSlot);
    connect(this, &UpdateWidgetTimer::AddWidgetInfoSignal,
            this, &UpdateWidgetTimer::AddWidgetInfoSlot);
    connect(this, &UpdateWidgetTimer::CleaerUpdateWidgetListSignal,
            this, &UpdateWidgetTimer::CleaerUpdateWidgetListSlot);

    connect(this, &UpdateWidgetTimer::StartUpdateWidgetTimer,
            this, &UpdateWidgetTimer::StartUpdateTimer);
    connect(this, &UpdateWidgetTimer::StopUpdateWidgetTimer,
            this, &UpdateWidgetTimer::StopUpdateTimer);

    connect(this, &QTimer::timeout,
            this, &UpdateWidgetTimer::UpdateWidgetTimeout);
}

//========================================================
UpdateWidgetTimer::~UpdateWidgetTimer()
//========================================================
{

}

//========================================================
void UpdateWidgetTimer::UpdateWidgetInfoSlot(const QVector<PlayJson::UpdateWidgetInfo>& src)
//========================================================
{
    m_updateInfoList = src;
}

//========================================================
void UpdateWidgetTimer::AddWidgetInfoSlot(const QVector<PlayJson::UpdateWidgetInfo>& src)
//========================================================
{
    QVector<PlayJson::UpdateWidgetInfo>::const_iterator srcIter = src.constBegin();
    while(srcIter != src.constEnd())
    {
        bool bIsValid = true;

        QVector<PlayJson::UpdateWidgetInfo>::const_iterator infoIter = m_updateInfoList.constBegin();
        while(infoIter != m_updateInfoList.constEnd())
        {
            if(srcIter == infoIter)
            {
                bIsValid = false;
                break;
            }

            ++infoIter;
        }

        if(true == bIsValid)
        {
            m_updateInfoList.push_back(*srcIter);
        }

        ++srcIter;
    }
}

//========================================================
void UpdateWidgetTimer::CleaerUpdateWidgetListSlot()
//========================================================
{
    StopUpdateTimer();

    m_updateInfoList.clear();
    ELGO_CONTROL_LOG("Clear Update Widget List !");
}

//========================================================
void UpdateWidgetTimer::StartUpdateTimer()
//========================================================
{
    if(false == this->isActive())
    {
        ELGO_CONTROL_LOG("Start Update Widget Timer - size: %d",
                         m_updateInfoList.size());
        this->start(UPDATE_TIMEOUT);
    }
}

//========================================================
void UpdateWidgetTimer::StopUpdateTimer()
//========================================================
{
    if(true == this->isActive())
    {
        ELGO_CONTROL_LOG("STOP Update Widget Timer");
        this->stop();
    }
}

//========================================================
void UpdateWidgetTimer::UpdateWidgetTimeout()
//========================================================
{
    QVector<PlayJson::UpdateWidgetInfo> updatedList;

    QVector<PlayJson::UpdateWidgetInfo>::iterator InfoIter = m_updateInfoList.begin();
    while(InfoIter != m_updateInfoList.end())
    {
        QString recvJsonStr;
        if(PlayJson::MediaType::NEWS == InfoIter->mediaType)
        {
            const bool bIsDownload = CurlDownload::DownloadNewsFeedXml(InfoIter->newsCategory, recvJsonStr);
            if(true == bIsDownload)
            {
                const bool bIsParsed = XmlParser::ParseRssNewsFeedResponse(recvJsonStr, InfoIter->newsCount,
                                                                           InfoIter->newsFeedList);
                if(true == bIsParsed)
                {
                    ELGO_CONTROL_LOG("Success - News {newsCount: %d, feedListSize: %d}",
                                     InfoIter->newsCount, InfoIter->newsFeedList.size());
                    updatedList.push_back(*InfoIter);
                }
                else
                {
                    ELGO_CONTROL_LOG("ERROR - Parsing Failed, category: %d", InfoIter->newsCategory);
                }
            }
            else
            {
                ELGO_CONTROL_LOG("ERROR - Download Failed, category: %d", InfoIter->newsCategory);
            }
        }
        else if(PlayJson::MediaType::WEATHER == InfoIter->mediaType)
        {
            // TEST 96, 74 - sahagu
            InfoIter->nx = 96;
            InfoIter->ny = 74;
            const bool bIsDownload = CurlDownload::DownloadWeatherInfoJson(InfoIter->nx, InfoIter->ny, recvJsonStr);
            if(true == bIsDownload)
            {
                PlayJson::ContentData contentData;
                const bool bIsParsed = JsonParser::ParseWeatherInfoJsonResponse(recvJsonStr, contentData);
                if(true == bIsParsed)
                {
                    InfoIter->LGT = contentData.LGT;
                    InfoIter->PTY = contentData.PTY;
                    InfoIter->REH = contentData.REH;
                    InfoIter->RN1 = contentData.RN1;
                    InfoIter->SKY = contentData.SKY;
                    InfoIter->T1H = contentData.T1H;
                    InfoIter->VEC = contentData.VEC;
                    InfoIter->WSD = contentData.WSD;

                    ELGO_CONTROL_LOG("Sucess - Weather {nx: %d, ny: %d}",
                                     InfoIter->nx, InfoIter->ny);
                    updatedList.push_back(*InfoIter);
                }
                else
                {
                    ELGO_CONTROL_LOG("ERROR - Weather Json Parsing {nx: %d, ny: %d}",
                                     InfoIter->nx, InfoIter->ny);
                }
            }
            else
            {
                ELGO_CONTROL_LOG("Error - Weather Info Download Failed {nx: %d, ny: %d}",
                                 InfoIter->nx, InfoIter->ny);
            }
        }
        else
        {
            ELGO_CONTROL_LOG("ERROR - Unknown Type: %d", InfoIter->mediaType);
        }

        ++InfoIter;
    }

    if(false == updatedList.empty())
    {
        QByteArray bytes;
        QDataStream byteStream(&bytes, QIODevice::WriteOnly);
        byteStream << updatedList;
        const bool bSendEvent = EFCEvent::SendEvent(ELGO_SYS::Proc::ELGO_VIEWER,
                                                    VIEWER_EVENT::Event::UPDATE_NEWS_WEATHER_INFO,
                                                    bytes);
        if(false == bSendEvent)
        {
            ELGO_CONTROL_LOG("ERROR - SendEvent: %d", VIEWER_EVENT::Event::UPDATE_NEWS_WEATHER_INFO);
        }
    }
}
