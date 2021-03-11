#ifndef CONTENTWEBSOCKET_H
#define CONTENTWEBSOCKET_H

// QT
#include <QWebSocket>
#include <QObject>

// Control
#include "NetworkCtrl/NetworkController.h";

class NetworkController;

class ContentWebSocket : public QObject
{
    Q_OBJECT
public:
    ContentWebSocket(QObject *parent = nullptr);
    ~ContentWebSocket();

    /** @brief */
    bool ConnectContentWebSocket();

private slots:
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
    QString token;
};

#endif // CONTENTWEBSOCKET_H
