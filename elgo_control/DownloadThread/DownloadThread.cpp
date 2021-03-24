// Control
#include "DownloadThread.h"
#include "CurlDownload.h"
#include "JSON/JsonParser.h"
#include "Logger/ControlLogger.h"


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
    if(DownloadDef::Action::RESOURCE == m_action)
    {
        ExecDownloadResourceData();
    }
    else
    {
        ELGO_CONTROL_LOG("Error - Unkwon action : %d", m_action);
    }
}

//========================================================
void DownloadThread::ExecDownloadResourceData()
//========================================================
{
    QString resourceUrl;
    QByteArray urlBytes = m_bytes;
    QDataStream readStream(&urlBytes, QIODevice::ReadOnly);
    readStream >> resourceUrl;

    QString response;
    const bool bIsResponse = CurlDownload::DownloadResourceList(resourceUrl, response);
    if(true == bIsResponse)
    {
        QList<ContentSchema::Resource> resource;
        JsonParser::ParseResourceResponse(response, resource);

        const int resourceSize = resource.size();
        for(int idx = 0; idx < resourceSize; idx++)
        {
            const bool bIsDownload = CurlDownload::DownloadResourceData(resource[idx]);
            if(false == bIsDownload)
            {
                ELGO_CONTROL_LOG("Error - Failed Download : %s", resource[idx].name.toStdString().c_str());
            }
        }
    }
    else
    {
        ELGO_CONTROL_LOG("Error - Failed Downloaded : %s", resourceUrl.toUtf8().constData());
    }
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
