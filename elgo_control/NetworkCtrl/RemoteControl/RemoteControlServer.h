#ifndef REMOTECONTROLSERVER_H
#define REMOTECONTROLSERVER_H

// QT
#include <QWebSocketServer>
#include <QWebSocket>

// control
#include "NetworkCtrl/NetworkController.h"
#include "RemoteControlHandler.h"

class NetworkController;
class RemoteControlHandler;

class RemoteControlServer : public QObject
{
    Q_OBJECT
private:
    static RemoteControlServer *pInstance;

    RemoteControlServer(QObject *parent = nullptr);
    ~RemoteControlServer();

public:
    /** @brief */
    static RemoteControlServer* GetInstance();

    /** @brief */
    void DestoryInstance();

public:
    /** @brief */
    void MakeResponseJsonString(const Remote::Action action,
                                const Remote::Result::Contents& contents,
                                QString& dest);
    /** @brief */
    void SendRemoteResponse(const Remote::Action action,
                            const Remote::Result::Contents& contents);

signals:
    /** @brief */
    void RemoteControlServerStartSignal();

    /** @brief */
    void RemoteClientDisconnect();

private slots:
    // Server
    /** @brief */
    void NewClientConnectedSlot();
    /** @brief */
    void AcceptErrorSlot(QAbstractSocket::SocketError socketError);
    /** @brief */
    void PeerVerifyErrorSlot(const QSslError& error);
    /** @brief */
    void RemoteServerErrorSlot(QWebSocketProtocol::CloseCode closeCode);

    /** @brief  Custom Signal */
    void TCPServerStartSlot();

    // Client
    /** @brief */
    void RemoteClientSocketConnectedSlot();
    /** @brief */
    void TextMsgRecvSlot(const QString& msg);
    /** @brief */
    void BinaryMsgRecvSlot(const QByteArray& bytes);
    /** @brief */
    void RemoteClientError(QAbstractSocket::SocketError error);
    /** @brief */
    void RemoteClientDisconnectedSlot();

    // Retry timer
    /** @brief */
    void RetryTimeout();

private:
    QWebSocketServer *m_server;
    QWebSocket *m_client;
    RemoteControlHandler *m_handler;

    // for retry sendMessage
    QString m_bufferdStr;
    QTimer m_retryTimer;
    int m_retryCount;

    bool m_bIsConnected;
};

#endif // REMOTECONTROLSERVER_H
