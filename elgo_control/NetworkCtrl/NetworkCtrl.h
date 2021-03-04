#ifndef WEBSOCKETCTRL_H
#define WEBSOCKETCTRL_H

#include <QObject>
#include <QtCore>
#include <QNetworkReply>
#include "NetworkCtrl/Definition/ServerInfo.h"

class NetworkCtrl : public QObject
{
    Q_OBJECT

public:
    explicit NetworkCtrl(QObject *parent = nullptr);
    virtual ~NetworkCtrl();

public:
    /** @brief */
    void SendControlIsReady();

    /** brief */
    void LoadServerInfoFromXML(QString &path);

    /** brief */
    void GetRemoteVersionFromWAS();

    /** brief */
    CONNECT_INFO GetConnectInfo();

    // For TCP Socket Test
    void TCPSenderTest();

private slots:
    void replyRemoteVersionFinished(QNetworkReply *reply);

private:
    CONNECT_INFO m_connecInfo;

    QNetworkAccessManager *m_netManager;


    QTcpSocket *tcpSocket;
};

#endif // WEBSOCKETCTRL_H
