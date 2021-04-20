#ifndef WIFIDEF_H
#define WIFIDEF_H

// QT
#include <QString>

struct WifiInfo
{
    WifiInfo()
        : freq(0)
        , signal(0)
        , enc(false)
        , nonGF(true)
    {
    }
    int freq;
    int signal;
    QString ssid;
    bool enc;
    bool nonGF;
};

#endif // WIFIDEF_H
