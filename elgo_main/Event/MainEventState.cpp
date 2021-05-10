// QT
#include <QDataStream>

// Common
#include "Common/Interface/ScheduleImpl.h"
#include "Common/Interface/ContentsPlayDataImpl.h"
#include "LocalSocketEvent/EFCEvent.h"

// Main
#include "MainEventState.h"
#include "MainThread/MainThread.h"
#include "Logger/MainLogger.h"
#include "Utils/DeviceManager.h"

//========================================================
MainEventState::MainEventState()
//========================================================
{
    m_threadPool = new QThreadPool;
    m_threadPool->setMaxThreadCount(MAX_THREAD_COUNT);

    // enroll event
    m_state.RegisterEvent(MAIN_EVENT::Event::PROCESS_IS_READY,
                          &MainEventState::RecvProcecssReady);

    m_state.RegisterEvent(MAIN_EVENT::Event::UPDATE_DEVICE_OPTIONS,
                          &MainEventState::RecvUpdateDeviceOptions);
    m_state.RegisterEvent(MAIN_EVENT::Event::UPDATE_DISPLAY_SLEEP,
                          &MainEventState::RecvUpdateDisplaySleep);

    m_state.RegisterEvent(MAIN_EVENT::Event::SYSTEM_REBOOT_MAIN,
                          &MainEventState::RecvSystemReboot);

    m_state.RegisterEvent(MAIN_EVENT::Event::SEARCHING_WIFI_LIST,
                          &MainEventState::RecvSearchingWifiList);
    m_state.RegisterEvent(MAIN_EVENT::Event::CONNECT_NEW_WIFI,
                          &MainEventState::RecvConnectNewWifi);

    m_state.RegisterEvent(MAIN_EVENT::Event::UPDATE_PLAY_SCHEDULE_LIST,
                          &MainEventState::RecvUpdatePlaySchedule);
    m_state.RegisterEvent(MAIN_EVENT::Event::CLEARE_ALL_PLAY_SCHEDULE_LIST,
                          &MainEventState::RecvClearAllPlaySchedule);
    m_state.RegisterEvent(MAIN_EVENT::Event::DELETE_PLAY_SCHEDULE_BY_ID,
                          &MainEventState::RecvDeletePlayScheduleById);

    m_state.RegisterEvent(MAIN_EVENT::Event::ADD_PLAY_DATA_TO_DB,
                          &MainEventState::RecvAddPlayDataToDB);
    m_state.RegisterEvent(MAIN_EVENT::Event::SAVE_PLAYING_DATA_TO_DB,
                          &MainEventState::RecvSavePlayingDataToDB);

    m_state.RegisterEvent(MAIN_EVENT::Event::UPDATE_POWER_SCHEDULE_LIST,
                          &MainEventState::RecvUpdatePowerSchedule);
    m_state.RegisterEvent(MAIN_EVENT::Event::DELETE_POWER_SCHEDULE_BY_ID,
                          &MainEventState::RecvDeletePowerScheduleById);

    m_state.RegisterEvent(MAIN_EVENT::Event::MAIN_ROTATE_SCREEN,
                          &MainEventState::RecvRotateScreen);
}

//========================================================
MainEventState::~MainEventState()
//========================================================
{

}

//========================================================
void MainEventState::ExecState(const quint16 event, const QByteArray &src)
//========================================================
{
    m_state.Exec(event, src);;
}

//========================================================
void MainEventState::RecvProcecssReady(const QByteArray &src)
//========================================================
{
    /**
     *  @note
     *          ELGO_VIEWER, CONTROL -> ELGO_MAIN
     *          receive status of process started
     *  @param
     *          ELGO_PROC::Proc proc
     */
    MainThread *thread = new MainThread;
    thread->SetMainEvent(MAIN_EVENT::Event::PROCESS_IS_READY);
    thread->SetRecvBytes(src);
    m_threadPool->start(thread);
}

