#ifndef REMOTESCHEMA_H
#define REMOTESCHEMA_H

// QT
#include <QString>

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
            ROTATE_DISPLAY_OK =5,
            ROTATE_DISPLAY_FAIL = 6,
            DEVICE_OPTIONS_OK = 7,
            DEVICE_OPTIONS_FAIL = 8
        };

        struct Contents
        {
            Contents()
                : status(NONE_RESULT)
            {

            }
            Status status;
            QString wifiList; // temp
        };
    }

    /**
     * @brief   elgo_remote -> elgo_control
     */
    enum Action
    {
        NONE_ACTION = 0,
        DEVICE_LOGIN = 1,
        LOAD_WIFI_LIST = 2,
        MANAGE_DEVICE = 3,
        ROTATE_DISPLAY = 4,
        DEVICE_OPTIONS = 5
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
        DeviceOptions()
            : bIsDisplaySleep(false)
            , deviceMute(false)
            , contentPause(false)
        {

        }
        bool bIsDisplaySleep;
        bool deviceMute;
        bool contentPause;
    };

    /** @brief  */
    struct Summary
    {
        Action action;
        DeviceLogin deviceLogin;
        ManageDevice mangeDevice;
        RotateDisplay rotateDisplay;
    };
}

#endif // REMOTESCHEMA_H
