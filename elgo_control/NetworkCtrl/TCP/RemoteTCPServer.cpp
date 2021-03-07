// control
#include "RemoteTCPServer.h"
#include "Logger/ControlLogger.h"

RemoteTCPServer* RemoteTCPServer::pInstance = nullptr;

//========================================================
RemoteTCPServer::RemoteTCPServer(QObject *parent)
    :QObject(parent)
    , m_connectedCount(0)
//========================================================
{
    m_server = new QTcpServer(this);
    m_server->setMaxPendingConnections(1); // recv only one elgo_remote

    // connect
    connect(this, SIGNAL(TCPServerStartSignal()), SLOT(TCPServerStartSlot()));
    connect(m_server, SIGNAL(newConnection()), this, SLOT(newConnectionSlot()));
    connect(m_server, SIGNAL(acceptError(QAbstractSocket::SocketError)),
            this, SLOT(AcceptErrorSlot(QAbstractSocket::SocketError)));

}

//========================================================
RemoteTCPServer::~RemoteTCPServer()
//========================================================
{
    delete m_server;
    m_server = NULL;
}

//========================================================
RemoteTCPServer* RemoteTCPServer::GetInstance()
//========================================================
{
    if(nullptr == pInstance)
    {
        pInstance = new RemoteTCPServer();
    }
    return pInstance;
}

//========================================================
void RemoteTCPServer::DestoryInstance()
//========================================================
{
    if(nullptr != pInstance)
    {
        return;
    }
    delete pInstance;
    pInstance = NULL;
}

//========================================================
void RemoteTCPServer::newConnectionSlot()
//========================================================
{
    if(0 == m_connectedCount)
    {
        m_connectedCount++;
        m_cliecnt = m_server->nextPendingConnection();

        connect(m_cliecnt, SIGNAL(readyRead()), this, SLOT(ReadEventSlot()));
        connect(m_cliecnt, SIGNAL(disconnected()), this, SLOT(DisconnectedSlot()));
        disconnect(m_server, SIGNAL(newConnection()), this, SLOT(newConnectionSlot()));
    }
    else
    {
        ELGO_CONTROL_LOG("Error - Already Connected Rmote");
    }
}

//========================================================
void RemoteTCPServer::ReadEventSlot()
//========================================================
{
    if(0 <= m_cliecnt->bytesAvailable())
    {
        QByteArray recvBytes = m_cliecnt->readAll();
        QDataStream recvStream(&recvBytes, QIODevice::ReadOnly);

        ELGO_CONTROL_LOG("Read Event Slot TEST : %s", recvBytes.toStdString().c_str());
    }
}

//========================================================
void RemoteTCPServer::AcceptErrorSlot(QAbstractSocket::SocketError socketError)
//========================================================
{
    ELGO_CONTROL_LOG("Emit SIGAL - acceptError : %d", socketError);
}

//========================================================
void RemoteTCPServer::DisconnectedSlot()
//========================================================
{
    ELGO_CONTROL_LOG("Remote Client Disconnted");
    m_cliecnt->close();
    m_connectedCount--;

    if(0 == m_connectedCount)
    {
        connect(m_server, SIGNAL(newConnection()), this, SLOT(newConnectionSlot()));
    }
}

//========================================================
void RemoteTCPServer::TCPServerStartSlot()
//========================================================
{
    CONNECT_INFO connInfo = NetworkController::GetInstance()->GetNetworkCtrl().GetConnectInfo();

    QHostAddress host(connInfo.REMOTE_TCP_HOST);
    quint16 port = REMOTE_TCP_PORT;

    bool bIsListen = m_server->listen(host, port);
    ELGO_CONTROL_LOG("Remote TCP Server Listen : %d, addr : %s, port : %u", bIsListen,
                     m_server->serverAddress().toString().toUtf8().constData(), m_server->serverPort());

}
