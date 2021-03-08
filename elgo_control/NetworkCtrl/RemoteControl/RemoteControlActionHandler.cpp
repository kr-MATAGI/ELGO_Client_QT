
// Control
#include "Logger/ControlLogger.h"
#include "RemoteControlActionHandler.h"
#include "JSON/JsonParser.h"

//========================================================
RemoteControlActionHandler::RemoteControlActionHandler()
//========================================================
{

}

//========================================================
RemoteControlActionHandler::~RemoteControlActionHandler()
//========================================================
{

}

//========================================================
void RemoteControlActionHandler::RunAction(Remote::Action action, const QString& src)
//========================================================
{
    if(Remote::Action::DEVICE_LOGIN == action)
    {
        GetDeviceLoginInfoValidation(src);
    }
    else
    {
        ELGO_CONTROL_LOG("Unkwon Action : %d", action);
    }
}


//========================================================
bool RemoteControlActionHandler::GetDeviceLoginInfoValidation(const QString& src)
//========================================================
{
    bool retValue = false;

    Remote::Summary recvSummary;
    const bool bIsParsing = JsonParser::RemoteControlDeviceLoginParse(src, recvSummary);
    if(true == bIsParsing)
    {
        ELGO_CONTROL_LOG("Device {ID : %s, PW : %s}",
                         recvSummary.device.id.toUtf8().constData(), recvSummary.device.pw.toUtf8().constData());

        // Checking id, pw in device Table
    }
    else
    {
        return retValue;
        ELGO_CONTROL_LOG("Error - Device Login json was not parsed");
    }

    return retValue;
}

