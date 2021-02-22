#include "LocalClient.h"

//========================================================
LocalClient::LocalClient(QObject *parent)
    : QObject(parent)
//========================================================
{
    m_socket = new QLocalSocket(this);

    connect(m_socket, &QLocalSocket::readyRead, this, &LocalClient::ReadData);
}

//========================================================
LocalClient::~LocalClient()
//========================================================
{
    delete m_socket;
    m_socket = NULL;
}

//========================================================
void LocalClient::ReadToReady()
//========================================================
{
    m_socket->connectToServer("main");
}

//========================================================
void LocalClient::ReadData()
//========================================================
{
    QString readData = m_socket->readAll();
    qDebug() << readData;
}
