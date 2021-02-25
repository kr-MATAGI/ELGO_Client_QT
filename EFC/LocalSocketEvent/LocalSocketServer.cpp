#include "LocalSocketServer.h"

#include <QLocalSocket>
#include <QDataStream>

//========================================================
LocalSocketServer::LocalSocketServer(::ELGO_PROC::Proc proc, QObject *parent)
    : QObject(parent)
//========================================================
{
    m_procName = ::ELGO_PROC::ELGOProc_enum2str[proc];
    if(m_server.listen(m_procName))
    {
        qDebug() << m_procName <<"Socket is Listening...";
        connect(&m_server, SIGNAL(newConnection()), this, SLOT(clientConnection()));
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
}

//========================================================
void LocalSocketServer::AddToList(QLocalSocket *socket)
//========================================================
{

    m_connectedList.push_back(socket);
    connect(socket, &QLocalSocket::readyRead, this, &LocalSocketServer::readEvent);
    connect(socket, &QLocalSocket::disconnected, this, &LocalSocketServer::discardSocket);
}

//========================================================
void LocalSocketServer::clientConnection()
//========================================================
{
    while(m_server.hasPendingConnections())
    {
        AddToList(m_server.nextPendingConnection());
    }
}

//========================================================
void LocalSocketServer::discardSocket()
//========================================================
{

}
