// QT
#include <QDebug>

// EFC
#include "Common/Deifinition.h"
#include "LocalSocketEvent/EFCEvent.h"
#include "ShardMem/ShmCtrl.h"

// Main
#include "Logger/MainLogger.h"
#include "MainThread.h"

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
    else if(MAIN_EVENT::Event::CHANGE_DEVICE_OPTIONS == m_event)
    {
        ExecChangeDeviceOptions();
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
     *  @brief  receive status of process started
     *  @param
     *          ELGO_PROC::Proc proc
     */
    QByteArray recvBytes = m_bytes;
    QDataStream out(&recvBytes, QIODevice::ReadOnly);

    ELGO_PROC::Proc proc = ELGO_PROC::Proc::NONE_PROC;
    out >> proc;
    ELGO_MAIN_LOG("%s proccess is Ready !", ELGO_PROC::ELGOProc_enum2str[proc]);

    if(ELGO_PROC::Proc::ELGO_CONTROL == proc)
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
        QByteArray sendBytes;
        QDataStream sendStream(&sendBytes, QIODevice::WriteOnly);
        sendStream << initConfig.server.wasHost;
        sendStream << initConfig.server.wasHostPort;
        sendStream << hostAddr.ip;

        const bool bContorlEvent = EFCEvent::SendEvent(ELGO_PROC::ELGO_CONTROL,
                            CONTROL_EVENT::Event::RECV_SERVER_INFO_FROM_MAIN, sendBytes);
        if(false == bContorlEvent)
        {
            ELGO_MAIN_LOG("SendEvent Error - %d", CONTROL_EVENT::Event::RECV_SERVER_INFO_FROM_MAIN);
        }
    }
    else if(ELGO_PROC::Proc::ELGO_VIEWER == proc)
    {
        // Not thing.
    }
    else
    {
        ELGO_MAIN_LOG("Unkwon ELGO_PROC %d", proc);
    }
}

//========================================================
void MainThread::ExecChangeDeviceOptions()
//========================================================
{
    /**
     *  @note
     *          ELGO_CONTROL -> ELGO_MAIN
     *          Change Device Options
     *  @param
     *          bool displayOnOff
     *          bool deviceMute
     *          bool contentPause
     */
    QByteArray recvBytes = m_bytes;
    QDataStream out(&recvBytes, QIODevice::ReadOnly);
    bool displayOnOff = false;
    bool deviceMute = false;
    bool contentPause = false;

    out >> displayOnOff;
    out >> deviceMute;
    out >> contentPause;

    // Set Display Sleep to Main Ctrl
    const bool bCurDisplaySleepStatus = MainController::GetInstance()->GetMainCtrl().GetDisplaySleepStatus();
    if(bCurDisplaySleepStatus != displayOnOff)
    {
        /**
         *  @note
         *          ELGO_MAIN -> ELGO_CONTROL
         *          Update display sleep status
         *  @param
         *          bool isDisplaySleep
         */
        QByteArray sendBytes;
        QDataStream sendStream(&sendBytes, QIODevice::WriteOnly);
        sendStream << displayOnOff;

        MainController::GetInstance()->GetMainCtrl().SetDisplaySleepStatus(displayOnOff);
        const bool bSendEvent = EFCEvent::SendEvent(ELGO_PROC::Proc::ELGO_CONTROL,
                                                    CONTROL_EVENT::Event::UPDATE_DISPLAY_SLEEP_STATUS, sendBytes);
        if(false == bSendEvent)
        {
            ELGO_MAIN_LOG("Error - SendEvent : %d", CONTROL_EVENT::Event::UPDATE_DISPLAY_SLEEP_STATUS);
        }
    }

    ELGO_MAIN_LOG("TEST RECV : %d %d %d", displayOnOff, deviceMute, contentPause);
}
