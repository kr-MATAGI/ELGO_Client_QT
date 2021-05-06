// QT
#include <QDebug>

// EFC
#include "LocalSocketEvent/EFCEvent.h"
#include "ShardMem/ShmCtrl.h"

// Common
#include "Common/Deifinition.h"
#include "Common/Interface/ContentsPlayDataImpl.h"

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
    else if(MAIN_EVENT::Event::SEARCHING_WIFI_LIST == m_event)
    {
        ExecSearchingWifiList();
    }
    else if(MAIN_EVENT::Event::CONNECT_NEW_WIFI == m_event)
    {
        ExecConnectNewWifi();
    }
    else if(MAIN_EVENT::Event::REQUEST_OFFLINE_SINGLE_PLAY == m_event)
    {
        ExecRequestOfflineSinglePlay();
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
         *  @note
         *          ELGO_MAIN -> ELGO_CONTROL
         *          receive WIFI information from main process
         *  @param
         *          QString wasHost,
         *          quint16 wasHostPort,
         *          QString remoteTCPHost
         *          bool bIsDisplaySleep
         */

        const DEVICE::Info& deviceInfo = MainController::GetInstance()->GetMainCtrl().GetDeviceInfo();
        const DEVICE::INIT_CONFIG &initConfig = MainController::GetInstance()->GetMainCtrl().GetInitConfig();
        const DEVICE::IPADDR &hostAddr = deviceInfo.ipAddr;
        const bool bIsDisplaySleep = MainController::GetInstance()->GetMainCtrl().GetDisplaySleepStatus();
        QByteArray sendBytes;
        QDataStream sendStream(&sendBytes, QIODevice::WriteOnly);
        sendStream << initConfig.server.wasHost;
        sendStream << initConfig.server.wasHostPort;
        sendStream << hostAddr.ip;
        sendStream << bIsDisplaySleep;

        const bool bContorlEvent = EFCEvent::SendEvent(ELGO_SYS::ELGO_CONTROL,
                                                       CONTROL_EVENT::Event::RECV_SERVER_INFO_FROM_MAIN,
                                                       sendBytes);
        if(false == bContorlEvent)
        {
            ELGO_MAIN_LOG("SendEvent Error - %d", CONTROL_EVENT::Event::RECV_SERVER_INFO_FROM_MAIN);
        }
    }
    else if(ELGO_SYS::Proc::ELGO_VIEWER == proc)
    {

    }
    else
    {
        ELGO_MAIN_LOG("ERROR - Unknwon ELGO_PROC %d", proc);
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

    const DEVICE::OS os = MainController::GetInstance()->GetMainCtrl().GetDeviceInfo().os;
    ELGO_MAIN_LOG("os: %s", DEVICE::OS_enum2str[os]);
    QString wlanName;

    // wlan name
    WifiManager::GetWlanInterfaceName(os, wlanName);
    MainController::GetInstance()->GetMainCtrl().SetDeviceWlanName(wlanName);
    ELGO_MAIN_LOG("WLAN Name : %s", wlanName.toStdString().c_str());

    // wake up wlan
    WifiManager::WakeUpWirelessInterface(os, wlanName);

    // get wifi list
    QVector<WifiInfo> wifiList;
    WifiManager::GetAcessibleWifiList(os, wlanName, wifiList);

    // Send Event
    /**
     * @note
     *       ELGO_MAIN -> ELGO_CONTROL
     *       Finish searching wifi, update list
     * @param
     *       int    wifiCnt
     *       [Loop]
     *       QString    ssid
     *       int    freq
     *       int    signal
     *       bool   bEncryption
     *       [End Loop]
     */
    QByteArray bytes;
    QDataStream sendStream(&bytes, QIODevice::WriteOnly);
    const int wifiListCnt = wifiList.size();
    ELGO_MAIN_LOG("Wifi Cnt: %d", wifiListCnt);

    sendStream << wifiListCnt;
    for(int idx = 0; idx < wifiListCnt; idx++)
    {
        sendStream << wifiList[idx].ssid;
        sendStream << wifiList[idx].freq;
        sendStream << wifiList[idx].signal;
        sendStream << wifiList[idx].enc;
    }

    const bool bSendEvent = EFCEvent::SendEvent(ELGO_SYS::Proc::ELGO_CONTROL,
                                                CONTROL_EVENT::Event::UPDATE_WIFI_LIST, bytes);
    if(false == bSendEvent)
    {
        ELGO_MAIN_LOG("Error - SendEvent : %d", CONTROL_EVENT::Event::UPDATE_WIFI_LIST);
    }
}

