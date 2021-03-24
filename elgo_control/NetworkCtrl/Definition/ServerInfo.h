#ifndef SERVERINFO_H
#define SERVERINFO_H

// QT
#include <QString>

#define REMOTE_TCP_PORT 9412
#define CONTENT_SERVER_URL  "wss://demo.elgo.co.kr:4500"
#define JWT_URL "/client/jwt/socket"


struct CONNECT_INFO
{
    CONNECT_INFO(const QString &wasHost = NULL, const quint16 wasPort = 4500,
                 const QString &remoteTCPHost = NULL)
    {
        WAS_HOST = wasHost;
        WAS_PORT = wasPort;
        REMOTE_HOST = remoteTCPHost;
    }

    QString WAS_HOST; // Web Backend Server
    quint16 WAS_PORT;

    QString REMOTE_HOST; // Remote Server

    QString remoteVersion;
    QString deviceVersion;
};

enum CONN_TIMEOUT
{
    GET_JWT_TIMEOUT = 3 * 1000,
};


#endif // SERVERINFO_H
