#ifndef REMOTECONTROLSERVER_H
#define REMOTECONTROLSERVER_H

// QT
#include <QWebSocketServer>
#include <QWebSocket>

// control
#include "../NetworkController.h"

class NetworkController;

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
    void newConnectionSlot();
    /** @brief */
    void AcceptErrorSlot(QAbstractSocket::SocketError socketError);
    /** @brief */
    void TCPServerStartSlot();

    // Client
    /** @brief */
    void TextMsgRecvSlot(const QString& msg);
    /** @brief */
    void BinaryMsgRecvSlot(const QByteArray& bytes);
    /** @brief */
    void RemoteClientDisconnectedSlot();

signals:
    /** @brief */
    void TCPServerStartSignal();

private:
    QWebSocketServer *m_server;
    QWebSocket *m_cliecnt;

    bool m_bIsConnected;
};

#endif // REMOTECONTROLSERVER_H