//========================================================
void MainThread::ExecConnectNewWifi()
//========================================================
{
    /**
     *  @note
     *          ELGO_CONTROL -> ELGO_MAIN
     *          Connect new wifi
     *  @param
     *          QString ssid
     *          QString password
     *          bool encryption
     */
    QDataStream recvStream(&m_bytes, QIODevice::ReadOnly);
    QString ssid;
    QString password;
    bool bEnc = false;

    recvStream >> ssid;
    recvStream >> password;
    recvStream >> bEnc;
    ELGO_MAIN_LOG("Recv new wifi info - {ssid: %s, pw: %s, enc: %d}",
                  ssid.toStdString().c_str(), password.toStdString().c_str(), bEnc);

    const DEVICE::OS os = MainController::GetInstance()->GetMainCtrl().GetDeviceInfo().os;
    const QString& wlanName = MainController::GetInstance()->GetMainCtrl().GetDeviceWlanName();

    const bool bIsConnect = WifiManager::ConnectNewWirelessInternet(os, wlanName, ssid, password, bEnc);

    QString newIP;
    WifiManager::GetDeviceLocalIP(newIP);
    ELGO_MAIN_LOG("New IP : %s", newIP.toStdString().c_str());
    WifiManager::UpdateRemoteServerHost(os, newIP);

    /**
     * @note
     *          ELGO_MAIN -> ELGO_CONTROL
     *          New WIFI connection result
     * @param
     *          bool bIsConnect
     *          QString newIP
     */
    QByteArray sendBytes;
    QDataStream sendStream(&sendBytes, QIODevice::WriteOnly);
    sendStream << bIsConnect;
    sendStream << newIP;
    const bool bSendEvent = EFCEvent::SendEvent(ELGO_SYS::Proc::ELGO_CONTROL,
                                                CONTROL_EVENT::Event::WIFI_CONNECTION_RESULT, sendBytes);
    if(false == bSendEvent)
    {
        ELGO_MAIN_LOG("Error - SendEvent : %d", CONTROL_EVENT::Event::WIFI_CONNECTION_RESULT);
    }
}

//========================================================
void MainThread::ExecRequestOfflineSinglePlay()
//========================================================
{
    /**
     *  @note
     *          ELGO_CONTROL -> ELGO_MAIN
     *          Request data for offline single play
     *  @param
     *          NONE
     */

    // Get PlayData List
    QVector<PlayJson::CustomPlayDataJson> customDataList;
    QVector<PlayJson::FixedPlayDataJson> fixedDataList;
    MainController::GetInstance()->GetDBCtrl().GetAllPlayDataFromDB(customDataList, fixedDataList);

    /**
     * @note
     *       ELGO_MAIN -> ELGO_VIEWER
     *       ADD Custom PlayData after system booting
     * @param
     *       QVector<PlayJson::CustomPlayDataJson>  customPlayDataList
     */
    QByteArray customDataBytes;
    QDataStream customDataStream(&customDataBytes, QIODevice::WriteOnly);
    customDataStream << customDataList;

    const bool bSendCustom = EFCEvent::SendEvent(ELGO_SYS::Proc::ELGO_VIEWER,
                                                 VIEWER_EVENT::Event::ADD_CUSTOM_PLAY_DATA_LIST,
                                                 customDataBytes);
    if(false == bSendCustom)
    {
        ELGO_MAIN_LOG("ERROR - SendEvent: %d", VIEWER_EVENT::Event::ADD_CUSTOM_PLAY_DATA_LIST);
    }

    QByteArray fixedDataBytes;
    QDataStream fixedDataStream(&fixedDataBytes, QIODevice::WriteOnly);
    fixedDataStream << fixedDataList;

    const bool bSendFixed = EFCEvent::SendEvent(ELGO_SYS::Proc::ELGO_VIEWER,
                                                VIEWER_EVENT::Event::ADD_FIXED_PLAY_DATA_LIST,
                                                fixedDataBytes);
    if(false == bSendFixed)
    {
        ELGO_MAIN_LOG("ERROR - SendEvent: %d", VIEWER_EVENT::Event::ADD_FIXED_PLAY_DATA_LIST);
    }

    // Get prev playing Data and
    int playingId = 0;
    PlayJson::PlayDataType playingType = PlayJson::PlayDataType::NONE_PLAY_DATA_TYPE;
    MainController::GetInstance()->GetDBCtrl().GetPlayingData(playingId, playingType);
    ELGO_MAIN_LOG("Prev Playing Data - {id: %d, type: %d}",
                  playingId, playingType);

    if(PlayJson::PlayDataType::CUSTOM == playingType)
    {
        QVector<PlayJson::CustomPlayDataJson>::const_iterator iter = customDataList.constBegin();
        while(iter != customDataList.constEnd())
        {
            if(iter->playData.id == playingId)
            {
                QByteArray playingBytes;
                QDataStream playingStream(&playingBytes, QIODevice::WriteOnly);
                playingStream << *iter;

                const bool bSendPlaying = EFCEvent::SendEvent(ELGO_SYS::Proc::ELGO_VIEWER,
                                                              VIEWER_EVENT::Event::PLAY_CUSTOM_PLAY_DATA,
                                                              playingBytes);
                if(false == bSendPlaying)
                {
                    ELGO_MAIN_LOG("ERROR - SendEvent: %d", VIEWER_EVENT::Event::PLAY_CUSTOM_PLAY_DATA);
                }

                break;
            }

            ++iter;
        }
    }
    else if(PlayJson::PlayDataType::FIXED == playingType)
    {
        QVector<PlayJson::FixedPlayDataJson>::const_iterator iter = fixedDataList.constBegin();
        while(iter != fixedDataList.constEnd())
        {
            if(iter->playData.id == playingId)
            {
                QByteArray playingBytes;
                QDataStream playingStream(&playingBytes, QIODevice::WriteOnly);
                playingStream << *iter;

                const bool bSendPlaying = EFCEvent::SendEvent(ELGO_SYS::Proc::ELGO_VIEWER,
                                                              VIEWER_EVENT::Event::PLAY_FIXED_PLAY_DATA,
                                                              playingBytes);
                if(false == bSendPlaying)
                {
                    ELGO_MAIN_LOG("ERROR - SendEvent: %d", VIEWER_EVENT::Event::PLAY_FIXED_PLAY_DATA);
                }

                break;
            }

            ++iter;
        }
    }
    else
    {
        ELGO_MAIN_LOG("Not Existed Data - type: %d", playingType);
    }

    // Get play and power Schedule
}
