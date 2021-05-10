#ifndef REMOTESCHEMA_H
#define REMOTESCHEMA_H

// QT
#include <QString>

// Control
#include "Definition/WifiDef.h"

namespace Remote
{
    /** @brief */
    namespace Result
    {
        enum Status
        {
            NONE_RESULT = 0,
            DEVICE_LOGIN_OK = 1,
            DEVIEC_LOGIN_FAIL = 2,
            MANAGE_DEVICE_OK = 3,
            MANAGE_DEVICE_FAIL = 4,
            ROTATE_DISPLAY_OK = 5,
            ROTATE_DISPLAY_FAIL = 6,
            DEVICE_OPTIONS_OK = 7,
            DEVICE_OPTIONS_FAIL = 8,
            UPDATE_WIFI_LIST_OK = 9,
            UPDATE_WIFI_LIST_FAIL = 10,
            CONNECT_WIFI_OK = 11,
            CONNECT_WIFI_FAIL = 12,
            USER_LOGIN_OK = 13,
            USER_LOGIN_FAIL = 14
        };

        struct Contents
        {
            Contents()
                : status(NONE_RESULT)
            {

            }
            Status status;
            QString os;
            QString udid;
            QVector<WifiInfo> wifiList;
        };
    }

    /**
     * @brief   elgo_remote -> elgo_control
     */
    enum Action
    {
        NONE_ACTION = 0,
        DEVICE_LOGIN = 1,
        UPDATE_WIFI_LIST = 2,
        MANAGE_DEVICE = 3,
        ROTATE_DISPLAY = 4,
        DEVICE_OPTIONS = 5,
        CONNECT_WIFI = 6,
        USER_LOGIN = 7
    };

    /** @brief  Device Login */
    struct DeviceLogin
    {
        QString id;
        QString pw;
    };

    /** @brief  Manage Device */
    struct ManageDevice
    {
        QString oldPw;
        QString newPw;
    };

    /** @brief  Rotate Device Display */
    enum Heading
    {
        HEAD_TOP = 1,
        HEAD_RIGHT = 2,
        HEAD_BOTTOM = 3,
        HEAD_LEFT = 4
    };

    struct RotateDisplay
    {
        RotateDisplay()
            : heading(HEAD_TOP)
        {

        }
        Heading heading;
    };

    /** @brief  Change Device Options */
    struct DeviceOptions
    {
        /**
         *  @note
         *          DisplayOnOff - ON: true, OFF: false
         *          Mute - ON: true, OFF: false
         *          Pause - ON: true, OFF: false
         */
        DeviceOptions()
            : bDisplaySleep(false)
            , bDeviceMute(false)
            , bContentPause(false)
        {

        }
        bool bDisplaySleep;
        bool bDeviceMute;
        bool bContentPause;
    };

    /** @brief */
    struct ConnectWifi
    {
        ConnectWifi()
            : bEnc(false)
        {
        }
        QString ssid;
        QString password;
        bool bEnc;
    };
}

#endif // REMOTESCHEMA_H
