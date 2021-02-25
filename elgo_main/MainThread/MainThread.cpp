#include "MainThread.h"

// EFC
#include "Common/Deifinition.h"
#include "LocalSocketEvent/EFCEvent.h"

//========================================================
MainThread::MainThread()
//========================================================
{
    m_dbCtrl = new MainDBCtrl;
}

//========================================================
MainThread::~MainThread()
//========================================================
{
    delete m_dbCtrl;
    m_dbCtrl = NULL;
}

//========================================================
void MainThread::SetMainEvent(MAIN_EVENT::Event event)\
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
}

//========================================================
void MainThread::ExecRecvProcecssReady()
//========================================================
{
    /**
     *  @brief  receive status of process started
     *  @param  ELGO_PROC::Proc proc
     */
    QByteArray recvBytes;
    QDataStream out(&recvBytes, QIODevice::ReadOnly);
    out.setVersion(QDataStream::Qt_5_12);

    ELGO_PROC::Proc proc;
    out >> proc;

    if(ELGO_PROC::Proc::ELGO_CONTROL == proc)
    {
        // elgo_control : Server Connection

    }
    else if(ELGO_PROC::Proc::ELGO_VIEWER == proc)
    {
        // elgo_viewer : make QrCode
        QByteArray sendBytes;
        const bool bViewerEvent = EFCEvent::SendEvent(ELGO_PROC::Proc::ELGO_VIEWER,
                                                      VIEWER_EVENT::Event::MAKE_QRCODE, sendBytes);
        if(false == bViewerEvent)
        {
            qDebug() << "Error - Event : " << VIEWER_EVENT::Event::MAKE_QRCODE;
        }
    }
    else
    {
        qDebug() << "Unkwon EGLO_PROC " << proc;
    }
}





