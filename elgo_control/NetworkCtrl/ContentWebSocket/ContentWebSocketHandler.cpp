
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
    else if(ContentSchema::Event::PLAY_SCHEDULE == serverJson.event)
    {
        ExecPlaySchedulesEvent(serverJson);
    }
    else if(ContentSchema::Event::POWER_SCHEDULE == serverJson.event)
    {
        ExecPowerSchedulesEvent(serverJson);
    }
    else if(ContentSchema::Event::DISPLAY_ON == serverJson.event ||
            ContentSchema::Event::DISPLAY_OFF == serverJson.event)
    {
        ExecDisplayOnOffEvent(serverJson, clientJson);
    }
    else if(ContentSchema::Event::SCREEN_CAPTURE == serverJson.event)
    {
        ExecScreenCaptureEvent(serverJson);
    }
    else if(ContentSchema::Event::SYSTEM_REBOOT == serverJson.event)
    {
//        ExecSystemRebootEvent(serverJson, clientJson);
    }
    else if(ContentSchema::Event::CLEAR_PLAY_SCHEDULE == serverJson.event)
    {
        ExecClearPlaySchdules(serverJson, clientJson);
    }
    else if(ContentSchema::Event::CLEAR_POWER_SCHEDULE == serverJson.event)
    {
        ExecClearPowerSchedules(serverJson);
    }
    else if(ContentSchema::Event::ERROR == serverJson.event)
    {
        ELGO_CONTROL_LOG("ERROR - %s", serverJson.payload.message.toUtf8().constData());
    }
    else
    {
        ELGO_CONTROL_LOG("ERROR - Unknwon Event : %d", serverJson.event);
    }
}

//========================================================
void ContentWebSocketHandler::ExecReadyEvent(const ContentSchema::Summary& serverJson,
                                             QString& clientJson)
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
void ContentWebSocketHandler::ExecRenameEvent(const ContentSchema::Summary& serverJson,
                                              QString& clientJson)
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
                                                  VIEWER_EVENT::Event::MAKE_QRCODE, sendBytes, true);
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
    DownloadThread *thread = new DownloadThread;
    thread->SetDownloadAction(DownloadDef::Action::SINGLE_PLAY);
    thread->SetContentSchema(serverJson);

    m_threadPool->start(thread);
}

//========================================================
void ContentWebSocketHandler::ExecPlaySchedulesEvent(const ContentSchema::Summary& serverJson)
//========================================================
{
    DownloadThread *thread = new DownloadThread;
    thread->SetDownloadAction(DownloadDef::Action::PLAY_SCHEDULE);
    thread->SetContentSchema(serverJson);

    m_threadPool->start(thread);
}

//========================================================
void ContentWebSocketHandler::ExecDisplayOnOffEvent(const ContentSchema::Summary& serverJson,
                                                    QString& clientJson)
//========================================================
{
    bool bDisplaySleep = NetworkController::GetInstance()->GetNetworkCtrl().GetDisplaySleepStatus();
    bool valueChanged = false;
    ELGO_CONTROL_LOG("currDisplaySleep: %d, event: %d", bDisplaySleep, serverJson.event);

    if(true == bDisplaySleep)
    {
        if(ContentSchema::DISPLAY_ON == serverJson.event)
        {
            bDisplaySleep = false;
            valueChanged = true;
        }
    }
    else
    {
        if(ContentSchema::DISPLAY_OFF == serverJson.event)
        {
            bDisplaySleep = true;
            valueChanged = true;
        }
    }

    if(true == valueChanged)
    {
        NetworkController::GetInstance()->GetNetworkCtrl().SetDisplaySleepStatus(bDisplaySleep);

        // Send Event to Main
        /**
         *  @note
         *          ELGO_CONTROL -> ELGO_MAIN
         *          Change display sleep status
         *  @param
         *          bool bDisplaySleep
         */
        QByteArray bytes;
        QDataStream dataStream(&bytes, QIODevice::WriteOnly);
        dataStream << bDisplaySleep;
        const bool bSendEvent = EFCEvent::SendEvent(ELGO_SYS::Proc::ELGO_MAIN,
                                                    MAIN_EVENT::Event::UPDATE_DISPLAY_SLEEP, bytes);
        if(false == bSendEvent)
        {
            ELGO_CONTROL_LOG("Error - SendEvent : %d", MAIN_EVENT::Event::UPDATE_DISPLAY_SLEEP);
        }

        // Response to Server
        ContentSchema::Summary modifiedJson = serverJson;
        modifiedJson.payload.src = serverJson.payload.dest;
        modifiedJson.payload.dest = serverJson.payload.src;
        modifiedJson.payload.type = ContentSchema::PayloadType::RESPONSE;

        JsonWriter::WriteContentServerRenameEvent(modifiedJson, clientJson);
    }
    else
    {
        ELGO_CONTROL_LOG("Not Changed Display Slepp Status - {curr: %d, event:  %d}",
                         bDisplaySleep, serverJson.event);
    }
}

