#ifndef WIFIDEF_H
#define WIFIDEF_H

// QT
#include <QString>

#define ROOT_PASSWORD   "elgo2580*"
#define REMOTE_HOST_NAME    "elgo-remote.com"

struct WifiInfo
{
    WifiInfo()
        : freq(0)
        , signal(-100)
        , enc(false)
    {
    }
    int freq;
    int signal;
    QString ssid;
    bool enc;
};

#endif // WIFIDEF_H
