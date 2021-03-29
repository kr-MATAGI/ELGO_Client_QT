// QT
#include <QLocalSocket>
#include <QDataStream>

// EFC
#include "LocalSocketServer.h"
#include "Logger/ELogger.h"

//========================================================
LocalSocketServer::LocalSocketServer(::ELGO_SYS::Proc proc, QObject *parent)
    : QObject(parent)
//========================================================
{
    m_procName = ::ELGO_SYS::ELGOProc_enum2str[proc];

    m_server.removeServer(m_procName);
    if(m_server.listen(m_procName))
    {
        qDebug() << m_procName <<"Socket is Listening...";
        connect(&m_server, SIGNAL(newConnection()), this, SLOT(ClientConnectionSlot()));
    }
    else
    {
        qDebug() << "Error - [" + m_procName + "] is LocalSocketServer :" << m_server.errorString();
    }
}

//========================================================
LocalSocketServer::~LocalSocketServer()
//========================================================
{
    for(QLocalSocket *socket : m_connectedList)
    {
        socket->close();
        socket->deleteLater();
    }
    m_server.close();
}

//========================================================
void LocalSocketServer::AddToList(QLocalSocket *socket)
//========================================================
{

    m_connectedList.push_back(socket);
    connect(socket, &QLocalSocket::readyRead, this, &LocalSocketServer::ReadEventSlot);
    connect(socket, &QLocalSocket::disconnected, this, &LocalSocketServer::DiscardSocketSlot);
}

//========================================================
void LocalSocketServer::ClientConnectionSlot()
//========================================================
{
    while(m_server.hasPendingConnections())
    {
        AddToList(m_server.nextPendingConnection());
    }
}

//========================================================
void LocalSocketServer::DiscardSocketSlot()
//========================================================
{

}
