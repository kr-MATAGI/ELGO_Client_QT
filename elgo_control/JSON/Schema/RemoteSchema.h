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
            MANAGE_DEVICE_FAIL = 4
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
        MANAGE_DEVICE = 3
    };

    /** @brief  Device Login */
    struct DeviceLogin
    {
        QString id;
        QString pw;
    };

    /** @brief  Mange Device */
    struct MangeDevice
    {
        QString oldPw;
        QString newPw;
    };

    /** @brief  */
    struct Summary
    {
        Action action;
        DeviceLogin deviceLogin;
        MangeDevice mangeDevice;
    };
}

#endif // REMOTESCHEMA_H
