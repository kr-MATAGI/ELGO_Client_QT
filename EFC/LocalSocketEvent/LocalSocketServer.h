#ifndef LOCALSOCKETSERVER_H
#define LOCALSOCKETSERVER_H

#include <QObject>
#include <QLocalServer>
#include "Common/Deifinition.h"

class LocalSocketServer : public QObject
{
    Q_OBJECT
public:
    LocalSocketServer(::ELGO_PROC::Proc name, QObject *parent = nullptr);
    ~LocalSocketServer();

private:
    void AddToList(QLocalSocket *socket);

private slots:
    void clientConnection();
    void readSocket();
    void discardSocket();

private:
    QString m_procName;
    QLocalServer m_server;
    QList<QLocalSocket *> m_connectedList;
    QLocalSocket *m_socket;

};

#endif // LOCALSOCKETSERVER_H