//========================================================
void ContentWebSocketHandler::ExecScreenCaptureEvent(const ContentSchema::Summary& serverJson)
//========================================================
{
    /**
    * @note
    *       ELGO_CONTROL -> ELGO_VIEWER
    *       current Screen capture on elgo_viewer
    *       if capture is successed, response to elgo_control
    * @param
    *       QString payload.src
    *       QString payload.dest
    */

    QByteArray bytes;
    QDataStream dataStream(&bytes, QIODevice::WriteOnly);
    dataStream << serverJson.payload.src;
    dataStream << serverJson.payload.dest;

    const bool bIsSendEvent = EFCEvent::SendEvent(ELGO_SYS::Proc::ELGO_VIEWER,
                                                  VIEWER_EVENT::Event::REQUEST_SCREEN_CAPTURE, bytes);
    if(false == bIsSendEvent)
    {
        ELGO_CONTROL_LOG("Error - Send Event : %d", VIEWER_EVENT::Event::REQUEST_SCREEN_CAPTURE);
    }
}

//========================================================
void ContentWebSocketHandler::ExecSystemRebootEvent(const ContentSchema::Summary& serverJson,
                                                    QString& clientJson)
//========================================================
{
    // Response to Server
    ContentSchema::Summary modifiedJson = serverJson;
    modifiedJson.payload.src = serverJson.payload.dest;
    modifiedJson.payload.dest = serverJson.payload.src;
    modifiedJson.payload.type = ContentSchema::PayloadType::RESPONSE;

    JsonWriter::WriteContentServerRenameEvent(modifiedJson, clientJson);
}

//========================================================
void ContentWebSocketHandler::ExecClearPlaySchdules(const ContentSchema::Summary& serverJson,
                                                    QString& clientJson)
//========================================================
{
    /**
     *  @note
     *          ELGO_CONTROL -> ELGO_MAIN
     *          Clear Play Schedule ID by clearPlaySchedule Event
     *  @param
     *          QString scheduleId
     */
    QByteArray bytes;
    QDataStream dataStream(&bytes, QIODevice::WriteOnly);
    dataStream << serverJson.payload.scheduleId;

    const bool bSendEvent = EFCEvent::SendEvent(ELGO_SYS::Proc::ELGO_MAIN,
                                                MAIN_EVENT::Event::CLEAR_PLAY_SCHEDULE_BY_ID,
                                                bytes);
    if(false == bSendEvent)
    {
        ELGO_CONTROL_LOG("Error - Send Event: %d", MAIN_EVENT::CLEAR_PLAY_SCHEDULE_BY_ID);
    }

    // Response to Server
    ContentSchema::Summary modifiedJson = serverJson;
    modifiedJson.payload.src = serverJson.payload.dest;
    modifiedJson.payload.dest = serverJson.payload.src;
    modifiedJson.payload.type = ContentSchema::PayloadType::RESPONSE;

    JsonWriter::WriteContentServerClearPlayScheduleEvent(modifiedJson, clientJson);
}

//========================================================
void ContentWebSocketHandler::ExecPowerSchedulesEvent(const ContentSchema::Summary& serverJson)
//========================================================
{
    DownloadThread *thread = new DownloadThread;
    thread->SetDownloadAction(DownloadDef::Action::POWER_SCHEDULE);
    thread->SetContentSchema(serverJson);

    m_threadPool->start(thread);
}

//========================================================
void ContentWebSocketHandler::ExecClearPowerSchedules(const ContentSchema::Summary& serverJson)
//========================================================
{

}
