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

signals:
    void ConnectContentServerSignal();

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

private:
    QWebSocket *m_socket;
    ContentWebSocketHandler *m_handler;
};

#endif // CONTENTWEBSOCKET_H
