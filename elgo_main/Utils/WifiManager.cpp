
// QT
#include <QProcess>
#include <QRegularExpression>

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
        // public exception - "echo %1 | sudo -S iw %2 scan | egrep 'SSID:|freq:|signal:|RSN:|\\* non-GF present:'"
        scanArg = QString("echo %1 | sudo -S iw %2 scan | egrep 'SSID:|freq:|signal:|RSN:'")
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
bool WifiManager::ConnectNewWirelessInternet(const DEVICE::OS os, const QString& wlanName, const QString& ssid,
                                             const QString& password, const bool enc)
//========================================================
{
    bool retValue = false;

    QProcess *process = new QProcess;

    QString cmd;
    QStringList args;
    QString connectArg;

    if( (DEVICE::OS::LINUX == os) || (DEVICE::OS::UBUNTU == os) )
    {
        cmd = "/bin/sh";

        if(false == enc)
        {
            connectArg = QString("nmcli dev wifi con %1")
                        .arg(ssid);
        }
        else
        {
            connectArg = QString("nmcli dev wifi con %1 password %2")
                        .arg(ssid)
                        .arg(password);
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

    if( (DEVICE::OS::LINUX == os) || (DEVICE::OS::UBUNTU == os) )
    {
        if(0 < byteStr.length())
        {
            retValue = true;
        }
    }
    else if( (DEVICE::OS::WINDOWS == os) || (DEVICE::OS::WINRT == os) )
    {
        //
    }
    else if(DEVICE::OS::ANDROID == os)
    {
        //
    }

    process->deleteLater();

    ELGO_MAIN_LOG("return value : %d", retValue);
    return retValue;
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
            ConvertUtf8ToKR(ssidVal, wifi.ssid);
            ++srcIter;
        }

        // RSN
        QString rsnStr = *srcIter;
        if(0 == rsnStr.length())
        {
            break;
        }

        QStringList rsnSplit = rsnStr.split(":");
        QString rsnKey = rsnSplit[0];
        if(0 == strcmp("RSN", rsnKey.toStdString().c_str()))
        {
            wifi.enc = true;
            ++srcIter;
        }

        dest.push_back(wifi);
    }
}

//========================================================
void WifiManager::ConvertUtf8ToKR(const QString&src, QString& dest)
//========================================================
{
    const QStringList& srcSplit = src.split(QRegularExpression("\\\\"));

    char hangulBuf[4] = {0,};
    int hanIdx = 0;
    foreach (const QString& item, srcSplit)
    {
        if('x' == item[0])
        {
            char hexBuf[8] = {0,};
            QString hexSlice = item[1];
            hexSlice += item[2];
            sprintf(hexBuf, "0x%s", hexSlice.toStdString().c_str());

            int utf8 = static_cast<int>(std::strtol(hexBuf, NULL, 0));
            hangulBuf[hanIdx] = static_cast<char>(utf8);

            if(2 == hanIdx)
            {
                dest.append(hangulBuf);

                memset(hangulBuf, '\0', sizeof(hangulBuf));
                hanIdx = 0;
                if(3 <= item.length())
                {
                    std::string remainStr = item.toStdString().substr(3);
                    dest.append(remainStr.c_str());
                }
            }
            else
            {
                hanIdx++;
            }
        }
        else
        {
            dest.append(item);
        }
    }

    ELGO_MAIN_LOG("Converted: %s -> %s",
                  src.toStdString().c_str(), dest.toStdString().c_str());
}
