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
void RemoteControlHandler::RunAction(Remote::Action action, const QString& src, Remote::Result::Contents& dest)
//========================================================
{
    if(Remote::Action::DEVICE_LOGIN == action)
    {
        dest.status = RemoteDeviceLogin(src);
    }
    else if(Remote::Action::UPDATE_WIFI_LIST == action)
    {
        dest.status = RemoteGetAvailableWifiList(src);
    }
    else if(Remote::Action::MANAGE_DEVICE == action)
    {
        dest.status = RemoteManageDevice(src);
    }
    else if(Remote::Action::ROTATE_DISPLAY == action)
    {
        dest.status = RemoteRotateDeviceScreen(src);
    }
    else if(Remote::Action::DEVICE_OPTIONS == action)
    {
        dest.status = RemoteUpdateDeviceOptions(src);
    }
    else if(Remote::Action::CONNECT_WIFI == action)
    {
        dest.status = RemoteConnectNewWifi(src);
    }
    else if(Remote::Action::USER_LOGIN == action)
    {
        dest.status = RemoteUserLogin(src, dest);
    }
    else
    {
        ELGO_CONTROL_LOG("ERROR - Unknwon Action : %d", action);
    }
}


//========================================================
Remote::Result::Status RemoteControlHandler::RemoteDeviceLogin(const QString& src)
//========================================================
{
    Remote::Result::Status retValue = Remote::Result::Status::DEVIEC_LOGIN_FAIL;

    Remote::DeviceLogin deviceLoginInfo;
    const bool bIsParsing = JsonParser::ParseRemoteDeviceLogin(src, deviceLoginInfo);
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
Remote::Result::Status RemoteControlHandler::RemoteGetAvailableWifiList(const QString& src)
//========================================================
{
    Remote::Result::Status retValue = Remote::Result::Status::NONE_RESULT;

    /**
     *  @note
     *          ELGO_CONTROL -> ELGO_MAIN
     *          Search available wifi list
     *  @param
     *          NONE
     */
    QByteArray bytes;
    const bool bSendEvnet = EFCEvent::SendEvent(ELGO_SYS::Proc::ELGO_MAIN,
                                                MAIN_EVENT::Event::SEARCHING_WIFI_LIST,
                                                bytes);
    if(false == bSendEvnet)
    {
        retValue = Remote::Result::UPDATE_WIFI_LIST_FAIL;
        ELGO_CONTROL_LOG("Error - SendEvent : %d", MAIN_EVENT::Event::SEARCHING_WIFI_LIST);
    }
    else
    {
        retValue = Remote::Result::UPDATE_WIFI_LIST_OK;
    }

    return retValue;
}

//========================================================
Remote::Result::Status RemoteControlHandler::RemoteManageDevice(const QString& src)
//========================================================
{
    Remote::Result::Status retValue = Remote::Result::Status::MANAGE_DEVICE_FAIL;

    Remote::ManageDevice manageDevice;
    const bool bIsParsing = JsonParser::ParseRemoteManageDevice(src, manageDevice);
    if(true == bIsParsing)
    {
        const bool bIsOk = NetworkController::GetInstance()->GetDBCtrl().ChangeDevicePassword(manageDevice);
        if( true == bIsOk )
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
Remote::Result::Status RemoteControlHandler::RemoteRotateDeviceScreen(const QString& src)
//========================================================
{
    Remote::Result::Status retValue = Remote::Result::Status::ROTATE_DISPLAY_OK;

    Remote::RotateDisplay rotateDisplay;
    const bool bIsParsing = JsonParser::ParseRemoteRotateDevice(src, rotateDisplay);
    if(true == bIsParsing)
    {
        ELGO_CONTROL_LOG("New Heading: %d", rotateDisplay.heading);

        QByteArray bytes;
        QDataStream dataStream(&bytes, QIODevice::WriteOnly);
        quint8 heading = static_cast<quint8>(rotateDisplay.heading);
        dataStream << heading;

        /**
         * @note
         *          ELGO_CONTROL -> ELGO_MAIN
         *          Exec Screen Rotation Command line
         * @param
         *          quint8  heading
         */
        const bool bMainEvent = EFCEvent::SendEvent(ELGO_SYS::Proc::ELGO_MAIN,
                                                    MAIN_EVENT::Event::MAIN_ROTATE_SCREEN,
                                                    bytes);
        if(false == bMainEvent)
        {
            retValue = Remote::Result::Status::ROTATE_DISPLAY_FAIL;
            ELGO_CONTROL_LOG("Error SendEvent: %d", MAIN_EVENT::Event::MAIN_ROTATE_SCREEN);

            return retValue;
        }

        /**
         * @note
         *       ELGO_CONTROL -> ELGO_VIEWER
         *       Rotate Display accroding to heading enum value.
         * @param
         *      quint8   heading (top : 1, right : 2, bottom : 3, left : 4)
         */
        const bool bViewerEvent = EFCEvent::SendEvent(ELGO_SYS::Proc::ELGO_VIEWER,
                                                      VIEWER_EVENT::Event::VIEWER_ROTATE_SCREEN,
                                                      bytes);
        if(false == bViewerEvent)
        {
            retValue = Remote::Result::Status::ROTATE_DISPLAY_FAIL;
            ELGO_CONTROL_LOG("Error - SendEvent: %d",
                             VIEWER_EVENT::Event::VIEWER_ROTATE_SCREEN);
        }
    }
    else
    {
        ELGO_CONTROL_LOG("Error - Rotate Display json was not parsed");
    }

    return retValue;
}

//========================================================
Remote::Result::Status RemoteControlHandler::RemoteUpdateDeviceOptions(const QString& src)
//========================================================
{
    Remote::Result::Status retValue = Remote::Result::Status::DEVICE_OPTIONS_FAIL;

    Remote::DeviceOptions deviceOptions;
    const bool bIsParsing = JsonParser::ParseRemoteDeviceOptions(src, deviceOptions);
    if(true == bIsParsing)
    {
        /**
         *  @brief  Change Device Options
         *  @param
         *          bool displaySleep
         *          bool deviceMute
         *          bool contentPause
         */

        QByteArray bytes;
        QDataStream dataStream(&bytes, QIODevice::WriteOnly);

        dataStream << deviceOptions.bDisplaySleep;
        dataStream << deviceOptions.bDeviceMute;
        dataStream << deviceOptions.bContentPause;

        NetworkController::GetInstance()->GetNetworkCtrl().SetDisplaySleepStatus(deviceOptions.bDisplaySleep);
        const bool bSendEvent = EFCEvent::SendEvent(ELGO_SYS::Proc::ELGO_MAIN,
                                                    MAIN_EVENT::Event::UPDATE_DEVICE_OPTIONS,
                                                    bytes);
        if(true == bSendEvent)
        {
            retValue = Remote::Result::Status::DEVICE_OPTIONS_OK;
        }
        else
        {
            ELGO_CONTROL_LOG("Error - SendEvent : %d", MAIN_EVENT::Event::UPDATE_DEVICE_OPTIONS);
        }
    }
    else
    {
        ELGO_CONTROL_LOG("Error - Change Device options json was not parsed");
    }

    return retValue;
}

//========================================================
Remote::Result::Status RemoteControlHandler::RemoteConnectNewWifi(const QString& src)
//========================================================
{
    Remote::Result::Status retValue = Remote::Result::Status::NONE_RESULT;

    Remote::ConnectWifi connectWifi;
    const bool bIsParsing = JsonParser::ParseConnectWifi(src, connectWifi);
    if(true == bIsParsing)
    {
        /**
         *  @note
         *          ELGO_CONTROL -> ELGO_MAIN
         *          Connect new wifi
         *  @param
         *          QString ssid
         *          QString password
         *          bool encryption
         */

        QByteArray bytes;
        QDataStream newStream(&bytes, QIODevice::WriteOnly);
        newStream << connectWifi.ssid;
        newStream << connectWifi.password;
        newStream << connectWifi.bEnc;
        const bool bSendEvnet = EFCEvent::SendEvent(ELGO_SYS::Proc::ELGO_MAIN,
                                                    MAIN_EVENT::Event::CONNECT_NEW_WIFI,
                                                    bytes);
        if(false == bSendEvnet)
        {
            retValue = Remote::Result::CONNECT_WIFI_FAIL;
            ELGO_CONTROL_LOG("Error - SendEvent : %d", MAIN_EVENT::Event::CONNECT_NEW_WIFI);
        }
        else
        {
            retValue = Remote::Result::CONNECT_WIFI_OK;
        }
    }
    else
    {
        retValue = Remote::Result::CONNECT_WIFI_FAIL;
        ELGO_CONTROL_LOG("Error - Parsing Error : %s", src.toStdString().c_str());
    }

    return retValue;
}

//========================================================
Remote::Result::Status RemoteControlHandler::RemoteUserLogin(const QString& src, Remote::Result::Contents& dest)
//========================================================
{
    Remote::Result::Status retValue = Remote::Result::Status::NONE_RESULT;

    const QString os = QSysInfo::productType();
    const QString udid = QSysInfo::machineUniqueId().toStdString().c_str();

    if( (0 < os.length()) && (0 < udid.length()) )
    {
        retValue = Remote::Result::Status::USER_LOGIN_OK;
        dest.os = os;
        dest.udid = udid;
    }
    else
    {
        retValue = Remote::Result::Status::USER_LOGIN_FAIL;
    }

    return retValue;
}
