#ifndef TCPHANDLER_H
#define TCPHANDLER_H

// QT
#include <QNetworkAccessManager>

class TCPHandler : public QObject
{
    Q_OBJECT
public:
    TCPHandler(QObject *parent = nullptr);
    ~TCPHandler();

public:
    /** @brief */

private:
    QNetworkAccessManager *m_manager;
};

#endif // TCPHANDLER_H
