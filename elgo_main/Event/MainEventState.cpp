// QT
#include <QDataStream>

// Common
#include "Common/Interface/ScheduleImpl.h"
#include "Common/Interface/ContentsPlayDataImpl.h"

// Main
#include "MainEventState.h"
#include "MainThread/MainThread.h"
#include "Logger/MainLogger.h"

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
    m_state.RegisterEvent(MAIN_EVENT::Event::CLEAR_PLAY_SCHEDULE_BY_ID,
                          &MainEventState::RecvClearPlayScheduleById);

    m_state.RegisterEvent(MAIN_EVENT::Event::ADD_PLAY_DATA_TO_DB,
                          &MainEventState::RecvAddPlayDataToDB);

    m_state.RegisterEvent(MAIN_EVENT::Event::UPDATE_POWER_SCHEDULE_LIST,
                          &MainEventState::RecvUpdatePowerSchedule);
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
     *  @brief  receive status of process started
     *  @param  ELGO_PROC::Proc proc
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
     *  @brief  Change Device Options
     *  @param
     *          bool displayOnOff
     *          bool deviceMute
     *          bool contentPause
     */

    MainThread *thread = new MainThread;
    thread->SetMainEvent(MAIN_EVENT::Event::UPDATE_DEVICE_OPTIONS);
    thread->SetRecvBytes(src);
    m_threadPool->start(thread);
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

    if(true == bDisplaySleep)
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

    // process
    QProcess *process = new QProcess;
    QString cmdStr;
    QStringList args;
#if defined(Q_OS_LINUX)
    cmdStr = "reboot";

#elif defined(Q_OS_WIN32) || defined(Q_OS_WIN64) || defined(Q_OS_WINRT)

#else defined(Q_OS_ANDROID)

#endif

    ELGO_MAIN_LOG("SYSTEM REBOOT !");
    process->start(cmdStr, args);
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

    MainController::GetInstance()->GetPlayTimer().ClearAllPlayScheduleList();
}

//========================================================
void MainEventState::RecvClearPlayScheduleById(const QByteArray& src)
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

    MainController::GetInstance()->GetPlayTimer().ClearPlayScheduleById(id);
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

    // on
    MainController::GetInstance()->GetDBCtrl().UpdateNewPowerSchedule(powerSchedule.onScheduleList,
                                                                      ScheduleJson::PowerStatus::POWER_ON);
    MainController::GetInstance()->GetPowerTimer().AddPowerScheduleList(powerSchedule.onScheduleList);

    // off
    MainController::GetInstance()->GetDBCtrl().UpdateNewPowerSchedule(powerSchedule.offScheduleList,
                                                                      ScheduleJson::PowerStatus::POWER_OFF);
    MainController::GetInstance()->GetPowerTimer().AddPowerScheduleList(powerSchedule.offScheduleList);
}
