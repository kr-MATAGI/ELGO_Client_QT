
// Control
#include "ContentWebSocket.h"
#include "Logger/ControlLogger.h"

//========================================================
ContentWebSocket::ContentWebSocket(QObject *parent)
    : QObject(parent)
//========================================================
{
    m_socket = new QWebSocket;

    // connect
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
}

//========================================================
bool ContentWebSocket::ConnectContentWebSocket()
//========================================================
{
    bool retValue = false;

    QNetworkRequest netRequest;
    QUrl url(CONTENT_SERVER_URL);
    QByteArray jwtBytes;
    QString jwt = NetworkController::GetInstance()->GetNetworkCtrl().GetJWTString();
    jwtBytes.append(jwt.toUtf8());

    netRequest.setUrl(url);
    netRequest.setRawHeader("Authorization", jwtBytes);
    m_socket->open(netRequest);
    retValue = m_socket->isValid();

    return retValue;
}

//========================================================
void ContentWebSocket::ConnectedSocketSlot()
//========================================================
{

}

//========================================================
void ContentWebSocket::DisconnectedSocketSlot()
//========================================================
{

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
    ELGO_CONTROL_LOG("");
}

//========================================================
void ContentWebSocket::BinaryMessageReceivedSlot(const QByteArray &message)
//========================================================
{
    ELGO_CONTROL_LOG("");
}
