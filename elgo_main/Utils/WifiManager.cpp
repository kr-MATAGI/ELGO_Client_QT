
// QT
#include <QProcess>

// Main
#include "WifiManager.h"
#include "Logger/MainLogger.h"

//========================================================
void WifiManager::GetWlanInterfaceName(const DEVICE::OS os, QString& dest)
//========================================================
{
    QProcess *process = new QProcess;

    QString cmd;
    QStringList args;
    if( (DEVICE::OS::LINUX == os) || (DEVICE::OS::UBUNTU == os) )
    {
        cmd = "/bin/sh";
        args << "-c";
        args << "iw dev | grep Interface";
    }
    else if( (DEVICE::OS::WINDOWS == os) || (DEVICE::OS::WINRT == os) )
    {
        //
    }
    else if(DEVICE::OS::ANDROID == os)
    {
        //
    }

    process->start(cmd, args);
    process->waitForFinished();

    QByteArray bytes = process->readAllStandardOutput();
    QString byteStr = QString::fromLocal8Bit(bytes);
    ELGO_MAIN_LOG("output: %s", byteStr.toStdString().c_str());

    // parsing
    byteStr.remove("\t");
    byteStr.remove("\n");
    QStringList byteSplit = byteStr.split(" ");
    if(1 <= byteSplit.size())
    {
        dest = byteSplit[1];
    }

    process->deleteLater();
}

//========================================================
void WifiManager::GetAcessibleWifiList(const DEVICE::OS os, const QString& wlanName, QVector<WifiInfo>& dest)
//========================================================
{
    QProcess *process = new QProcess;

    QString cmd;
    QStringList args;
    QString scanArg;
    if( (DEVICE::OS::LINUX == os) || (DEVICE::OS::UBUNTU == os) )
    {
        cmd = "/bin/sh";
        scanArg = QString("echo %1 | sudo -S iw %2 scan | egrep 'SSID:|freq:|signal:|RSN:|\\* non-GF present:'")
                .arg(ROOT_PASSWORD)
                .arg(wlanName);
        args << "-c";
        args << scanArg;
    }
    else if( (DEVICE::OS::WINDOWS == os) || (DEVICE::OS::WINRT == os) )
    {
        //
    }
    else if(DEVICE::OS::ANDROID == os)
    {
        //
    }
    ELGO_MAIN_LOG("ScanArg: %s", scanArg.toStdString().c_str());

    // start
    process->start(cmd, args);
    process->waitForFinished();

    // output
    QByteArray bytes = process->readAllStandardOutput();
    QString byteStr = QString::fromLocal8Bit(bytes);
    ELGO_MAIN_LOG("output: %s", byteStr.toStdString().c_str());

    // parsing
    if( (DEVICE::OS::LINUX == os) || (DEVICE::OS::UBUNTU == os) )
    {
        ParsingLinuxString(byteStr, dest);
    }

    // print
    foreach(const WifiInfo& wifi, dest)
    {
        ELGO_MAIN_LOG("ssid: %s, freq: %d, signal: %d, enc: %d",
                      wifi.ssid.toStdString().c_str(),
                      wifi.freq, wifi.signal, wifi.enc);
    }

    process->deleteLater();
}

//========================================================
void WifiManager::ConnectNewWirelessInternet(const DEVICE::OS os, const QString& wlanName, const QString& ssid,
                                             const QString& password, const bool enc)
//========================================================
{
    QProcess *process = new QProcess;

    QString cmd;
    QStringList args;
    QString connectArg;

    if( (DEVICE::OS::LINUX == os) || (DEVICE::OS::UBUNTU == os) )
    {
        cmd = "/bin/sh";

        if(false == enc)
        {
            connectArg = QString("echo %1 | sudo -S iw %2 connect %3")
                    .arg(ROOT_PASSWORD)
                    .arg(wlanName)
                    .arg(ssid);
        }
        else
        {

        }

        args << "-c";
        args << connectArg;
    }
    else if( (DEVICE::OS::WINDOWS == os) || (DEVICE::OS::WINRT == os) )
    {
        //
    }
    else if(DEVICE::OS::ANDROID == os)
    {
        //
    }
    ELGO_MAIN_LOG("ConnectArg : %s", connectArg.toStdString().c_str());

    // start
    process->start(cmd, args);
    process->waitForFinished();

    // output
    QByteArray bytes = process->readAllStandardOutput();
    QString byteStr = QString::fromLocal8Bit(bytes);
    ELGO_MAIN_LOG("output: %s", byteStr.toStdString().c_str());

    process->deleteLater();
}

