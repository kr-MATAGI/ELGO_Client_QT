
// Control
#include "JSON/JsonParser.h"
#include "ContentWebSocket.h"
#include "Logger/ControlLogger.h"

//========================================================
ContentWebSocket::ContentWebSocket(QObject *parent)
    : QObject(parent)
//========================================================
{
    m_socket = new QWebSocket;
    m_handler = new ContentWebSocketHandler;

    // connect
    // custion singal
    connect(this, SIGNAL(ConnectContentServerSignal()),
            this, SLOT(ConnectContentSocketSlot()));

    connect(m_socket, SIGNAL(connected()), this, SLOT(ConnectedSocketSlot()));
    connect(m_socket, SIGNAL(disconnected()), this, SLOT(DisconnectedSocketSlot()));
    connect(m_socket, SIGNAL(error(QAbstractSocket::SocketError)),
            this, SLOT(ErrorOccurredSocketSlot(QAbstractSocket::SocketError)));
    connect(m_socket, SIGNAL(sslErrors(const QList<QSslError>)),
            this, SLOT(SslErrorsOccurredSocketSlot(const QList<QSslError>)));
    connect(m_socket, SIGNAL(textMessageReceived(const QString&)),
            this, SLOT(TextMessageReceivedSlot(const QString&)));
    connect(m_socket, SIGNAL(binaryMessageReceived(const QByteArray&)),
            this, SLOT(BinaryMessageReceivedSlot(const QByteArray&)));
}

//========================================================
ContentWebSocket::~ContentWebSocket()
//========================================================
{
    delete m_socket;
    m_socket = NULL;

    delete m_handler;
    m_handler = NULL;
}

//========================================================
void ContentWebSocket::ConnectContentSocketSlot()
//========================================================
{
    QNetworkRequest netRequest;
    QUrl url(CONTENT_SERVER_URL);
    QByteArray jwtBytes;
    QString jwt = NetworkController::GetInstance()->GetNetworkCtrl().GetJWTString();
    jwtBytes.append(jwt.toUtf8());

    netRequest.setUrl(url);
    netRequest.setRawHeader("Authorization", jwtBytes);
    m_socket->open(netRequest);
}

//========================================================
void ContentWebSocket::ConnectedSocketSlot()
//========================================================
{
    if(NULL != m_socket)
    {
        const bool bIsValid = m_socket->isValid();
        ELGO_CONTROL_LOG("Content WebSocket Valid : %d, host : %s",
                         bIsValid, m_socket->peerAddress().toString().toUtf8().constData());
    }
    else
    {
        ELGO_CONTROL_LOG("NULL == m_socket");
    }
}

//========================================================
void ContentWebSocket::DisconnectedSocketSlot()
//========================================================
{
    // TODO
    ELGO_CONTROL_LOG("");
}

//========================================================
void ContentWebSocket::ErrorOccurredSocketSlot(QAbstractSocket::SocketError socketError)
//========================================================
{
    ELGO_CONTROL_LOG("Error - socketError : %d", socketError);
}

//========================================================
void ContentWebSocket::SslErrorsOccurredSocketSlot(const QList<QSslError> sslErros)
//========================================================
{
    foreach(const QSslError& error, sslErros)
    {
        ELGO_CONTROL_LOG("Error - SslError :turn inner method result */ %s", error.errorString().toStdString().c_str());
    }
}

//========================================================
void ContentWebSocket::TextMessageReceivedSlot(const QString &message)
//========================================================
{
    ELGO_CONTROL_LOG("%s", message.toUtf8().constData());
    if(NULL != m_socket)
    {
        ContentSchema::Summary response;
        const bool bIsParsing = JsonParser::ParseContentServerJsonResponse(message, response);
        if(true == bIsParsing)
        {
            QString request;
            m_handler->RunEvent(response, request);
            if(0 < request.length())
            {
                m_socket->sendTextMessage(request);
                ELGO_CONTROL_LOG("Send Request : %s", request.toUtf8().constData());
            }
        }
        else
        {
            ELGO_CONTROL_LOG("Error - Content Server Response parsing failed");
        }
    }
    else
    {
        ELGO_CONTROL_LOG("NULL == m_socket");
    }
}

//========================================================
void ContentWebSocket::BinaryMessageReceivedSlot(const QByteArray &message)
//========================================================
{
    // TODO
    ELGO_CONTROL_LOG("");
}