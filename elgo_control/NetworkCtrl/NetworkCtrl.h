#ifndef WEBSOCKETCTRL_H
#define WEBSOCKETCTRL_H

// QT
#include <QObject>
#include <QtCore>
#include <QNetworkReply>

// control
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

    /** @brief */
    void GetRemoteVersionFromWAS();

    /** @brief */
    void SetConnectInfo(const CONNECT_INFO& newValue);

    /** @brief */
    CONNECT_INFO GetConnectInfo();

private slots:
    void ReplyRemoteVersionFinishedSlot(QNetworkReply *reply);

private:
    CONNECT_INFO m_connecInfo;
};

#endif // WEBSOCKETCTRL_H
