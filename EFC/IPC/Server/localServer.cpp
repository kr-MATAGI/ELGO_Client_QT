#include "LocalServer.h"

#include <QLocalSocket>

//========================================================
LocalServer::LocalServer(QObject *parent)
    : QLocalServer(parent)
//========================================================
{
    m_server = new QLocalServer(this);
}

//========================================================
LocalServer::~LocalServer()
//========================================================
{
    delete m_server;
    m_server = NULL;
}

//========================================================
void LocalServer::ClientConnection()
//========================================================
{
    QByteArray writeData;
    writeData.append("connected !");

    QLocalSocket *clientCon;
    clientCon = m_server->nextPendingConnection();
    clientCon->write(writeData, writeData.size());
    clientCon->flush();
    clientCon->disconnectFromServer();

    delete clientCon;
    clientCon = NULL;
}

//========================================================
void LocalServer::SetServerName(const QString &name)
//========================================================
{
    m_name = name;
}

//========================================================
void LocalServer::LocalServerStart(QString &name)
//========================================================
{
    if(false == m_server->listen(name))
    {
        qDebug() << "Failed Local Server Start - " << name;
        m_server->close();
        return;
    }

    connect(m_server, &QLocalServer::newConnection, this, &LocalServer::ClientConnection);
}
