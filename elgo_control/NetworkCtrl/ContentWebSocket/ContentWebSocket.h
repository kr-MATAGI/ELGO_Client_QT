#ifndef CONTENTWEBSOCKET_H
#define CONTENTWEBSOCKET_H

// QT
#include <QWebSocket>
#include <QObject>

// Control
#include "NetworkCtrl/NetworkController.h"
#include "ContentWebSocketHandler.h"

class NetworkController;
class ContentWebSocketHandler;

class ContentWebSocket : public QObject
{
    Q_OBJECT
public:
    /** @brief */
    ContentWebSocket(QObject *parent = nullptr);
    /** @brief */
    ~ContentWebSocket();

    /** @brief */
    void SendTextMessageToServer(const QString& textMsg);

signals:
    /** @brief */
    void ConnectContentServerSignal();

    /** @brief */
    void StopProgressTimerSignal();

private slots:
    /** @brief */
    void ConnectContentSocketSlot();
    /** @brief */
    void ConnectedSocketSlot();
    /** @brief */
    void DisconnectedSocketSlot();
    /** @brief */
    void ErrorOccurredSocketSlot(QAbstractSocket::SocketError socketError);
    /** @brief */
    void SslErrorsOccurredSocketSlot(const QList<QSslError> sslErros);

    /** @brief */
    void TextMessageReceivedSlot(const QString &message);
    /** @brief */
    void BinaryMessageReceivedSlot(const QByteArray &message);

    /** @brief */
    void ErrorTimeout();

    /** @brief */
    void ProgressTimeout();
    /** @brief */
    void StopProgressTimerSlot();

private:
    QWebSocket *m_socket;
    ContentWebSocketHandler *m_handler;

    QTimer m_errorTimer;
    QTimer m_progressTimer;
    ContentSchema::Summary m_progressRes;
};

#endif // CONTENTWEBSOCKET_H
