#ifndef REMOTECONTROLSERVER_H
#define REMOTECONTROLSERVER_H

// QT
#include <QWebSocketServer>
#include <QWebSocket>

// control
#include "NetworkCtrl/NetworkController.h"
#include "RemoteControlActionHandler.h"

class NetworkController;
class RemoteControlActionHandler;

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

signals:
    /** @brief */
    void TCPServerStartSignal();

private:
    QWebSocketServer *m_server;
    QWebSocket *m_cliecnt;
    RemoteControlActionHandler *m_handler;

    bool m_bIsConnected;
};

#endif // REMOTECONTROLSERVER_H
