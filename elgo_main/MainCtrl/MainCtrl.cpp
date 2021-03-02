// QT
#include <QSysInfo>
#include <QHostAddress>
#include <QNetworkInterface>
#include <QHostInfo>
#include <QStorageInfo>

// EFC
#include "ShardMem/ShmCtrl.h"

// Main
#include "MainCtrl.h"
#include "XML/XMLParser.h"
#include "Logger/MainLogger.h"

//========================================================
MainCtrl::MainCtrl()
//========================================================
{
    m_dbCtrl = new MainDBCtrl;
    m_ipShm = new ShmCtrl;
}

//========================================================
MainCtrl::~MainCtrl()
//========================================================
{
    delete m_dbCtrl;
    m_dbCtrl = NULL;

    delete m_ipShm;
    m_ipShm = NULL;
}

//========================================================
MainDBCtrl& MainCtrl::GetDBCtrl()
//========================================================
{
    return *m_dbCtrl;
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
    ELGO_MAIN_LOG("Device - totalStorage : %d, freeStorage : %d", m_deviceInfo.storage.totalStorage, m_deviceInfo.storage.freeStorage);

    // Get Network Address Info (Wired Internet)
    QList<QHostAddress> hostList = QHostInfo::fromName(m_deviceInfo.hostName).addresses();
    foreach (const QHostAddress& address, hostList) {
        if (address.protocol() == QAbstractSocket::IPv4Protocol && address.isLoopback() == false) {
            m_deviceInfo.ipAddr.ip = address.toString();
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

    // Save IP addres into shared Memory
    m_deviceInfo.ipAddr.ip = "localhost"; // test
    QByteArray shmBytes;
    QDataStream shmDataStream(&shmBytes, QIODevice::WriteOnly);
    shmDataStream << m_deviceInfo.ipAddr.ip;
    const bool bWriteShmIP = m_ipShm->ShmWrite(SHM_NAME::SHM_IP, shmBytes);
    if(false == bWriteShmIP)
    {
        ELGO_MAIN_LOG("Error Write IP into SHM");
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

    // TODO : will be share Config Info
}

//========================================================
const DEVICE::Info& MainCtrl::GetDeviceInfo()
//========================================================
{
    return m_deviceInfo;
}

//========================================================
QString MainCtrl::MakeProcessPath(::ELGO_PROC::Proc proc)
//========================================================
{
    // TODO : load configuration path
    QString basePath;
#if defined (Q_OS_LINUX) || defined (Q_OS_UNIX)
    basePath = "/home/jaehoon/바탕화면/ELGO/build-ELGO_Client-Desktop_Qt_5_15_2_GCC_64bit-Release/";
    basePath += ::ELGO_PROC::ELGOProc_enum2str[proc];
    basePath += "/";
    basePath += ::ELGO_PROC::ELGOProc_enum2str[proc];
#elif defined (Q_OS_WIN32) || defined(Q_OS_WIN64) || defined(Q_OS_WINRT)
    basePath = "C:/Project/Qt/build-ELGO_Client-Desktop_Qt_5_15_2_MinGW_32_bit-Debug/";
    basePath += ::ELGO_PROC::ELGOProc_enum2str[proc];
    basePath += "/debug/";
    basePath += ::ELGO_PROC::ELGOProc_enum2str[proc];
    basePath += ".exe";
#endif

    return basePath;
}
