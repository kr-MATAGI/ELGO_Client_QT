#ifndef WIFIDEF_H
#define WIFIDEF_H

// QT
#include <QString>

struct WifiInfo
{
    WifiInfo()
        : freq(0)
        , signal(-100)
        , bEnc(false)
    {
    }
    QString ssid;
    int freq;
    int signal;
    bool bEnc;
};

#endif // WIFIDEF_H
