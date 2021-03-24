
// EFC
#include "LocalSocketEvent/EFCEvent.h"

// Control
#include "JSON/JsonParser.h"
#include "JSON/JsonWriter.h"
#include "ContentWebSocketHandler.h"
#include "Logger/ControlLogger.h"

//========================================================
ContentWebSocketHandler::ContentWebSocketHandler()
//========================================================
{

}

//========================================================
ContentWebSocketHandler::~ContentWebSocketHandler()
//========================================================
{

}

//========================================================
void ContentWebSocketHandler::RunEvent(const ContentSchema::Summary& response, QString& request)
//========================================================
{
    if(ContentSchema::Event::READY == response.event)
    {
        ContentSchema::Summary modifiedResponse = response;
        modifiedResponse.payload.displayPower = NetworkController::GetInstance()->GetNetworkCtrl().GetDisplaySleepStatus();
        JsonWriter::WriteContentServerAccessRequest(modifiedResponse, request);
    }
    else if(ContentSchema::Event::RENAME == response.event)
    {
        NetworkController::GetInstance()->GetDBCtrl().UpdateDeviceNameFromServer(response.payload.deviceName);

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
        const bool bViewerEvent = EFCEvent::SendEvent(ELGO_PROC::Proc::ELGO_VIEWER,
                                                      VIEWER_EVENT::Event::MAKE_QRCODE, sendBytes);
        if(false == bViewerEvent)
        {
            ELGO_CONTROL_LOG("SendEvent Error - %d", VIEWER_EVENT::Event::MAKE_QRCODE);
        }
    }
    else if(ContentSchema::Event::ERROR == response.event)
    {
        ELGO_CONTROL_LOG("ERROR - %s", response.payload.message.toUtf8().constData());
    }
    else
    {
        ELGO_CONTROL_LOG("Unkwon Event : %d", response.event);
    }
}
