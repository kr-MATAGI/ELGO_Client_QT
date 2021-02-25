#include "EFCEvent.h"

#include <QDebug>
#include <QLocalSocket>
#include <QDataStream>

bool EFCEvent::SendEvent(ELGO_PROC::Proc proc, quint16 event, QByteArray &src)
{
    bool retValue = true; // TODO : change to eunm of error (for details)

    QLocalSocket *socket = new QLocalSocket;
    QString procName = ::ELGO_PROC::ELGOProc_enum2str[proc];

    if(socket)
    {
        socket->connectToServer(procName);
        if(socket->isOpen())
        {
            QByteArray bytes;
            QDataStream out(&bytes, QIODevice::WriteOnly);
            out.setVersion(QDataStream::Qt_5_12);
            out << event;
            out << src;

            const qint64 result = socket->write(bytes);
            socket->waitForBytesWritten(100);
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
