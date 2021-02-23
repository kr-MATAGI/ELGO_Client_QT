#include "LocalSocketClient.h"

#include <QDataStream>

LocalSocketClient::LocalSocketClient(QObject *parent)
    : QObject(parent)
{
    m_socket = new QLocalSocket(this);

    connect(m_socket, &QLocalSocket::disconnected, this, &LocalSocketClient::discardSocket);
}

LocalSocketClient::~LocalSocketClient()
{
    if(m_socket)
    {
        m_socket->close();
    }
}

void LocalSocketClient::discardSocket()
{
    m_socket->deleteLater();
    m_socket = NULL;
}

void LocalSocketClient::SendEvent(::ELGO_PROC::Proc proc, QByteArray &src)
{
    QString procName = ::ELGO_PROC::ELGOProc_enum2str[proc];

    if(m_socket)
    {
        m_socket->connectToServer(procName);
        if(m_socket->isOpen())
        {
            QDataStream out(&src, QIODevice::WriteOnly);
            out.setVersion(QDataStream::Qt_5_12);
            m_socket->write(src);
        }
        else
        {
            qDebug() << "No Open !";
        }
    }
    else
    {
        qDebug() << "Not Connected !";
    }
}
