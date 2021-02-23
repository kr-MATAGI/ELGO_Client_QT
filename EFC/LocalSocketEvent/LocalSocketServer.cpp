#include "LocalSocketServer.h"

#include <QLocalSocket>
#include <QDataStream>

LocalSocketServer::LocalSocketServer(::ELGO_PROC::Proc proc, QObject *parent)
    : QObject(parent)
{
    m_procName = ::ELGO_PROC::ELGOProc_enum2str[proc];
    if(m_server.listen(m_procName))
    {
        qDebug() << "Listening...";
        connect(&m_server, SIGNAL(newConnection()), this, SLOT(clientConnection()));
    }
}

LocalSocketServer::~LocalSocketServer()
{
    for(QLocalSocket *socket : m_connectedList)
    {
        socket->close();
        socket->deleteLater();
    }
}

void LocalSocketServer::clientConnection()
{
    while(m_server.hasPendingConnections())
    {
        AddToList(m_server.nextPendingConnection());
    }
}

void LocalSocketServer::AddToList(QLocalSocket *socket)
{

    m_connectedList.push_back(socket);
    connect(socket, &QLocalSocket::readyRead, this, &LocalSocketServer::readSocket);
    connect(socket, &QLocalSocket::disconnected, this, &LocalSocketServer::discardSocket);
}

void LocalSocketServer::readSocket()
{
    QLocalSocket *socket = (QLocalSocket*)sender();
    QByteArray recvBytes = socket->readAll();

    qDebug() << recvBytes.toStdString().c_str();
}

void LocalSocketServer::discardSocket()
{

}
