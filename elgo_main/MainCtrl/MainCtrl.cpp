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
bool MainCtrl::CheckingWirelessInternet()
//========================================================
{
    bool retValue = false;

    // Todo : Wireless Internet Checking code

    return retValue;
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
QString MainCtrl::MakeProcessPath(::ELGO_PROC::Proc proc)
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
    basePath += ::ELGO_PROC::ELGOProc_enum2str[proc];
    basePath += "/";
    basePath += ::ELGO_PROC::ELGOProc_enum2str[proc];
#elif defined (Q_OS_WIN32) || defined(Q_OS_WIN64) || defined(Q_OS_WINRT)
    basePath += ::ELGO_PROC::ELGOProc_enum2str[proc];
    basePath += "/debug/";
    basePath += ::ELGO_PROC::ELGOProc_enum2str[proc];
    basePath += ".exe";
#endif

    return basePath;
}
