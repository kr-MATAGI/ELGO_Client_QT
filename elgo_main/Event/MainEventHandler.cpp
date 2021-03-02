// QT
#include <QLocalSocket>
#include <QDataStream>

// Main
#include "MainEventHandler.h"

//========================================================
MainEventHandler::MainEventHandler(ELGO_PROC::Proc name, QObject *parent)
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
void MainEventHandler::readEvent()
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
