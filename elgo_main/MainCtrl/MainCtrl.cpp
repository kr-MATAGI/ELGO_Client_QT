// QT
#include <QSysInfo>
#include <QHostAddress>
#include <QNetworkInterface>
#include <QNetworkAccessManager>
#include <QHostInfo>
#include <QStorageInfo>
#include <QNetworkReply>

// Common
#include "Common/CommonDef.h"

// Main
#include "MainCtrl.h"
#include "XML/XMLParser.h"
#include "Logger/MainLogger.h"
#include "Utils/WifiManager.h"

//========================================================
MainCtrl::MainCtrl(QObject *parent)
    : QObject(parent)
      ,m_bDisplaySleep(false)
//========================================================
{

}

//========================================================
MainCtrl::~MainCtrl()
//========================================================
{

}

//========================================================
void MainCtrl::LoadCurrentDeviceInfo()
//========================================================
{

    QString architec = QSysInfo::currentCpuArchitecture();
    QString os = QSysInfo::productType();

    m_deviceInfo.architec = ::DEVICE::ArchitecString2Enum(architec);
    m_deviceInfo.os = ::DEVICE::OSString2Enum(os);
    m_deviceInfo.hostName = QHostInfo::localHostName();;

    // Get Device Storage Info
    QStorageInfo storageInfo= QStorageInfo::root();
    const qint64 totalByte = storageInfo.bytesTotal(); // unit : byte
    const qint64 freeByte = storageInfo.bytesFree(); // unit : byte
    m_deviceInfo.storage.totalStorage = totalByte / 1024 / 1024; // MByte
    m_deviceInfo.storage.freeStorage = freeByte / 1024/ 1024; // MByte
    ELGO_MAIN_LOG("Device - {totalStorage : %d MB, freeStorage : %d MB}",
                  m_deviceInfo.storage.totalStorage,
                  m_deviceInfo.storage.freeStorage);

    // Get Network Address Info
    QList<QHostAddress> allAddr = QNetworkInterface::allAddresses();
    for(int idx = 0; idx < allAddr.count(); idx++)
    {
        if(QAbstractSocket::IPv4Protocol == allAddr[idx].protocol()
                && false == allAddr[idx].isLoopback())
        {
            m_deviceInfo.ipAddr.ip = allAddr[idx].toString();
        }
    }

    foreach (const QNetworkInterface& networkInterface, QNetworkInterface::allInterfaces())
    {
        foreach (const QNetworkAddressEntry& entry, networkInterface.addressEntries())
        {
            if (entry.ip().toString() == m_deviceInfo.ipAddr.ip)
            {
                m_deviceInfo.ipAddr.mac = networkInterface.hardwareAddress();
                break;
            }
        }
    }

    // Add elgo-remote.com to /etc/hosts
    WifiManager::UpdateRemoteServerHost(m_deviceInfo.os, m_deviceInfo.ipAddr.ip);
}

//========================================================
void MainCtrl::CheckDefaultFolder()
//========================================================
{
    // DB
    if(false == QDir().exists(DB_DIR_PATH))
    {
        ELGO_MAIN_LOG("Not existed DB folder: %s",
                      DB_DIR_PATH);
        QDir().mkdir(DB_DIR_PATH);
    }

    // Resource
    if(false == QDir().exists(RESOURCE_DIR_PATH))
    {
        ELGO_MAIN_LOG("Not existed resource folder: %s",
                      RESOURCE_DIR_PATH);
        QDir().mkdir(RESOURCE_DIR_PATH);
    }

    QString iconPath = RESOURCE_DIR_PATH;
    iconPath.append("icon");
    if(false == QDir().exists(iconPath))
    {
        ELGO_MAIN_LOG("Not existed resource folder: %s",
                      iconPath.toStdString().c_str());
        QDir().mkdir(iconPath);
    }

    QString videoPath = RESOURCE_DIR_PATH;
    videoPath.append("video");
    if(false == QDir().exists(videoPath))
    {
        ELGO_MAIN_LOG("Not existed resource folder: %s",
                      videoPath.toStdString().c_str());
        QDir().mkdir(videoPath);
    }

    QString imagePath = RESOURCE_DIR_PATH;
    imagePath.append("image");
    if(false == QDir().exists(imagePath))
    {
        ELGO_MAIN_LOG("Not existed resource folder: %s",
                      imagePath.toStdString().c_str());
        QDir().mkdir(imagePath);
    }
}

//========================================================
void MainCtrl::LoadConfigurationInfo()
//========================================================
{
    // Load Init Configuration XML File
    const bool bIsLoadXML = XMLParser::LoadInitConfigurationXML(m_initConfig);
    ELGO_MAIN_LOG("Result - Load InitConfig XML : %d", bIsLoadXML);
}

