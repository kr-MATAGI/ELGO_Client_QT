#ifndef LOCALSOCKETCLIENT_H
#define LOCALSOCKETCLIENT_H

#include <QObject>
#include <QLocalSocket>
#include "Common/Deifinition.h"

class LocalSocketClient : public QObject
{
    Q_OBJECT

public:
    LocalSocketClient(QObject *parent = nullptr);
    ~LocalSocketClient();

    /** brief */
    void SendEvent(::ELGO_PROC::Proc proc, QByteArray &src);

private slots:
    void discardSocket();

public:
    QLocalSocket * m_socket;
};

#endif // LOCALSOCKETCLIENT_H