//========================================================
void MainEventState::RecvUpdateDeviceOptions(const QByteArray& src)
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

    QByteArray copyBytes = src;
    QDataStream copySteam(&copyBytes, QIODevice::ReadOnly);
    bool displaySleep = false;
    bool deviceMute = false;
    bool contentPause = false;

    copySteam >> displaySleep;
    copySteam >> deviceMute;
    copySteam >> contentPause;

    // Sleep Status
    const bool bCurrDisplaySleep = MainController::GetInstance()->GetMainCtrl().GetDisplaySleepStatus();
    ELGO_MAIN_LOG("Recv Value - {displaySleep: %d, currDisplaySleep: %d, deviceMute: %d, contentPause: %d}",
                  displaySleep, bCurrDisplaySleep, deviceMute, contentPause);

    const DEVICE::OS os = MainController::GetInstance()->GetMainCtrl().GetDeviceInfo().os;
    if(bCurrDisplaySleep != displaySleep)
    {

        DeviceManager::UpdateSleepStatus(os, displaySleep);
        MainController::GetInstance()->GetMainCtrl().SetDisplaySleepStatus(displaySleep);
    }

    // Mute
    DeviceManager::DeviceMute(os, deviceMute);

    // Pause
    QByteArray viewerBytes;
    QDataStream viewerStream(&viewerBytes, QIODevice::WriteOnly);
    viewerStream << contentPause;
    const bool bViewerEvent = EFCEvent::SendEvent(ELGO_SYS::Proc::ELGO_VIEWER,
                                                  VIEWER_EVENT::Event::UPDATE_PLAYER_PAUSE_STATUS,
                                                  viewerBytes);
    if(false == bViewerEvent)
    {
        ELGO_MAIN_LOG("ERROR - Send Event: %d", VIEWER_EVENT::Event::UPDATE_PLAYER_PAUSE_STATUS);
    }
}

//========================================================
void MainEventState::RecvUpdateDisplaySleep(const QByteArray& src)
//========================================================
{
    /**
     *  @note
     *          ELGO_CONTROL -> ELGO_MAIN
     *          Change display sleep status
     *  @param
     *          bool bDisplaySleep
     */

    QByteArray bytes = src;
    QDataStream stream(&bytes, QIODevice::ReadOnly);
    bool bDisplaySleep = false;
    stream >> bDisplaySleep;

    MainController::GetInstance()->GetMainCtrl().SetDisplaySleepStatus(bDisplaySleep);
    ELGO_MAIN_LOG("Display Sleep: %d", bDisplaySleep);

    const DEVICE::OS os = MainController::GetInstance()->GetMainCtrl().GetDeviceInfo().os;
    DeviceManager::UpdateSleepStatus(os, bDisplaySleep);
}

//========================================================
void MainEventState::RecvSystemReboot(const QByteArray& src)
//========================================================
{
    /**
     *  @note
     *          ELGO_CONTROL -> ELGO_MAIN
     *          System Reboot
     *  @param
     *          NONE
     */

    ELGO_MAIN_LOG("System Reboot Start (After 5 sec)!");
    const DEVICE::OS os = MainController::GetInstance()->GetMainCtrl().GetDeviceInfo().os;
    DeviceManager::SystemReboot(os);
}

//========================================================
void MainEventState::RecvSearchingWifiList(const QByteArray& src)
//========================================================
{
    /**
     *  @note
     *          ELGO_CONTROL -> ELGO_MAIN
     *          Search available wifi list
     *  @param
     *          NONE
     */

    MainThread *newThread = new MainThread;
    newThread->SetMainEvent(MAIN_EVENT::Event::SEARCHING_WIFI_LIST);
    m_threadPool->start(newThread);
}

//========================================================
void MainEventState::RecvConnectNewWifi(const QByteArray& src)
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

    MainThread *newThread = new MainThread;
    newThread->SetMainEvent(MAIN_EVENT::Event::CONNECT_NEW_WIFI);
    newThread->SetRecvBytes(src);
    m_threadPool->start(newThread);
}

