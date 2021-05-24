// QT
#include <QLocalSocket>
#include <QDataStream>

// Main
#include "MainEventHandler.h"

//========================================================
MainEventHandler::MainEventHandler(ELGO_SYS::Proc name, QObject *parent)
    : LocalSocketServer(name, parent)
//========================================================
{

}

//========================================================
MainEventHandler::~MainEventHandler()
//========================================================
{

}

//========================================================
void MainEventHandler::ReadEventSlot()
//========================================================
{
    QLocalSocket *socket = (QLocalSocket *)sender();
    QByteArray recvBytes = socket->readAll();
    QDataStream recvStream(&recvBytes, QIODevice::ReadOnly);

    quint16 event = MAIN_EVENT::Event::NONE_MAIN_EVENT;
    QByteArray innerBytes;
    recvStream >> event;
    recvStream >> innerBytes;

    m_state.ExecState(event, innerBytes);
}
