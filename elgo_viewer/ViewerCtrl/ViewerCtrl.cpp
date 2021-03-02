#include "ViewerCtrl.h"

// QT
#include <QByteArray>
#include <QDataStream>

// EFC
#include "Common/Deifinition.h"
#include "LocalSocketEvent/EFCEvent.h"

// Contorl
#include "Logger/ViewerLogger.h"

//========================================================
ViewerCtrl::ViewerCtrl()
//========================================================
{

}

//========================================================
ViewerCtrl::~ViewerCtrl()
//========================================================
{

}


//========================================================
void ViewerCtrl::SendViewerIsReady()
//========================================================
{
    /**
     *  @brief  receive status of process started
     *  @param  ELGO_PROC::Proc proc
     */

    QByteArray bytes;
    QDataStream outStream(&bytes, QIODevice::WriteOnly);
    outStream << ELGO_PROC::Proc::ELGO_VIEWER;
    const bool bSendEvent = EFCEvent::SendEvent(ELGO_PROC::Proc::ELGO_MAIN,
                                                MAIN_EVENT::Event::PROCESS_IS_READY, bytes);
    if(false == bSendEvent)
    {
        ELGO_VIEWER_LOG("SendEvent Error %s", MAIN_EVENT::Event::PROCESS_IS_READY);
    }
}
