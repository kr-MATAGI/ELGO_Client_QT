#include "mainctrl.h"

#include <QSysInfo>
#include <QHostAddress>
#include <QNetworkInterface>
#include <QHostInfo>

//========================================================
MainCtrl::MainCtrl()
//========================================================
{
    memset(m_procStatus, false, sizeof(m_procStatus));
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

    // Get Network Address Info
    QList<QHostAddress> hostList = QHostInfo::fromName(m_deviceInfo.hostName).addresses();
    foreach (const QHostAddress& address, hostList) {
        if (address.protocol() == QAbstractSocket::IPv4Protocol && address.isLoopback() == false) {
            m_deviceInfo.ip = address.toString();
        }
    }

    foreach (const QNetworkInterface& networkInterface, QNetworkInterface::allInterfaces()) {
        foreach (const QNetworkAddressEntry& entry, networkInterface.addressEntries()) {
            if (entry.ip().toString() == m_deviceInfo.ip) {
                m_deviceInfo.mac = networkInterface.hardwareAddress();
                m_deviceInfo.netMask = entry.netmask().toString();

                break;
            }
        }
    }
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
    QString basePath = "C:/Project/Qt/build-ELGO_Client-Desktop_Qt_5_15_2_MinGW_64_bit-Release/EFC/release/";
    basePath += ::ELGO_PROC::ELGOProc_enum2str[proc];
    basePath += ".exe";
    return basePath;
}