//========================================================
void MainEventState::RecvUpdatePlaySchedule(const QByteArray& src)
//========================================================
{
    /**
     *  @note
     *          ELGO_CONTROL -> ELGO_MAIN
     *          For manage custom/fixed play schedule
     *  @param
     *          QVector<ScheduleJson::PlaySchedule> playScheduleList
     */

    QByteArray copyBytes = src;
    QDataStream dataStream(&copyBytes, QIODevice::ReadOnly);

    PlayJson::PlayDataType type = PlayJson::PlayDataType::NONE_PLAY_DATA_TYPE;
    dataStream >> type;

    if(PlayJson::PlayDataType::CUSTOM == type)
    {
        PlayJson::CustomPlayDataJson customPlayData;
        QVector<ScheduleJson::PlaySchedule> playScheduleList;

        dataStream >> customPlayData;
        dataStream >> playScheduleList;

        MainController::GetInstance()->GetDBCtrl().AddNewPlayDataToDB(customPlayData);
        MainController::GetInstance()->GetPlayTimer().AddPlayScheduleList(playScheduleList);
    }
    else if(PlayJson::PlayDataType::FIXED == type)
    {
        PlayJson::FixedPlayDataJson fixedPlayData;
        QVector<ScheduleJson::PlaySchedule> playScheduleList;

        dataStream >> fixedPlayData;
        dataStream >> playScheduleList;

        MainController::GetInstance()->GetDBCtrl().AddNewPlayDataToDB(fixedPlayData);
        MainController::GetInstance()->GetPlayTimer().AddPlayScheduleList(playScheduleList);
    }
    else
    {
        ELGO_MAIN_LOG("ERROR - None Play Data Type: %d", type);
    }
}

//========================================================
void MainEventState::RecvClearAllPlaySchedule(const QByteArray& src)
//========================================================
{
    /**
     *  @note
     *          ELGO_CONTROL -> ELGO_MAIN
     *          Clear All Play Schedule List
     *          Cause by single play event
     *  @param
     *          NONE
     */

    MainController::GetInstance()->GetDBCtrl().ClearAllPlaySchedule();
    MainController::GetInstance()->GetPlayTimer().ClearAllPlayScheduleList();
}

//========================================================
void MainEventState::RecvDeletePlayScheduleById(const QByteArray& src)
//========================================================
{
    /**
     *  @note
     *          ELGO_CONTROL -> ELGO_MAIN
     *          Clear Play Schedule ID by clearPlaySchedule Event
     *  @param
     *          QString scheduleId
     */

    QByteArray copyBytes = src;
    QDataStream dataStream(&copyBytes, QIODevice::ReadOnly);
    QString id;
    dataStream >> id;
    ELGO_MAIN_LOG("Ready to Delete - {id: %s}", id.toStdString().c_str());

    MainController::GetInstance()->GetDBCtrl().DeletePlayScheduleById(id);
    MainController::GetInstance()->GetPlayTimer().DeletePlayScheduleById(id);
}

//========================================================
void MainEventState::RecvAddPlayDataToDB(const QByteArray& src)
//========================================================
{
    /**
     *  @note
     *          ELGO_CONTROL -> ELGO_MAIN
     *          Add PlayData to DB
     *          Cause by single play event
     *  @param
     *          PlayJson::PlayDataType  playJson
     *          [ CustomPlayDataJson  customPlayData ||
     *            FixedPlayDataJson   fixedPlayData ]
     */

    QByteArray copyBytes = src;
    QDataStream dataStream(&copyBytes, QIODevice::ReadOnly);
    PlayJson::PlayDataType playType = PlayJson::PlayDataType::NONE_PLAY_DATA_TYPE;
    dataStream >> playType;

    if(PlayJson::PlayDataType::CUSTOM == playType)
    {
        PlayJson::CustomPlayDataJson customPlayData;
        dataStream >> customPlayData;

        MainController::GetInstance()->GetDBCtrl().AddNewPlayDataToDB(customPlayData);
        ELGO_MAIN_LOG("Add Custom PlayData to DB - {id: %d, type: %d}",
                      customPlayData.playData.id, customPlayData.playData.playDataType);
    }
    else if(PlayJson::PlayDataType::FIXED == playType)
    {
        PlayJson::FixedPlayDataJson fixedPlayData;
        dataStream >> fixedPlayData;

        MainController::GetInstance()->GetDBCtrl().AddNewPlayDataToDB(fixedPlayData);
        ELGO_MAIN_LOG("ADD Fixed PlayData to DB - {id: %d, type: %d}",
                      fixedPlayData.playData.id, fixedPlayData.playData.playDataType);
    }
    else
    {
        ELGO_MAIN_LOG("ERROR - Unknown PlayData Type: %d", playType);
    }
}

