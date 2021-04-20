// QT
#include <QDataStream>

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
