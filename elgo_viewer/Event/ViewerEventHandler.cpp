#include "ViewerEventHandler.h"

#include <QLocalSocket>
#include <QDataStream>
#include <LocalSocketEvent/EFCEvent.h>

//========================================================
ViewerEventHandler::ViewerEventHandler(ELGO_SYS::Proc name, QObject *parent)
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
void ViewerEventHandler::ReadEventSlot()
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
