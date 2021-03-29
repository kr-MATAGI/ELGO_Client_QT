// QT
#include <QLocalSocket>
#include <QDataStream>

// Contorl
#include "ControlEventHandler.h"

//========================================================
ControlEventHandler::ControlEventHandler(ELGO_SYS::Proc proc, QObject *parent)
    : LocalSocketServer(proc, parent)
//========================================================
{

}

//========================================================
ControlEventHandler::~ControlEventHandler()
//========================================================
{

}

//========================================================
void ControlEventHandler::ReadEventSlot()
//========================================================
{
    QLocalSocket *socket = (QLocalSocket *)sender();
    QByteArray recvBytes = socket->readAll();
    QDataStream recvStream(&recvBytes, QIODevice::ReadOnly);
    recvStream.setVersion(QDataStream::Qt_5_12);

    quint16 event = MAIN_EVENT::Event::NONE_MAIN_EVENT;
    QByteArray innerBytes;
    recvStream >> event;
    recvStream >> innerBytes;

    m_state.ExecState(event, innerBytes);
}
