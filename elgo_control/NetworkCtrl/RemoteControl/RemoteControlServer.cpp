// control
#include "RemoteControlServer.h"
#include "Logger/ControlLogger.h"

RemoteControlServer* RemoteControlServer::pInstance = nullptr;

//========================================================
RemoteControlServer::RemoteControlServer(QObject *parent)
    :QObject(parent)
    , m_bIsConnected(false)
//========================================================
{
    m_server = new QWebSocketServer("RemoteServer", QWebSocketServer::NonSecureMode, this);
    m_server->setMaxPendingConnections(1); // recv only one elgo_remote

    // connect
    connect(m_server, SIGNAL(newConnection()), this, SLOT(newConnectionSlot()));
    connect(m_server, SIGNAL(acceptError(QAbstractSocket::SocketError)),
            this, SLOT(AcceptErrorSlot(QAbstractSocket::SocketError)));

    // custom signals
    connect(this, SIGNAL(TCPServerStartSignal()), SLOT(TCPServerStartSlot()));

}

//========================================================
RemoteControlServer::~RemoteControlServer()
//========================================================
{
    delete m_server;
    m_server = NULL;
}

//========================================================
RemoteControlServer* RemoteControlServer::GetInstance()
//========================================================
{
    if(nullptr == pInstance)
    {
        pInstance = new RemoteControlServer();
    }
    return pInstance;
}

//========================================================
void RemoteControlServer::DestoryInstance()
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
void RemoteControlServer::newConnectionSlot()
//========================================================
{
    if(false == m_bIsConnected)
    {
        ELGO_CONTROL_LOG("New Remote Client Access !");

        m_bIsConnected = true;
        m_cliecnt = m_server->nextPendingConnection();

        // connect
        connect(m_cliecnt, SIGNAL(textMessageReceived(const QString&)),
                this, SLOT(TextMsgRecvSlot(const QString&)));
        connect(m_cliecnt, SIGNAL(binaryMessageReceived(const QByteArray&)),
                this, SLOT(BinaryMsgRecvSlot(const QByteArray&)));
        connect(m_cliecnt, SIGNAL(disconnected()),
                this, SLOT(RemoteClientDisconnectedSlot()));

        // server mush connect only one client
        disconnect(m_server, SIGNAL(newConnection()), this, SLOT(newConnectionSlot()));
    }
    else
    {
        ELGO_CONTROL_LOG("Error - Already Connected Rmote");
    }
}

//========================================================
void RemoteControlServer::TextMsgRecvSlot(const QString& msg)
//========================================================
{
    if(NULL != m_cliecnt)
    {
        ELGO_CONTROL_LOG("%s", msg.toUtf8().constData());
    }
    else
    {
        ELGO_CONTROL_LOG("NULL == Remote Client");
    }
}

//========================================================
void RemoteControlServer::BinaryMsgRecvSlot(const QByteArray& bytes)
//========================================================
{
    if(NULL != m_cliecnt)
    {
        ELGO_CONTROL_LOG("%s", bytes.toStdString().c_str());
    }
    else
    {
        ELGO_CONTROL_LOG("NULL == Remote Client");
    }
}

//========================================================
void RemoteControlServer::AcceptErrorSlot(QAbstractSocket::SocketError socketError)
//========================================================
{
    ELGO_CONTROL_LOG("Emit SIGAL - acceptError : %d", socketError);
}

//========================================================
void RemoteControlServer::RemoteClientDisconnectedSlot()
//========================================================
{
    ELGO_CONTROL_LOG("Remote Client Disconnted");

    if(NULL != m_cliecnt)
    {
        m_cliecnt->close();
        m_cliecnt->deleteLater();
        m_bIsConnected = false;

        if(false == m_bIsConnected)
        {
            connect(m_server, SIGNAL(newConnection()), this, SLOT(newConnectionSlot()));
        }
    }
    else
    {
        ELGO_CONTROL_LOG("NULL == Remote Client");
    }
}

//========================================================
void RemoteControlServer::TCPServerStartSlot()
//========================================================
{
    CONNECT_INFO connInfo = NetworkController::GetInstance()->GetNetworkCtrl().GetConnectInfo();

    QHostAddress host(connInfo.REMOTE_TCP_HOST);
    quint16 port = REMOTE_TCP_PORT;

    bool bIsListen = m_server->listen(host, port);
    ELGO_CONTROL_LOG("Remote TCP Server Listen : %d, addr : %s, port : %u", bIsListen,
                     m_server->serverAddress().toString().toUtf8().constData(), m_server->serverPort());

}