//========================================================
void WifiManager::WakeUpWirelessInterface(const DEVICE::OS os, const QString& wlanName)
//========================================================
{
    QProcess *process = new QProcess;

    QString cmd;
    QStringList args;
    QString wakeUpArg;

    if( (DEVICE::OS::LINUX == os) || (DEVICE::OS::UBUNTU == os) )
    {
        cmd = "/bin/sh";
        wakeUpArg = QString("echo %1 | sudo -S ip link set %2 up")
                .arg(ROOT_PASSWORD)
                .arg(wlanName);

        args << "-c";
        args << wakeUpArg;
    }
    else if( (DEVICE::OS::WINDOWS == os) || (DEVICE::OS::WINRT == os) )
    {
        //
    }
    else if(DEVICE::OS::ANDROID == os)
    {
        //
    }
    ELGO_MAIN_LOG("WakeUpArg: %s", wakeUpArg.toStdString().c_str());

    // start
    process->start(cmd, args);
    process->waitForFinished();
    process->deleteLater();
}

//========================================================
void WifiManager::ParsingLinuxString(const QString& src, QVector<WifiInfo>& dest)
//========================================================
{
    QString copySrc = src;
    copySrc.remove("\t");
    copySrc.remove("*");
    copySrc.remove(" ");
    QStringList srcSplit = copySrc.split("\n");
    QVector<QString> srcVec = srcSplit.toVector();

    QVector<QString>::iterator srcIter = srcVec.begin();
    for(; srcIter != srcVec.end(); )
    {
        WifiInfo wifi;

        // freq
        QString freqStr = *srcIter;
        if(0 == freqStr.length())
        {
            break;
        }

        QStringList freqSplit = freqStr.split(":");
        QString freqKey = freqSplit[0];
        QString freqVal = freqSplit[1];
        if(0 == strcmp("freq", freqKey.toStdString().c_str()))
        {
            wifi.freq = freqVal.toInt();
            ++srcIter;
        }

        // signal
        QString signalStr = *srcIter;
        if(0 == signalStr.length())
        {
            break;
        }

        QStringList signalSplit = signalStr.split(":");
        QString signalKey = signalSplit[0];
        QString signalVal = signalSplit[1];
        if(0 == strcmp("signal", signalKey.toStdString().c_str()))
        {
            signalVal.remove("dBm");
            wifi.signal = static_cast<int>(signalVal.toDouble());
            ++srcIter;
        }

        // SSID
        QString ssidStr = *srcIter;
        if(0 == ssidStr.length())
        {
            break;
        }

        QStringList ssidSplit = ssidStr.split(":");
        QString ssidKey = ssidSplit[0];
        QString ssidVal = ssidSplit[1];
        if(0 == strcmp("SSID", ssidKey.toStdString().c_str()))
        {
            wifi.ssid = ssidVal;
            ++srcIter;
        }

        // RSN or non-GFpresent
        QString rsnOrNonGfStr = *srcIter;
        if(0 == rsnOrNonGfStr.length())
        {
            break;
        }

        QStringList rsnOrNonGfSplit = rsnOrNonGfStr.split(":");
        QString rsnOrNonGfKey = rsnOrNonGfSplit[0];
        QString rsnOrNonGfVal = rsnOrNonGfSplit[1];
        if(0 == strcmp("RSN", rsnOrNonGfKey.toStdString().c_str()))
        {
            wifi.enc = true;
            ++srcIter;
        }
        else if(0 == strcmp("non-GFpresent", rsnOrNonGfKey.toStdString().c_str()))
        {
            const bool nonGf = static_cast<bool>(rsnOrNonGfVal.toInt());
            wifi.nonGF = nonGf;
            ++srcIter;
        }

        rsnOrNonGfStr = *srcIter;
        if(0 == rsnOrNonGfStr.length())
        {
            break;
        }

        rsnOrNonGfSplit = rsnOrNonGfStr.split(":");
        rsnOrNonGfKey = rsnOrNonGfSplit[0];
        rsnOrNonGfVal = rsnOrNonGfSplit[1];
        if(0 == strcmp("RSN", rsnOrNonGfKey.toStdString().c_str()))
        {
            wifi.enc = true;
            ++srcIter;
        }
        else if(0 == strcmp("non-GFpresent", rsnOrNonGfKey.toStdString().c_str()))
        {
            const bool nonGf = static_cast<bool>(rsnOrNonGfVal.toInt());
            wifi.nonGF = nonGf;
            ++srcIter;
        }

        if(true == wifi.nonGF)
        {
            dest.push_back(wifi);
        }
    }
}
