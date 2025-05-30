#ifndef SERVERINFO_H
#define SERVERINFO_H

// QT
#include <QString>

#define REMOTE_TCP_PORT 9412
#define JWT_URL "/client/jwt/socket"

/** @brief */
struct CONNECT_INFO
{
    CONNECT_INFO(const QString &wasHost = NULL,
                 const quint16 wasPort = 4500,
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
    DEFAULT_TIMEOUT = 5 * 1000,
    GET_JWT_TIMEOUT = 8 * 1000,
    PROGRESS_TIMEROUT = 10 * 1000,
    RECONNECT_TIMEOUT = 30 * 1000
};


#endif // SERVERINFO_H
