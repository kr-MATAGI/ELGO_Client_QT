
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
        const bool bResult = GetDeviceLoginInfoValidation(src);
        if(true == bResult)
        {
            results.status = Remote::Result::Status::DEVICE_LOGIN_OK;
        }
        else
        {
            results.status = Remote::Result::Status::DEVIEC_LOGIN_FAIL;
        }
    }
    else if(Remote::Action::LOAD_WIFI_LIST == action)
    {
        // TODO : modified param
        GetAvailableWifiList(src);
    }
    else if(Remote::Action::MANAGE_DEVICE == action)
    {
        const bool bResult = ManageDeviceInfo(src);
        if(true == bResult)
        {
            results.status = Remote::Result::Status::MANAGE_DEVICE_OK;
        }
        else
        {
            results.status = Remote::Result::Status::MANAGE_DEVICE_FAIL;
        }
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

    Remote::DeviceLogin deviceLoginInfo;
    const bool bIsParsing = JsonParser::ParseRemoteControlDeviceLogin(src, deviceLoginInfo);
    if(true == bIsParsing)
    {
        // Checking id, pw in device table
        retValue = NetworkController::GetInstance()->GetDBCtrl().CheckDeviceLogingInfo(deviceLoginInfo);
        ELGO_CONTROL_LOG("Device {ID : %s, PW : %s}, verify result : %d",
                         deviceLoginInfo.id.toUtf8().constData(),
                         deviceLoginInfo.pw.toUtf8().constData(), retValue);
    }
    else
    {
        ELGO_CONTROL_LOG("Error - Device Login json was not parsed");
    }

    return retValue;
}

//========================================================
bool RemoteControlActionHandler::GetAvailableWifiList(const QString& src)
//========================================================
{
    bool retValue = false;

    // TODO : load wifi list


    return retValue;
}

//========================================================
bool RemoteControlActionHandler::ManageDeviceInfo(const QString& src)
//========================================================
{
    bool retValue = false;

    Remote::MangeDevice manageDevice;
    const bool bIsParsing = JsonParser::ParseRemoteControlManageDevice(src, manageDevice);
    if(true == bIsParsing)
    {
        retValue = NetworkController::GetInstance()->GetDBCtrl().ChangeDevicePassword(manageDevice);
        if( true == retValue)
        {
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
