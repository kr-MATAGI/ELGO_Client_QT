
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
void RemoteControlActionHandler::RunAction(Remote::Action action, const QString& src, Remote::Result::Contents& results)
//========================================================
{
    if(Remote::Action::DEVICE_LOGIN == action)
    {
        results.status = GetDeviceLoginInfoValidation(src);
    }
    else if(Remote::Action::LOAD_WIFI_LIST == action)
    {
        // TODO : modified param
        GetAvailableWifiList(src);
    }
    else if(Remote::Action::MANAGE_DEVICE == action)
    {
        results.status = ManageDeviceInfo(src);
    }
    else if(Remote::Action::ROTATE_DISPLAY == action)
    {
        results.status = RotateDeviceDisplay(src);
    }
    else
    {
        ELGO_CONTROL_LOG("Unkwon Action : %d", action);
    }
}


//========================================================
Remote::Result::Status RemoteControlActionHandler::GetDeviceLoginInfoValidation(const QString& src)
//========================================================
{
    Remote::Result::Status retValue = Remote::Result::Status::DEVIEC_LOGIN_FAIL;

    Remote::DeviceLogin deviceLoginInfo;
    const bool bIsParsing = JsonParser::ParseRemoteControlDeviceLogin(src, deviceLoginInfo);
    if(true == bIsParsing)
    {
        // Checking id, pw in device table
        const bool bIsOk = NetworkController::GetInstance()->GetDBCtrl().CheckDeviceLogingInfo(deviceLoginInfo);
        ELGO_CONTROL_LOG("Device {ID : %s, PW : %s}, verify result : %d",
                         deviceLoginInfo.id.toUtf8().constData(),
                         deviceLoginInfo.pw.toUtf8().constData(), retValue);
        if(true == bIsOk)
        {
            retValue = Remote::Result::Status::DEVICE_LOGIN_OK;
        }
    }
    else
    {
        ELGO_CONTROL_LOG("Error - Device Login json was not parsed");
    }

    return retValue;
}

//========================================================
Remote::Result::Status RemoteControlActionHandler::GetAvailableWifiList(const QString& src)
//========================================================
{
    Remote::Result::Status retValue = Remote::Result::Status::NONE_RESULT;

    // TODO : load wifi list


    return retValue;
}

//========================================================
Remote::Result::Status RemoteControlActionHandler::ManageDeviceInfo(const QString& src)
//========================================================
{
    Remote::Result::Status retValue = Remote::Result::Status::MANAGE_DEVICE_FAIL;

    Remote::ManageDevice manageDevice;
    const bool bIsParsing = JsonParser::ParseRemoteControlManageDevice(src, manageDevice);
    if(true == bIsParsing)
    {
        const bool bIsOk = NetworkController::GetInstance()->GetDBCtrl().ChangeDevicePassword(manageDevice);
        if( true == bIsOk)
        {
            retValue = Remote::Result::Status::MANAGE_DEVICE_OK;
            ELGO_CONTROL_LOG("Changed device password { %s -> %s }",
                             manageDevice.oldPw.toUtf8().constData(), manageDevice.newPw.toUtf8().constData());
        }
    }
    else
    {
        ELGO_CONTROL_LOG("Error - Manage Device json was not parsed");
    }


    return retValue;
}

//========================================================
Remote::Result::Status RemoteControlActionHandler::RotateDeviceDisplay(const QString& src)
//========================================================
{
    Remote::Result::Status retValue = Remote::Result::Status::ROTATE_DISPLAY_FAIL;

    Remote::RotateDisplay rotateDisplay;
    const bool bIsParsing = JsonParser::ParseRemoteControlRotateDevice(src, rotateDisplay);
    if(true == bIsParsing)
    {

    }
    else
    {
        ELGO_CONTROL_LOG("Error - Rotate Display json was not parsed");
    }

    return retValue;
}
