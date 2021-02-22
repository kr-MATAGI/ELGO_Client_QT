#ifndef WEBSOCKETCTRL_H
#define WEBSOCKETCTRL_H

#include <QObject>
#include <QtCore>
#include <QNetworkReply>
#include "TCPConnect/Definition/serverInfo.h"

class NetworkCtrl : public QObject
{
    Q_OBJECT

public:
    explicit NetworkCtrl(QObject *parent = nullptr);
    virtual ~NetworkCtrl();

public:
    /** brief */
    void LoadServerInfoFromXML(QString &path);
    /** brief */
    void GetRemoteVersionFromWAS();
    /** brief */
    CONNECT_INFO GetConnectInfo();
    /** brief */
    void GetActiveWirelessInternetList();

private slots:
    void replyRemoteVersionFinished(QNetworkReply *reply);

private:
    CONNECT_INFO m_connecInfo;
    QStringList m_wifiList;

    QNetworkAccessManager *m_netManager;
};

#endif // WEBSOCKETCTRL_H
