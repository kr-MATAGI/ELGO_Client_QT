#include "EFCEvent.h"

#include <QDebug>
#include <QDataStream>

//========================================================
bool EFCEvent::SendEvent(const ELGO_SYS::Proc proc, const quint16 event, const QByteArray &src)
//========================================================
{
    bool retValue = true; // TODO : change to eunm of error (for details)

    QLocalSocket *socket = new QLocalSocket;
    QString procName = ::ELGO_SYS::ELGOProc_enum2str[proc];

    if(socket)
    {
        socket->connectToServer(procName);
        if(socket->waitForConnected())
        {
            const bool bIsSend = SendMessage(socket, event, src);
            if(false == bIsSend)
            {
                qDebug() << "Socket send() Error";
            }
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
bool EFCEvent::SendMessage(QLocalSocket* socket, const quint16 event, const QByteArray &src)
//========================================================
{
    bool retValue = true;

    QByteArray bytes;
    QDataStream out(&bytes, QIODevice::WriteOnly);
    out << event;
    out << src;

    const qint64 result = socket->write(bytes);
    socket->waitForBytesWritten();
    if(-1 == result)
    {
        retValue = false;
        qDebug() << "Socket Write Error";
    }

    return retValue;
}
