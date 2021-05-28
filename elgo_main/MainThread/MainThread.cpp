// STL
#include <unordered_map>

// EFC
#include "LocalSocketEvent/EFCEvent.h"

// Common
#include "Common/CommonDef.h"
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
    ELGO_MAIN_LOG("%s proccess is Ready !",
                  ELGO_SYS::ELGOProc_enum2str[proc].toStdString().c_str());

    if(ELGO_SYS::Proc::ELGO_CONTROL == proc)
    {
        // Server [host, port] info send to elgo_control
        /**
         *  @note
         *          ELGO_MAIN -> ELGO_CONTROL
         *          receive server connection information.
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

        // Get prev playing Data and
        int playingId = 0;
        PlayJson::PlayDataType playingType = PlayJson::PlayDataType::NONE_PLAY_DATA_TYPE;
        MainController::GetInstance()->GetDBCtrl().GetPlayingData(playingId, playingType);
        ELGO_MAIN_LOG("Prev Playing Data - {id: %d, type: %d}", playingId, playingType);

        // Get Play Schedule List
        QVector<ScheduleJson::PlaySchedule> playScheduleList;
        MainController::GetInstance()->GetDBCtrl().GetAllPlayScheduleList(playScheduleList);

        // Delete - Not need play Data
        QVector<std::pair<int, PlayJson::PlayDataType>> needPlayDataList;
        needPlayDataList.push_back(std::make_pair(playingId, playingType));

        QVector<ScheduleJson::PlaySchedule>::const_iterator playScheduleIter = playScheduleList.constBegin();
        while(playScheduleIter != playScheduleList.constEnd())
        {
            if(false == playScheduleIter->scheduleList.empty())
            {
                const int playDataId = playScheduleIter->scheduleList[0].playDataId;
                const PlayJson::PlayDataType playDataType = playScheduleIter->scheduleList[0].type;

                needPlayDataList.push_back(std::make_pair(playDataId, playDataType));
            }

            ++playScheduleIter;
        }

        QVector<PlayJson::CustomPlayDataJson>::iterator customIter = customDataList.begin();
        while(customIter != customDataList.end())
        {
            bool bIsNeed = false;
            QVector<std::pair<int, PlayJson::PlayDataType>>::const_iterator needIter = needPlayDataList.constBegin();
            while(needIter != needPlayDataList.constEnd())
            {
                if( (customIter->playData.id == needIter->first) &&
                    (customIter->playData.playDataType == needIter->second) )
                {
                    bIsNeed = true;
                    break;
                }

                ++needIter;
            }

            if(false == bIsNeed)
            {
                MainController::GetInstance()->GetDBCtrl().DeletePlayData(customIter->playData.id,
                                                                          customIter->playData.playDataType);
                customIter = customDataList.erase(customIter);
            }
            else
            {
                ++customIter;
            }
        }

        QVector<PlayJson::FixedPlayDataJson>::iterator fixedIter = fixedDataList.begin();
        while(fixedIter != fixedDataList.end())
        {
            bool bIsNeed = false;
            QVector<std::pair<int, PlayJson::PlayDataType>>::const_iterator needIter = needPlayDataList.constBegin();
            while(needIter != needPlayDataList.constEnd())
            {
                if( (fixedIter->playData.id == needIter->first) &&
                    (fixedIter->playData.playDataType == needIter->second) )
                {
                    bIsNeed = true;
                    break;
                }

                ++needIter;
            }
            if(false == bIsNeed)
            {
                MainController::GetInstance()->GetDBCtrl().DeletePlayData(fixedIter->playData.id,
                                                                          fixedIter->playData.playDataType);
                fixedIter = fixedDataList.erase(fixedIter);
            }
            else
            {
                ++fixedIter;
            }
        }

        // Check Resource Data
        CheckValidResourceFile(customDataList, fixedDataList);

        // Send Play Data to Viewer
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


        // Send prev PlayingData Info to Viewer
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

        // Set playSchedule
        if(0 < playScheduleList.size())
        {
            emit MainController::GetInstance()->GetPlayTimer().AddPlayScheduleListSignal(playScheduleList);
        }

        // Get power schedule
        ScheduleJson::PowerSchedule powerScheduleList;
        MainController::GetInstance()->GetDBCtrl().GetAllPowerScheduleList(powerScheduleList);
        if(0 < powerScheduleList.scheduleList.size())
        {
            emit MainController::GetInstance()->GetPowerTimer().AddPowerScheduleListSignal(powerScheduleList.scheduleList);
        }
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
    ELGO_MAIN_LOG("os: %s",
                  DEVICE::OS_enum2str[os].toStdString().c_str());
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
     *       Finish searching wifi list.
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
    const bool bIsConnect = WifiManager::ConnectNewWirelessInternet(os, ssid, password, bEnc);

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
void MainThread::CheckValidResourceFile(const QVector<PlayJson::CustomPlayDataJson>& customSrc,
                                        const QVector<PlayJson::FixedPlayDataJson>& fixedSrc)
//========================================================
{
    // QVector<std::pair<PlayJson::MediaType, fileName>>
    std::unordered_map<std::string, PlayJson::MediaType> reSrcList;

    // custom play data
    QVector<PlayJson::CustomPlayDataJson>::const_iterator customIter = customSrc.constBegin();
    while(customIter != customSrc.constEnd())
    {
        const QVector<PlayJson::PageData>& pageDataList = customIter->pageDataList;

        QVector<PlayJson::PageData>::const_iterator pageIter = pageDataList.constBegin();
        while(pageIter != pageDataList.constEnd())
        {
            const QVector<PlayJson::CustomLayerData>& layerDataList = pageIter->layerDataList;

            QVector<PlayJson::CustomLayerData>::const_iterator layerIter = layerDataList.constBegin();
            while(layerIter != layerDataList.constEnd())
            {
                const PlayJson::MediaType mediaType = layerIter->layerContent.contentInfo.mediaType;
                const std::string& fileName = layerIter->layerContent.name.toStdString();

                reSrcList.insert(std::make_pair(fileName, mediaType));

                ++layerIter;
            }

            ++pageIter;
        }

        ++customIter;
    }

    // fixed play data
    QVector<PlayJson::FixedPlayDataJson>::const_iterator fixedIter = fixedSrc.constBegin();
    while(fixedIter != fixedSrc.constEnd())
    {
        const QVector<PlayJson::FixedLayerData>& layerDataList = fixedIter->layerDataList;

        QVector<PlayJson::FixedLayerData>::const_iterator layerIter = layerDataList.constBegin();
        while(layerIter != layerDataList.constEnd())
        {
            const QVector<PlayJson::ContentData>& contentDataList = layerIter->contentDataList;

            QVector<PlayJson::ContentData>::const_iterator contentIter = contentDataList.constBegin();
            while(contentIter != contentDataList.constEnd())
            {
                const PlayJson::MediaType mediaType = contentIter->contentInfo.mediaType;
                const std::string& fileName = contentIter->name.toStdString();

                reSrcList.insert(std::make_pair(fileName, mediaType));

                ++contentIter;
            }

            ++layerIter;
        }

        ++fixedIter;
    }

    // Local Resource Folder
    // image
    QString imagePath = RESOURCE_DIR_PATH;
    imagePath += "image/";
    QDir imageDir(imagePath);
    const QStringList& imageFileList = imageDir.entryList();
    foreach(auto file, imageFileList)
    {
        if( (0 == strcmp(".", file.toStdString().c_str())) ||
            (0 == strcmp("..", file.toStdString().c_str())) )
        {
            continue;
        }

        if(0 == reSrcList.count(file.toStdString()))
        {
            QString filePath = imagePath;
            filePath += file;

            QFile::remove(filePath);
            ELGO_MAIN_LOG("Delete Resrouce File - path: %s",
                          filePath.toStdString().c_str());
        }
    }

    // video
    QString videoPath = RESOURCE_DIR_PATH;
    videoPath += "video/";
    QDir videoDir(videoPath);
    const QStringList& videoFileList = videoDir.entryList();
    foreach(auto file, videoFileList)
    {
        if( (0 == strcmp(".", file.toStdString().c_str())) ||
            (0 == strcmp("..", file.toStdString().c_str())) )
        {
            continue;
        }

        if(0 == reSrcList.count(file.toStdString()))
        {
            QString filePath = videoPath;
            filePath += file;

            QFile::remove(filePath);
            ELGO_MAIN_LOG("Delete Resrouce File - path: %s",
                          filePath.toStdString().c_str());
        }
    }
}
