#ifndef REMOTESCHEMA_H
#define REMOTESCHEMA_H

// QT
#include <QString>

namespace Remote
{
    // elgo_remote -> elgo_control
    enum Action
    {
        NONE_ACTION = 0,
        DEVICE_LOGIN = 1,
    };

    struct DeviceJson
    {
        QString id;
        QString pw;
    };

    struct UserJson
    {
        QString id;
        QString pw;
    };

    struct Summary
    {
        Action action;
        DeviceJson device;
        UserJson user;
    };

    // elgo_control -> elgo_remote
}

#endif // REMOTESCHEMA_H
