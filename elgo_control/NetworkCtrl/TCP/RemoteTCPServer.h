#ifndef TCPSERVER_H
#define TCPSERVER_H

// QT
#include <QTcpServer>

// control
#include "../NetworkController.h"

class NetworkController;

class RemoteTCPServer : public QObject
{
    Q_OBJECT
private:
    static RemoteTCPServer *pInstance;

    RemoteTCPServer(QObject *parent = nullptr);
    ~RemoteTCPServer();

public:
    /** @brief */
    static RemoteTCPServer* GetInstance();

    /** @brief */
    void DestoryInstance();


private slots:
    // Server
    /** @brief */
    void newConnectionSlot();
    /** @brief */
    void AcceptErrorSlot(QAbstractSocket::SocketError socketError);
    /** @brief */
    void DisconnectedSlot();

    // Socket
    void ReadEventSlot();

signals:
    /** @brief */
    void TCPServerStartSignal();

private slots:
    /** @brief */
    void TCPServerStartSlot();

private:
    QTcpServer *m_server;
    QTcpSocket *m_cliecnt;

    int m_connectedCount;
};

#endif // TCPSERVER_H
