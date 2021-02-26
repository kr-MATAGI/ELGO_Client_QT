#include "ViewerEventHandler.h"

#include <QLocalSocket>
#include <QDataStream>
#include <LocalSocketEvent/EFCEvent.h>

//========================================================
ViewerEventHandler::ViewerEventHandler(ELGO_PROC::Proc name, QObject *parent)
    : LocalSocketServer(name, parent)
//========================================================
{

}

//========================================================
ViewerEventHandler::~ViewerEventHandler()
//========================================================
{

}

//========================================================
void ViewerEventHandler::readEvent()
//========================================================
{
    QLocalSocket *socket = (QLocalSocket *)sender();
    QByteArray recvBytes = socket->readAll();
    QDataStream recvStream(&recvBytes, QIODevice::ReadOnly);

    quint16 event = VIEWER_EVENT::Event::NONE_VIEWER_EVENT;
    QByteArray innerBytes;
    recvStream >> event;
    recvStream >> innerBytes;

    m_state.ExecState(event, innerBytes);
}

//========================================================
void ViewerEventHandler::SendViewerIsReady()
//========================================================
{
    QByteArray bytes;
    QDataStream outStream(&bytes, QIODevice::ReadOnly);
    outStream << ELGO_PROC::Proc::ELGO_VIEWER;

    /**
     *  @brief  receive status of process started
     *  @param  ELGO_PROC::Proc proc
     */
    const bool bSendEvent = EFCEvent::SendEvent(ELGO_PROC::Proc::ELGO_MAIN, MAIN_EVENT::Event::PROCESS_IS_READY, bytes);
    if(false == bSendEvent)
    {
        qDebug() << "Error - " << MAIN_EVENT::Event::PROCESS_IS_READY << " SendEvent()";
    }
}
