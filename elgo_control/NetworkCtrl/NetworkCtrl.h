#ifndef WEBSOCKETCTRL_H
#define WEBSOCKETCTRL_H

// QT
#include <QObject>
#include <QtCore>
#include <QNetworkReply>

// control
#include "NetworkCtrl/Definition/ServerInfo.h"
#include "ContentWebSocket/ContentWebSocket.h"

class ContentWebSocket;

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
    void ConnectContentWebSocketToServer();

    /** @brief */
    bool GetAccessibleJwtFromServer(QString& dest);

    /** @brief */
    void SetConnectInfo(const CONNECT_INFO& newValue);

    /** @brief */
    void GetBeautifyUDID(const QString& src, QString& dest);

    /** @brief */
    CONNECT_INFO GetConnectInfo();

    /** @brief */
    QString& GetJWTString();

    /** @brief */
    ContentWebSocket& GetContentWebSocket();

private:
    /** @brief */
    size_t WriteCallBack(char *contents, size_t size, size_t nmemb, void *userData);

private:
    ContentWebSocket *m_socket;

    CONNECT_INFO m_connecInfo;
    QString m_jwt;
};

#endif // WEBSOCKETCTRL_H
