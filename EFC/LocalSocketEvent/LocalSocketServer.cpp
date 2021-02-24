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

    m_socket = new QLocalSocket();
    connect(m_socket, &QLocalSocket::disconnected, this, &LocalSocketServer::discardSocket);
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

    if(m_socket)
    {
        m_socket->close();
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
bool LocalSocketServer::SendEvent(ELGO_PROC::Proc proc, quint16 event, QByteArray &src)
//========================================================
{
    bool retValue = true; // TODO : change to eunm of error (for details)

    QString procName = ::ELGO_PROC::ELGOProc_enum2str[proc];

    if(m_socket)
    {
        m_socket->connectToServer(procName);
        if(m_socket->isOpen())
        {
            QByteArray bytes;
            QDataStream out(&bytes, QIODevice::WriteOnly);
            out.setVersion(QDataStream::Qt_5_12);
            out << event;
            out << src;

            const qint64 result = m_socket->write(bytes);
            m_socket->waitForBytesWritten(100);
            if(-1 == result)
            {
                retValue = false;
                qDebug() << "Socket write() Error";
            }
        }
        else
        {
            retValue = false;
            qDebug() << procName << "Socket is not open";
        }
    }
    else
    {
        retValue = false;
        qDebug() << "Scoket is NULL";
    }

    return retValue;
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
void LocalSocketServer::readEvent()
//========================================================
{
    QLocalSocket *socket = (QLocalSocket*)sender();
    QByteArray recvBytes = socket->readAll();
    QDataStream recvStream(&recvBytes, QIODevice::ReadOnly);

    quint8 event = 0;
    recvStream >> event;
    qDebug() << event;

    QByteArray innerBytes;
    recvStream >> innerBytes;
}

//========================================================
void LocalSocketServer::discardSocket()
//========================================================
{
    m_socket->deleteLater();
    m_socket = NULL;
}
