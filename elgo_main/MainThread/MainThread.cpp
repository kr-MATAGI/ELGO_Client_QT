// QT
#include <QDebug>

// EFC
#include "Common/Deifinition.h"
#include "LocalSocketEvent/EFCEvent.h"
#include "ShardMem/ShmCtrl.h"

// Main
#include "MainThread.h"
#include "Logger/MainLogger.h"
#include "Utils/WifiManager.h"

//========================================================
MainThread::MainThread()
//========================================================
{

}

//========================================================
MainThread::~MainThread()
//========================================================
{

}

//========================================================
void MainThread::SetMainEvent(const MAIN_EVENT::Event event)
//========================================================
{
    m_event = event;
}

//========================================================
void MainThread::SetRecvBytes(const QByteArray &src)
//========================================================
{
    m_bytes = src;
}

//========================================================
void MainThread::run()
//========================================================
{
    if(MAIN_EVENT::Event::PROCESS_IS_READY == m_event)
    {
        ExecRecvProcecssReady();
    }
    else if(MAIN_EVENT::Event::UPDATE_DEVICE_OPTIONS == m_event)
    {
        ExecUpdateDeviceOptions();
    }
    else if(MAIN_EVENT::Event::SEARCHING_WIFI_LIST == m_event)
    {
        ExecSearchingWifiList();
    }
    else
    {
        ELGO_MAIN_LOG("Error - Unkown Event (%d)", m_event);
    }
}

//========================================================
void MainThread::ExecRecvProcecssReady()
//========================================================
{
    /**
     *  @note
     *          ELGO_VIEWER, CONTROL -> ELGO_MAIN
     *          receive status of process started
     *  @param
     *          ELGO_PROC::Proc proc
     */
    QByteArray recvBytes = m_bytes;
    QDataStream out(&recvBytes, QIODevice::ReadOnly);

    ELGO_SYS::Proc proc = ELGO_SYS::Proc::NONE_PROC;
    out >> proc;
    ELGO_MAIN_LOG("%s proccess is Ready !", ELGO_SYS::ELGOProc_enum2str[proc]);

    if(ELGO_SYS::Proc::ELGO_CONTROL == proc)
    {
        // Server [host, port] info send to elgo_control
        /**
         *  @brief  receive WIFI information from main process
         *  @param
         *          QString wasHost,
         *          int wasHostPort,
         *          QString remoteTCPHost
         */

        const DEVICE::INIT_CONFIG &initConfig = MainController::GetInstance()->GetMainCtrl().GetInitConfig();
        const DEVICE::IPADDR &hostAddr = MainController::GetInstance()->GetMainCtrl().GetDeviceInfo().ipAddr;
        const bool bIsDisplaySleep = MainController::GetInstance()->GetMainCtrl().GetDisplaySleepStatus();
        QByteArray sendBytes;
        QDataStream sendStream(&sendBytes, QIODevice::WriteOnly);
        sendStream << initConfig.server.wasHost;
        sendStream << initConfig.server.wasHostPort;
        sendStream << hostAddr.ip;
        sendStream << bIsDisplaySleep;

        const bool bContorlEvent = EFCEvent::SendEvent(ELGO_SYS::ELGO_CONTROL,
                            CONTROL_EVENT::Event::RECV_SERVER_INFO_FROM_MAIN, sendBytes);
        if(false == bContorlEvent)
        {
            ELGO_MAIN_LOG("SendEvent Error - %d", CONTROL_EVENT::Event::RECV_SERVER_INFO_FROM_MAIN);
        }
    }
    else if(ELGO_SYS::Proc::ELGO_VIEWER == proc)
    {
        // Not thing.
    }
    else
    {
        ELGO_MAIN_LOG("Unkwon ELGO_PROC %d", proc);
    }
}

//========================================================
void MainThread::ExecUpdateDeviceOptions()
//========================================================
{
    /**
     *  @note
     *          ELGO_CONTROL -> ELGO_MAIN
     *          Change Device Options
     *  @param
     *          bool displaySleep
     *          bool deviceMute
     *          bool contentPause
     */
    QByteArray recvBytes = m_bytes;
    QDataStream out(&recvBytes, QIODevice::ReadOnly);
    bool displaySleep = false;
    bool deviceMute = false;
    bool contentPause = false;
    const bool bCurrDisplaySleep = MainController::GetInstance()->GetMainCtrl().GetDisplaySleepStatus();

    out >> displaySleep;
    out >> deviceMute;
    out >> contentPause;
    ELGO_MAIN_LOG("Recv Value - {displaySleep: %d, currDisplaySleep: %d, deviceMute: %d, contentPause: %d",
                  displaySleep, bCurrDisplaySleep, deviceMute, contentPause);

    // Set Display Sleep to Main Ctrl
    if(bCurrDisplaySleep != displaySleep)
    {
        MainController::GetInstance()->GetMainCtrl().SetDisplaySleepStatus(displaySleep);

            // process
        QProcess *process = new QProcess;
        QString cmdStr;
        QStringList args;

#if defined(Q_OS_LINUX)
        cmdStr = "/usr/bin/xset";
        args << "-display";
        args << ":0.0";
        args << "dpms";
        args << "force";

        if(true == displaySleep)
        {
            args << "off";
        }
        else
        {
            args << "on";
        }

#elif defined(Q_OS_WIN32) || defined(Q_OS_WIN64) || defined(Q_OS_WINRT)

#else defined(Q_OS_ANDROID)

#endif

        process->start(cmdStr, args);
        process->waitForFinished();
        ELGO_MAIN_LOG("cmdStr: %s, args: %s",
                        cmdStr.toStdString().c_str(), args.back().toStdString().c_str());
    }
    else
    {
        ELGO_MAIN_LOG("Not Changed Display Sleep Status - {recv: %d, curr: %d}",
                      displaySleep, bCurrDisplaySleep);
    }
}

//========================================================
void MainThread::ExecSearchingWifiList()
//========================================================
{
    /**
     *  @note
     *          ELGO_CONTROL -> ELGO_MAIN
     *          Search available wifi list
     *  @param
     *          NONE
     */

    DEVICE::OS os = MainController::GetInstance()->GetMainCtrl().GetDeviceInfo().os;
    ELGO_MAIN_LOG("os: %s", DEVICE::OS_enum2str[os]);
    QString wlanName;

    // wlan name
    WifiManager::GetWlanInterfaceName(os, wlanName);
    ELGO_MAIN_LOG("WLAN Name : %s", wlanName.toStdString().c_str());

    // get wifi list
    QVector<WifiInfo> wifiList;
    WifiManager::GetAcessibleWifiList(os, wlanName, wifiList);
}
