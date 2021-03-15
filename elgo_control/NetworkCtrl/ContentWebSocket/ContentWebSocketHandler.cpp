
// Control
#include "JSON/JsonParser.h"
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
        JsonParser::WriteContentServerAccessRequest(modifiedResponse, request);
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
