// control
#include "RemoteControlServer.h"
#include "Logger/ControlLogger.h"
#include "JSON/JsonParser.h"
#include "JSON/JsonWriter.h"

RemoteControlServer* RemoteControlServer::pInstance = nullptr;

//========================================================
RemoteControlServer::RemoteControlServer(QObject *parent)
    :QObject(parent)
    , m_bIsConnected(false)
//========================================================
{
    m_handler = new RemoteControlHandler;
    m_server = new QWebSocketServer("RemoteServer", QWebSocketServer::NonSecureMode, this);
    m_server->setMaxPendingConnections(1); // recv only one elgo_remote

    // connect
    connect(m_server, SIGNAL(newConnection()), this, SLOT(NewClientConnectedSlot()));
    connect(m_server, SIGNAL(acceptError(QAbstractSocket::SocketError)),
            this, SLOT(AcceptErrorSlot(QAbstractSocket::SocketError)));
    connect(m_server, SIGNAL(peerVerifyError(const QSslError&)),
            this, SLOT(PeerVerifyErrorSlot(const QSslError&)));
    connect(m_server, SIGNAL(serverError(QWebSocketProtocol::CloseCode)),
            this, SLOT(RemoteServerErrorSlot(QWebSocketProtocol::CloseCode)));

    // custom signals
    connect(this, SIGNAL(RemoteControlServerStartSignal()), SLOT(TCPServerStartSlot()));

}

//========================================================
RemoteControlServer::~RemoteControlServer()
//========================================================
{
    delete m_server;
    m_server = NULL;

    delete  m_handler;
    m_handler = NULL;

    delete m_cliecnt;
    m_cliecnt = NULL;
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
void RemoteControlServer::NewClientConnectedSlot()
//========================================================
{
    if(false == m_bIsConnected)
    {
        ELGO_CONTROL_LOG("New Remote Client Access !");

        m_bIsConnected = true;
        m_cliecnt = m_server->nextPendingConnection();

        // connect
        connect(m_cliecnt, SIGNAL(connected()), this, SLOT(RemoteClientSocketConnectedSlot()));
        connect(m_cliecnt, SIGNAL(textMessageReceived(const QString&)),
                this, SLOT(TextMsgRecvSlot(const QString&)));
        connect(m_cliecnt, SIGNAL(binaryMessageReceived(const QByteArray&)),
                this, SLOT(BinaryMsgRecvSlot(const QByteArray&)));
        connect(m_cliecnt, SIGNAL(error(QAbstractSocket::SocketError)),
                this, SLOT(RemoteClientError(QAbstractSocket::SocketError)));
        connect(m_cliecnt, SIGNAL(disconnected()),
                this, SLOT(RemoteClientDisconnectedSlot()));

        // server mush connect only one client
        disconnect(m_server, SIGNAL(newConnection()), this, SLOT(NewClientConnectedSlot()));
    }
    else
    {
        ELGO_CONTROL_LOG("Error - Already Connected Rmote");
    }
}

//========================================================
void RemoteControlServer::AcceptErrorSlot(QAbstractSocket::SocketError socketError)
//========================================================
{
    ELGO_CONTROL_LOG("Error - AcceptError : %d", socketError);
}

//========================================================
void RemoteControlServer::PeerVerifyErrorSlot(const QSslError& error)
//========================================================
{
    ELGO_CONTROL_LOG("Error - PeerVerifyError : %s (%d)",
                     error.errorString().toUtf8().constData(), error.error());
}

//========================================================
void RemoteControlServer::RemoteServerErrorSlot(QWebSocketProtocol::CloseCode closeCode)
//========================================================
{
    ELGO_CONTROL_LOG("Error - RemoteServerError : %d", closeCode);
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
            connect(m_server, SIGNAL(newConnection()), this, SLOT(NewClientConnectedSlot()));
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

    QHostAddress host(connInfo.REMOTE_HOST);
    quint16 port = REMOTE_TCP_PORT;

    bool bIsListen = m_server->listen(host, port);
    ELGO_CONTROL_LOG("Remote TCP Server Listen : %d, addr : %s, port : %u", bIsListen,
                     m_server->serverAddress().toString().toUtf8().constData(), m_server->serverPort());

}

//========================================================
void RemoteControlServer::MakeResponseJsonString(const Remote::Action action,
                                                 const Remote::Result::Contents& contents,
                                                 QString& dest)
//========================================================
{
    if(Remote::Action::DEVICE_LOGIN == action)
    {
        JsonWriter::WriteDeviceLoginResponse(action, contents, dest);
    }
    else if(Remote::Action::UPDATE_WIFI_LIST == action)
    {
        // Not Here, SendTextMessage
    }
    else if(Remote::Action::MANAGE_DEVICE == action)
    {
        JsonWriter::WriteManageDeviceResponse(action, contents, dest);
    }
    else if(Remote::Action::ROTATE_DISPLAY == action)
    {
        JsonWriter::WriteRotateDisplayResponse(action, contents, dest);
    }
    else if(Remote::Action::DEVICE_OPTIONS == action)
    {
        JsonWriter::WriteDeviceOptionsResponse(action, contents, dest);
    }
    else
    {
        ELGO_CONTROL_LOG("Error - Unkwon Action : %d", action);
    }
}

//========================================================
void RemoteControlServer::TextMsgRecvSlot(const QString& msg)
//========================================================
{
    if(NULL != m_cliecnt)
    {
        const Remote::Action recvAction = JsonParser::PaseRemoteActionText(msg);
        ELGO_CONTROL_LOG("Recv From Client : %s, action : %d", msg.toUtf8().constData(), recvAction);

        // Run Action
        Remote::Result::Contents resultContents;
        m_handler->RunAction(recvAction, msg, resultContents);

        QString sendJson;
        MakeResponseJsonString(recvAction, resultContents, sendJson);
        if(0 < sendJson.length())
        {
            m_cliecnt->sendTextMessage(sendJson);
            ELGO_CONTROL_LOG("elgo_control -> elgo_remote : %s", sendJson.toUtf8().constData());
        }
    }
    else
    {
        ELGO_CONTROL_LOG("NULL == Remote Client");
    }
}

//========================================================
void RemoteControlServer::SendTextMessage(const Remote::Action action,
                                          const Remote::Result::Contents& contents)
//========================================================
{
    if(NULL != m_cliecnt)
    {
        QString responseJson;
        JsonWriter::WriteUpdateWifiListResponse(action, contents, responseJson);

        if(0 < responseJson.length())
        {
            m_cliecnt->sendTextMessage(responseJson);
            ELGO_CONTROL_LOG("elgo_control -> elgo_remote : %s",
                             responseJson.toStdString().c_str());
        }
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
void RemoteControlServer::RemoteClientSocketConnectedSlot()
//========================================================
{
    ELGO_CONTROL_LOG("Remote Client Connected - %s", m_cliecnt->peerAddress().toString().toUtf8().constData());
}

//========================================================
void RemoteControlServer::RemoteClientError(QAbstractSocket::SocketError error)
//========================================================
{
    ELGO_CONTROL_LOG("Error - RemoteClientError: %d", error);
}
