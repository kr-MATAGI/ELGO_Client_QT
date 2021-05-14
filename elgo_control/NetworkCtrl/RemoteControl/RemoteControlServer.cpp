// control
#include "RemoteControlServer.h"
#include "Logger/ControlLogger.h"
#include "JSON/JsonParser.h"
#include "JSON/JsonWriter.h"

RemoteControlServer* RemoteControlServer::pInstance = nullptr;

#define RETRY_TIMEOUT   1000 // unit: msec

//========================================================
RemoteControlServer::RemoteControlServer(QObject *parent)
    :QObject(parent)
    , m_retryCount(0)
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
    connect(this, SIGNAL(RemoteControlServerStartSignal()),
            this, SLOT(TCPServerStartSlot()));

    connect(this, SIGNAL(RemoteClientDisconnect()),
            this, SLOT(RemoteClientDisconnectedSlot()));

    // retry timer
    connect(&m_retryTimer, SIGNAL(timeout()),
            this, SLOT(RetryTimeout()));
}

//========================================================
RemoteControlServer::~RemoteControlServer()
//========================================================
{
    delete m_server;
    m_server = NULL;

    delete  m_handler;
    m_handler = NULL;

    delete m_client;
    m_client = NULL;
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
        m_client = m_server->nextPendingConnection();

        // connect
        connect(m_client, SIGNAL(connected()), this, SLOT(RemoteClientSocketConnectedSlot()));
        connect(m_client, SIGNAL(textMessageReceived(const QString&)),
                this, SLOT(TextMsgRecvSlot(const QString&)));
        connect(m_client, SIGNAL(binaryMessageReceived(const QByteArray&)),
                this, SLOT(BinaryMsgRecvSlot(const QByteArray&)));
        connect(m_client, SIGNAL(error(QAbstractSocket::SocketError)),
                this, SLOT(RemoteClientError(QAbstractSocket::SocketError)));
        connect(m_client, SIGNAL(disconnected()),
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
    RemoteClientDisconnectedSlot();
}

//========================================================
void RemoteControlServer::RemoteClientDisconnectedSlot()
//========================================================
{
    ELGO_CONTROL_LOG("Remote Client Disconnted");

    if(NULL != m_client)
    {
        m_client->close();
        m_client->deleteLater();
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
    QHostAddress host("elgo-remote.com");
    quint16 port = REMOTE_TCP_PORT;

    bool bIsListen = m_server->listen(host, port);
    ELGO_CONTROL_LOG("Remote TCP Server Listen : %d, addr : elgo-remote.com, port : %u",
                     bIsListen, m_server->serverPort());
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
    else if(Remote::Action::CONNECT_WIFI == action)
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
    else if(Remote::Action::USER_LOGIN == action)
    {
        JsonWriter::WriteRemoteUserLoginResponse(action, contents, dest);
    }
    else
    {
        ELGO_CONTROL_LOG("Error - Unknwon Action : %d", action);
    }
}

//========================================================
void RemoteControlServer::TextMsgRecvSlot(const QString& msg)
//========================================================
{
    if(NULL != m_client)
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
            m_client->sendTextMessage(sendJson);
            ELGO_CONTROL_LOG("elgo_control -> elgo_remote : %s", sendJson.toUtf8().constData());
        }
    }
    else
    {
        ELGO_CONTROL_LOG("NULL == Remote Client");
    }
}

//========================================================
void RemoteControlServer::SendRemoteResponse(const Remote::Action action,
                                             const Remote::Result::Contents& contents)
//========================================================
{
    if(NULL != m_client)
    {
        QString responseJson;

        if(Remote::Action::UPDATE_WIFI_LIST == action)
        {
            JsonWriter::WriteUpdateWifiListResponse(action, contents, responseJson);
        }
        else if(Remote::Action::CONNECT_WIFI == action)
        {
            JsonWriter::WriteConnectWifiResultResponse(action ,contents, responseJson);
        }
        else
        {
            ELGO_CONTROL_LOG("Error - Not Enrolled Action : %d", action);
        }

        if(0 < responseJson.length())
        {
            const qint64 sendBytsSize = m_client->sendTextMessage(responseJson);
            ELGO_CONTROL_LOG("elgo_control -> elgo_remote : %lld, { %s }",
                             sendBytsSize, responseJson.toStdString().c_str());
            if(0 == sendBytsSize)
            {
                // Retry
                m_bufferdStr = responseJson;
                ELGO_CONTROL_LOG("RetryTimer Start And BUFFERD - %s", m_bufferdStr.toStdString().c_str());

                m_retryTimer.start(RETRY_TIMEOUT);
            }
        }
    }
    else
    {
        ELGO_CONTROL_LOG("NULL == Remote Client");
    }
}

//========================================================
void RemoteControlServer::RetryTimeout()
//========================================================
{
    m_retryCount++;
    ELGO_CONTROL_LOG("Retry Count: %d { %s }", m_retryCount, m_bufferdStr.toStdString().c_str());
    if(NULL != m_client)
    {
        if(0 < m_bufferdStr.length())
        {
            const quint64 sendByteSize = m_client->sendTextMessage(m_bufferdStr);
            if(0 < sendByteSize)
            {
                ELGO_CONTROL_LOG("Retry Send Success - %s", m_bufferdStr.toStdString().c_str());
                m_bufferdStr.clear();
                m_retryTimer.stop();
                m_retryCount = 0;
            }
        }
        else
        {
            if(true == m_retryTimer.isActive())
            {
                m_retryTimer.stop();
                m_retryCount = 0;
            }
        }
    }
    else
    {
        ELGO_CONTROL_LOG("NULL == Remote Client");
        if(true == m_retryTimer.isActive())
        {
            m_retryTimer.stop();
            m_retryCount = 0;
        }
    }
}

//========================================================
void RemoteControlServer::BinaryMsgRecvSlot(const QByteArray& bytes)
//========================================================
{
    if(NULL != m_client)
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
    ELGO_CONTROL_LOG("Remote Client Connected - %s", m_client->peerAddress().toString().toUtf8().constData());
}

//========================================================
void RemoteControlServer::RemoteClientError(QAbstractSocket::SocketError error)
//========================================================
{
    ELGO_CONTROL_LOG("Error - RemoteClientError: %d", error);
}
