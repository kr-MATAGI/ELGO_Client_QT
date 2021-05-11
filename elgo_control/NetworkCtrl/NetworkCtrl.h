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
    CONNECT_INFO GetConnectInfo();

    /** @brief */
    void SetRemoteHostIP(const QString& newValue);
    /** @brief */
    QString& GetRemoteHostIP();

    /** @brief */
    QString& GetJWTString();
    /** @brief */
    ContentWebSocket& GetContentWebSocket();

    /** @brief */
    bool UploadScreenCaptureImage(const QString& savedPath, QString& uploadedPath);

    /** @brief  Set/Get DPMS */
    void SetDisplaySleepStatus(const bool newValue);
    /** @brief */
    bool GetDisplaySleepStatus();

    /** @brief */
    void SetDeviceName(const QString& value);
    /** @brief */
    QString& GetDeviceName();

private:
    /** @brief */
    static size_t WriteFunction(void *ptr, size_t size, size_t nmemb, std::string* data);

private:
    ContentWebSocket *m_socket;
    CONNECT_INFO m_connecInfo;

    QString m_jwt;
    QString m_deviceName;

    bool m_bDisplaySleep;
};

#endif // WEBSOCKETCTRL_H
