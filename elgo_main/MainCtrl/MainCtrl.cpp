// QT
#include <QSysInfo>
#include <QHostAddress>
#include <QNetworkInterface>
#include <QHostInfo>
#include <QStorageInfo>

// Main
#include "MainCtrl.h"
#include "XML/XMLParser.h"
#include "Logger/MainLogger.h"

//========================================================
MainCtrl::MainCtrl()
    : m_bDisplaySleep(false)
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
    ELGO_MAIN_LOG("Device - totalStorage : %d MB, freeStorage : %d MB",
                  m_deviceInfo.storage.totalStorage, m_deviceInfo.storage.freeStorage);

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

    foreach (const QNetworkInterface& networkInterface, QNetworkInterface::allInterfaces()) {
        foreach (const QNetworkAddressEntry& entry, networkInterface.addressEntries()) {
            if (entry.ip().toString() == m_deviceInfo.ipAddr.ip) {
                m_deviceInfo.ipAddr.mac = networkInterface.hardwareAddress();
                break;
            }
        }
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
const DEVICE::Info& MainCtrl::GetDeviceInfo()
//========================================================
{
    return m_deviceInfo;
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
#if defined(Q_OS_LINUX)
    const QString cmdStr = "/usr/bin/xset";
    QStringList args;
    args << "-q";
#elif defined(Q_OS_WIN32) || defined(Q_OS_WIN64) || defined(Q_OS_WINRT)

#elif defined(Q_OS_ANDROID)

#else
    ELGO_MAIN_LOG("Error - Not supported OS");
#endif

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
QString MainCtrl::MakeProcessPath(::ELGO_SYS::Proc proc)
//========================================================
{
    // TODO : load configuration path
    QString basePath = m_initConfig.binaryPath;
    if(0 >= basePath.size())
    {
        ELGO_MAIN_LOG("Not Loaded basepath, use the default path");
#if defined (Q_OS_LINUX) || defined (Q_OS_UNIX)
        basePath = "/home/jaehoon/바탕화면/ELGO/build-ELGO_Client-Desktop_Qt_5_15_2_GCC_64bit-Release/";
#elif defined (Q_OS_WIN32) || defined(Q_OS_WIN64) || defined(Q_OS_WINRT)
        basePath = "C:/Project/Qt/build-ELGO_Client-Desktop_Qt_5_15_2_MinGW_32_bit-Debug/";
#endif
    }

#if defined (Q_OS_LINUX) || defined (Q_OS_UNIX)
    basePath += ::ELGO_SYS::ELGOProc_enum2str[proc];
    basePath += "/";
    basePath += ::ELGO_SYS::ELGOProc_enum2str[proc];
#elif defined (Q_OS_WIN32) || defined(Q_OS_WIN64) || defined(Q_OS_WINRT)
    basePath += ::ELGO_PROC::ELGOProc_enum2str[proc];
    basePath += "/debug/";
    basePath += ::ELGO_PROC::ELGOProc_enum2str[proc];
    basePath += ".exe";
#endif

    return basePath;
}
