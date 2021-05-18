
// Control
#include "JSON/JsonParser.h"
#include "JSON/JsonWriter.h"
#include "ContentWebSocket.h"
#include "Logger/ControlLogger.h"
#include "LocalSocketEvent/EFCEvent.h"

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

    connect(&m_errorTimer, &QTimer::timeout,
            this, &ContentWebSocket::ErrorTimeout);

    connect(&m_progressTimer, &QTimer::timeout,
            this, &ContentWebSocket::ProgressTimeout);
    connect(this, &ContentWebSocket::StopProgressTimerSignal,
            this, &ContentWebSocket::StopProgressTimerSlot);
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
void ContentWebSocket::SendTextMessageToServer(const QString& textMsg)
//========================================================
{
    if(NULL != m_socket)
    {
        if(0 < textMsg.length())
        {
            const qint64 sendSize = m_socket->sendTextMessage(textMsg);
            ELGO_CONTROL_LOG("Send (%lld): %s", sendSize,
                             textMsg.toUtf8().constData());
        }
        else
        {
            ELGO_CONTROL_LOG("Error - 0 == text.length()");
        }
    }
    else
    {
        ELGO_CONTROL_LOG("Error - NULL == socket");
    }
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
    ELGO_CONTROL_LOG("Content Server WebSocket - Disconnected");
}

//========================================================
void ContentWebSocket::ErrorOccurredSocketSlot(QAbstractSocket::SocketError socketError)
//========================================================
{
    ELGO_CONTROL_LOG("Error - socketError : %d", socketError);

    m_errorTimer.start(100);
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
            if( (ContentSchema::Event::SINGLE_PLAY == response.event) ||
                (ContentSchema::Event::PLAY_SCHEDULE == response.event) )
            {
                m_progressRes = response;
                m_progressTimer.setSingleShot(true);
                m_progressTimer.start(CONN_TIMEOUT::PROGRESS_TIMEROUT);
                ELGO_CONTROL_LOG("Progress Timer Start");
            }

            QString request;
            m_handler->RunEvent(response, request);
            if(0 < request.length())
            {
                const qint64 sendSize = m_socket->sendTextMessage(request);
                ELGO_CONTROL_LOG("Send Request (%d) : %s", sendSize,
                                 request.toUtf8().constData());

                // Reboot Event
                if(ContentSchema::Event::SYSTEM_REBOOT == response.event)
                {
                    // is need safe disconnect? (elgo_control, viewer)
                    QByteArray reBootBytes;
                    const int bSendEvent = EFCEvent::SendEvent(ELGO_SYS::Proc::ELGO_MAIN,
                                                               MAIN_EVENT::Event::SYSTEM_REBOOT_MAIN,
                                                               reBootBytes);
                    if(false == bSendEvent)
                    {
                        ELGO_CONTROL_LOG("Error - SendEvent: %d", bSendEvent);
                    }
                }
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
    ELGO_CONTROL_LOG("");
}

//========================================================
void ContentWebSocket::ErrorTimeout()
//========================================================
{
    /**
     * @note
     *       ELGO_CONTROL -> ELGO_VIEWER
     *       Viewer will make qr code image and display.
     * @param
     *       QString    ip
     *       QString    deviceName
     */
    QByteArray bytes;
    QDataStream dataStream(&bytes, QIODevice::WriteOnly);
    const QString& ip = NetworkController::GetInstance()->GetNetworkCtrl().GetConnectInfo().REMOTE_HOST;
    dataStream << ip;
    dataStream << QString("서버 연결 실패");

    const bool bSendEvent = EFCEvent::SendEvent(ELGO_SYS::Proc::ELGO_VIEWER,
                                                VIEWER_EVENT::Event::MAKE_QRCODE,
                                                bytes);
    if(false == bSendEvent)
    {
        ELGO_CONTROL_LOG("ERROR - SendEvent: %d", VIEWER_EVENT::Event::MAKE_QRCODE);
    }

    m_errorTimer.stop();
}

//========================================================
void ContentWebSocket::ProgressTimeout()
//========================================================
{
    QString progressJson;
    ContentSchema::Summary modifiedJson = m_progressRes;
    modifiedJson.payload.src = m_progressRes.payload.dest;
    modifiedJson.payload.dest = m_progressRes.payload.src;
    modifiedJson.payload.type = ContentSchema::PayloadType::PROGRESS;

    JsonWriter::WriteContentServerProgressResponse(modifiedJson, progressJson);
    SendTextMessageToServer(progressJson);
}

//========================================================
void ContentWebSocket::StopProgressTimerSlot()
//========================================================
{
    if(true == m_progressTimer.isActive())
    {
        m_progressTimer.stop();
        m_progressRes = ContentSchema::Summary();
        ELGO_CONTROL_LOG("Progress Timer Stop");
    }
    else
    {
        ELGO_CONTROL_LOG("Progress Timer is NOT Active");
    }
}
