#ifndef SERVERINFO_H
#define SERVERINFO_H

// QT
#include <QString>

#define REMOTE_TCP_PORT 9412

struct CONNECT_INFO
{
    CONNECT_INFO(const QString &wasHost = NULL, const quint16 wasPort = 4500,
                 const QString &remoteTCPHost = NULL)
    {
        WAS_HOST = wasHost;
        WAS_PORT = wasPort;
        REMOTE_TCP_HOST = remoteTCPHost;
    }

    QString WAS_HOST; // Web Backend Server
    quint16 WAS_PORT;

    QString REMOTE_TCP_HOST; // TCP Server

    QString remoteVersion;
    QString deviceVersion;
};

enum CONN_TIMEOUT
{
    REMOTE_VERSION_CHECK = 3 * 1000,
};


#endif // SERVERINFO_H
