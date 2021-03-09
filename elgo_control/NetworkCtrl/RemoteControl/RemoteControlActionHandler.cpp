
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
bool RemoteControlActionHandler::RunAction(Remote::Action action, const QString& src)
//========================================================
{
    bool retValue = false;

    if(Remote::Action::DEVICE_LOGIN == action)
    {
        retValue = GetDeviceLoginInfoValidation(src);
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

    Remote::Summary deviceLoginInfo;
    const bool bIsParsing = JsonParser::RemoteControlDeviceLoginParse(src, deviceLoginInfo);
    if(true == bIsParsing)
    {
        // Checking id, pw in device Table
        retValue = NetworkController::GetInstance()->GetDBCtrl().CheckDeviceLogingInfo(deviceLoginInfo.device);
        ELGO_CONTROL_LOG("Device {ID : %s, PW : %s}, verify result : %d",
                         deviceLoginInfo.device.id.toUtf8().constData(), deviceLoginInfo.device.pw.toUtf8().constData(), retValue);
    }
    else
    {
        ELGO_CONTROL_LOG("Error - Device Login json was not parsed");
    }

    return retValue;
}