//========================================================
void MainCtrl::CheckInternetConnection()
//========================================================
{
    // Just Check Internet Connection
    QNetworkAccessManager nam;
    QNetworkRequest req(QUrl("http://www.google.com"));
    QNetworkReply* reply = nam.get(req);
    QEventLoop loop;
    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    bool bIsConn = reply->bytesAvailable();
    if(false == bIsConn)
    {
        ELGO_MAIN_LOG("Ready to Connect - {ssid: %s, pw: %s}",
                      m_initConfig.internet.ssid.toStdString().c_str(),
                      m_initConfig.internet.pw.toStdString().c_str());
        QString wlanName;
        WifiManager::GetWlanInterfaceName(m_deviceInfo.os, wlanName);
        WifiManager::WakeUpWirelessInterface(m_deviceInfo.os, wlanName);
        WifiManager::ConnectNewWirelessInternet(m_deviceInfo.os,
                                                m_initConfig.internet.ssid,
                                                m_initConfig.internet.pw,
                                                true);

        LoadCurrentDeviceInfo();
    }
    ELGO_MAIN_LOG("Internet Connection - %d", bIsConn);
}

//========================================================
const DEVICE::Info& MainCtrl::GetDeviceInfo()
//========================================================
{
    return m_deviceInfo;
}

//========================================================
void MainCtrl::SetDeviceIP(const QString& src)
//========================================================
{
    m_deviceInfo.ipAddr.ip = src;
}

//========================================================
const DEVICE::INIT_CONFIG& MainCtrl::GetInitConfig()
//========================================================
{
    return m_initConfig;
}

//========================================================
void MainCtrl::CheckDisplaySleepStatus()
//========================================================
{
    QString cmdStr;
    QStringList args;
#if defined(Q_OS_LINUX)
    cmdStr = "/usr/bin/xset";
    args << "-q";
#elif defined(Q_OS_WIN32) || defined(Q_OS_WIN64) || defined(Q_OS_WINRT)

#elif defined(Q_OS_ANDROID)

#else
    ELGO_MAIN_LOG("Error - Not supported OS");
#endif

    ELGO_MAIN_LOG("cmdStr : %s", cmdStr.toStdString().c_str());

    QProcess *process = new QProcess;

    process->start(cmdStr, args);
    process->waitForFinished();

    QByteArray bytes = process->readAllStandardOutput();
    QString outputStr = QString::fromLocal8Bit(bytes);

    // split
    QStringList monitorSplit = outputStr.split("Monitor");
    if(1 <= monitorSplit.size())
    {
        QString monitorStr = monitorSplit[1];
        QStringList onOffSplit = monitorStr.split(" ");
        if(3 <= onOffSplit.size())
        {
            QString onOffStr = onOffSplit.back().remove("\n");

            if(0 == strcmp("On", onOffStr.toStdString().c_str()))
            {
                m_bDisplaySleep = false;
            }
            else
            {
                m_bDisplaySleep = true;
            }
        }
    }

    ELGO_MAIN_LOG("IsDisplaySleep Status : %d", m_bDisplaySleep);

    process->deleteLater();
}

//========================================================
void MainCtrl::SetDisplaySleepStatus(const bool newValue)
//========================================================
{
    m_bDisplaySleep = newValue;
}

//========================================================
bool MainCtrl::GetDisplaySleepStatus()
//========================================================
{
    return m_bDisplaySleep;
}

//========================================================
void MainCtrl::SetDeviceWlanName(const QString& src)
//========================================================
{
    m_wlanName = src;
}

//========================================================
const QString& MainCtrl::GetDeviceWlanName()
//========================================================
{
    return m_wlanName;
}

//========================================================
QString MainCtrl::MakeProcessPath(::ELGO_SYS::Proc proc)
//========================================================
{
    QString basePath = m_initConfig.binaryPath;
    if(0 >= basePath.size())
    {
        ELGO_MAIN_LOG("Not Loaded basepath, use the default path");
#if defined (Q_OS_LINUX) || defined (Q_OS_UNIX)
        basePath = "/opt/ELGO/";
#elif defined (Q_OS_WIN32) || defined(Q_OS_WIN64) || defined(Q_OS_WINRT)
        basePath = "C:/Project/Qt/build-ELGO_Client-Desktop_Qt_5_15_2_MinGW_32_bit-Debug/";
#endif
    }

#if defined (Q_OS_LINUX) || defined (Q_OS_UNIX)
    basePath += ::ELGO_SYS::ELGOProc_enum2str[proc];
#elif defined (Q_OS_WIN32) || defined(Q_OS_WIN64) || defined(Q_OS_WINRT)
    basePath += ::ELGO_PROC::ELGOProc_enum2str[proc];
    basePath += "/debug/";
    basePath += ::ELGO_PROC::ELGOProc_enum2str[proc];
    basePath += ".exe";
#endif

    return basePath;
}