//========================================================
void MainEventState::RecvSavePlayingDataToDB(const QByteArray& src)
//========================================================
{
    /**
     *  @note
     *          ELGO_VIEWER -> ELGO_MAIN
     *          Save Current Playing PlayData to DB
     *  @param
     *          int playDataId
     *          PlayJson::PlayDataType type
     */

    QByteArray copyBytes = src;
    QDataStream copyStream(&copyBytes, QIODevice::ReadOnly);

    int playDataId = 0;
    PlayJson::PlayDataType playDataType = PlayJson::PlayDataType::NONE_PLAY_DATA_TYPE;

    copyStream >> playDataId;
    copyStream >> playDataType;
    ELGO_MAIN_LOG("Save to DB - playData{id: %d, type: %d}", playDataId, playDataType);

    // Save to DB
    MainController::GetInstance()->GetDBCtrl().UpdatePlayingData(playDataId, playDataType);
    MainController::GetInstance()->GetPlayTimer().UpdatePlayingData(playDataId, playDataType);
}

//========================================================
void MainEventState::RecvUpdatePowerSchedule(const QByteArray& src)
//========================================================
{
    /**
     *  @note
     *          ELGO_CONTROL -> ELGO_MAIN
     *          Update Power Schedule
     *  @param
     *          ScheduleJson::PowerSchedule powerSchedule
     */

    QByteArray copyBytes = src;
    QDataStream dataStream(&copyBytes, QIODevice::ReadOnly);

    ScheduleJson::PowerSchedule powerSchedule;
    dataStream >> powerSchedule;

    MainController::GetInstance()->GetDBCtrl().UpdateNewPowerSchedule(powerSchedule.scheduleList);
    MainController::GetInstance()->GetPowerTimer().AddPowerScheduleList(powerSchedule.scheduleList);
}

//========================================================
void MainEventState::RecvDeletePowerScheduleById(const QByteArray& src)
//========================================================
{
    /**
     *  @note
     *          ELGO_CONTROL -> ELGO_MAIN
     *          Delete Power Schedule by ID
     *  @param
     *          QString scheduleId
     */

    QByteArray copyBytes = src;
    QDataStream dataStream(&copyBytes, QIODevice::ReadOnly);

    QString scheduleId;
    dataStream >> scheduleId;
    ELGO_MAIN_LOG("Ready to Delete - {id: %s}", scheduleId.toStdString().c_str());

    MainController::GetInstance()->GetDBCtrl().DeletePowerScheduleById(scheduleId);
    MainController::GetInstance()->GetPowerTimer().DeletePowerScheduleById(scheduleId);
}

//========================================================
void MainEventState::RecvRotateScreen(const QByteArray& src)
//========================================================
{
    /**
     * @note
     *          ELGO_CONTROL -> ELGO_MAIN
     *          Exec Screen Rotation Command line
     * @param
     *          quint8  heading
     */
    QByteArray copyBytes = src;
    QDataStream copySteam(&copyBytes, QIODevice::ReadOnly);
    quint8 heading = 0;
    copySteam >> heading;

    const DEVICE::OS os = MainController::GetInstance()->GetMainCtrl().GetDeviceInfo().os;
    DeviceManager::RotateScreen(os, heading);
}
