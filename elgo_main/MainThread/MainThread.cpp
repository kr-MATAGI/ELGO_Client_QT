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
    else
    {
        qDebug() << __FUNCTION__ << "Unkwon Event";
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
    out.setVersion(QDataStream::Qt_5_12);

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
        // elgo_viewer : make QrCode
        /**
        * @note
        *       ELGO_MAIN -> ELGO_VIEWER
        *       Viewer will make qr code image and display.
        * @param
        *       QString ip
        */

        QString ip = MainController::GetInstance()->GetMainCtrl().GetDeviceInfo().ipAddr.ip;
        QByteArray sendBytes;
        QDataStream sendStream(&sendBytes, QIODevice::WriteOnly);
        sendStream << ip;
        const bool bViewerEvent = EFCEvent::SendEvent(ELGO_PROC::Proc::ELGO_VIEWER,
                                                      VIEWER_EVENT::Event::MAKE_QRCODE, sendBytes);
        if(false == bViewerEvent)
        {
            ELGO_MAIN_LOG("SendEvent Error - %d", VIEWER_EVENT::Event::MAKE_QRCODE);
        }
    }
    else
    {
        ELGO_MAIN_LOG("Unkwon ELGO_PROC %d", proc);
    }
}
