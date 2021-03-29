
// EFC
#include "LocalSocketEvent/EFCEvent.h"

// Control
#include "JSON/JsonParser.h"
#include "JSON/JsonWriter.h"
#include "ContentWebSocketHandler.h"
#include "Logger/ControlLogger.h"
#include "DownloadThread/DownloadThread.h"

//========================================================
ContentWebSocketHandler::ContentWebSocketHandler()
//========================================================
{
    m_threadPool = new QThreadPool;
    m_threadPool->setMaxThreadCount(MAX_THREAD_COUNT);
}

//========================================================
ContentWebSocketHandler::~ContentWebSocketHandler()
//========================================================
{

}

//========================================================
void ContentWebSocketHandler::RunEvent(const ContentSchema::Summary& serverJson, QString& clientJson)
//========================================================
{
    if(ContentSchema::Event::READY == serverJson.event)
    {
        ExecReadyEvent(serverJson, clientJson);
    }
    else if(ContentSchema::Event::RENAME == serverJson.event)
    {
        ExecRenameEvent(serverJson, clientJson);
    }
    else if(ContentSchema::Event::SINGLE_PLAY == serverJson.event)
    {
        ExecSinglePlayEvent(serverJson);
    }
    else if(ContentSchema::Event::PLAY_SCHEDULES == serverJson.event)
    {
        ExecPlaySchedulesEvent(serverJson);
    }
    else if(ContentSchema::Event::POWER_SCHEDULES == serverJson.event)
    {

    }
    else if(ContentSchema::Event::CLEAR_PLAY_SCHEDULE == serverJson.event)
    {

    }
    else if(ContentSchema::Event::CLEAR_POWER_SCHEDULE == serverJson.event)
    {

    }
    else if(ContentSchema::Event::ERROR == serverJson.event)
    {
        ELGO_CONTROL_LOG("ERROR - %s", serverJson.payload.message.toUtf8().constData());
    }
    else
    {
        ELGO_CONTROL_LOG("Unkwon Event : %d", serverJson.event);
    }
}

//========================================================
void ContentWebSocketHandler::ExecReadyEvent(const ContentSchema::Summary& serverJson, QString& clientJson)
//========================================================
{
    ContentSchema::Summary modifiedResponse = serverJson;
    modifiedResponse.payload.src = serverJson.payload.dest;
    modifiedResponse.payload.dest = serverJson.payload.src;
    modifiedResponse.event = ContentSchema::Event::ACCESS;
    modifiedResponse.payload.type = ContentSchema::PayloadType::ONCE;
    modifiedResponse.payload.displayPower = NetworkController::GetInstance()->GetNetworkCtrl().GetDisplaySleepStatus();

    JsonWriter::WriteContentServerAccessEvent(modifiedResponse, clientJson);
}

//========================================================
void ContentWebSocketHandler::ExecRenameEvent(const ContentSchema::Summary& serverJson, QString& clientJson)
//========================================================
{
    NetworkController::GetInstance()->GetDBCtrl().UpdateDeviceNameFromServer(serverJson.payload.deviceName);

    /**
    * @note
    *       ELGO_CONTROL -> ELGO_VIEWER
    *       Viewer will make qr code image and display.
    * @param
    *       QString ip
    */
    QString ip = NetworkController::GetInstance()->GetNetworkCtrl().GetConnectInfo().REMOTE_HOST;
    QByteArray sendBytes;
    QDataStream sendStream(&sendBytes, QIODevice::WriteOnly);
    sendStream << ip;
    const bool bViewerEvent = EFCEvent::SendEvent(ELGO_SYS::Proc::ELGO_VIEWER,
                                                  VIEWER_EVENT::Event::MAKE_QRCODE, sendBytes);
    if(false == bViewerEvent)
    {
        ELGO_CONTROL_LOG("SendEvent Error - %d", VIEWER_EVENT::Event::MAKE_QRCODE);
    }

    // Json
    ContentSchema::Summary modifiedJson = serverJson;
    modifiedJson.payload.src = serverJson.payload.dest;
    modifiedJson.payload.dest = serverJson.payload.src;
    modifiedJson.payload.type = ContentSchema::PayloadType::RESPONSE;

    JsonWriter::WriteContentServerRenameEvent(modifiedJson, clientJson);
}

//========================================================
void ContentWebSocketHandler::ExecSinglePlayEvent(const ContentSchema::Summary& serverJson)
//========================================================
{
    QByteArray bytes;
    QDataStream stream(&bytes, QIODevice::WriteOnly);
    stream << serverJson.payload.url;

    DownloadThread *thread = new DownloadThread;
    thread->SetContentSchema(serverJson);
    thread->SetDownloadAction(DownloadDef::Action::SINGLE_PLAY);
    thread->SetDownloadBytes(bytes);
    m_threadPool->start(thread);
}

//========================================================
void ContentWebSocketHandler::ExecPlaySchedulesEvent(const ContentSchema::Summary& serverJson)
//========================================================
{
    QByteArray bytes;
    QDataStream stream(&bytes, QIODevice::WriteOnly);
    stream << serverJson.payload.url;

    DownloadThread *thread = new DownloadThread;
    thread->SetContentSchema(serverJson);
    thread->SetDownloadAction(DownloadDef::Action::PLAY_SCHEDULES);
    thread->SetDownloadBytes(bytes);
    m_threadPool->start(thread);
}
