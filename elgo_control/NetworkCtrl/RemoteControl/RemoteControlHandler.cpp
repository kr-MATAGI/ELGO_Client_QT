// EFC
#include "LocalSocketEvent/EFCEvent.h"

// Control
#include "Logger/ControlLogger.h"
#include "RemoteControlHandler.h"
#include "JSON/JsonParser.h"

//========================================================
RemoteControlHandler::RemoteControlHandler()
//========================================================
{

}

//========================================================
RemoteControlHandler::~RemoteControlHandler()
//========================================================
{

}

//========================================================
void RemoteControlHandler::RunAction(Remote::Action action, const QString& src, Remote::Result::Contents& results)
//========================================================
{
    if(Remote::Action::DEVICE_LOGIN == action)
    {
        results.status = GetDeviceLoginInfoValidation(src);
    }
    else if(Remote::Action::LOAD_WIFI_LIST == action)
    {
        // TODO : make Code
        results.status = GetAvailableWifiList(src);
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
Remote::Result::Status RemoteControlHandler::GetDeviceLoginInfoValidation(const QString& src)
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
Remote::Result::Status RemoteControlHandler::GetAvailableWifiList(const QString& src)
//========================================================
{
    Remote::Result::Status retValue = Remote::Result::Status::NONE_RESULT;

    // TODO : load wifi list


    return retValue;
}

//========================================================
Remote::Result::Status RemoteControlHandler::ManageDeviceInfo(const QString& src)
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
Remote::Result::Status RemoteControlHandler::RotateDeviceDisplay(const QString& src)
//========================================================
{
    Remote::Result::Status retValue = Remote::Result::Status::ROTATE_DISPLAY_FAIL;

    Remote::RotateDisplay rotateDisplay;
    const bool bIsParsing = JsonParser::ParseRemoteControlRotateDevice(src, rotateDisplay);
    if(true == bIsParsing)
    {
        /**
        * @note
        *       ELGO_CONTROL -> ELGO_VIEWER
        *       Rotate Display accroding to heading enum value.
        * @param
        *      quint8   heading (top : 1, right : 2, bottom : 3, left : 4)
        */
        QByteArray bytes;
        QDataStream dataStream(&bytes, QIODevice::WriteOnly);
        quint8 heading = static_cast<quint8>(rotateDisplay.heading);
        dataStream << heading;

        const bool bSendEvent = EFCEvent::SendEvent(ELGO_PROC::Proc::ELGO_VIEWER, VIEWER_EVENT::Event::ROTATE_DISPLAY, bytes);
        if(true == bSendEvent)
        {
            retValue = Remote::Result::Status::ROTATE_DISPLAY_OK;
        }
    }
    else
    {
        ELGO_CONTROL_LOG("Error - Rotate Display json was not parsed");
    }

    return retValue;
}
