#ifndef SERVERINFO_H
#define SERVERINFO_H

#include <QString>

struct CONNECT_INFO
{
    QString WAS_HOST; // Web Backend Server
    int WAS_PORT;

    QString MAIN_SOCKET_HOST; // Main Socket Server
    int MAIN_SOCKET_PORT;
    int SUB_SOCKET_PROT;

    QString TUNNELING_HOST; // Remote Control Server
    int TUNNELING_PORT;

    QString version;
};

enum CONN_TIMEOUT
{
    REMOTE_VERSION_CHECK = 3 * 1000,
};


#endif // SERVERINFO_H
