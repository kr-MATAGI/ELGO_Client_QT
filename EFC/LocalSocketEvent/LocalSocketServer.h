#ifndef LOCALSOCKETSERVER_H
#define LOCALSOCKETSERVER_H

#include <QObject>
#include <QLocalServer>
#include <QMap>
#include "Common/Deifinition.h"
#include "Common/EventList.h"

typedef void (*func)(QByteArray&);

class LocalSocketServer : public QObject
{
    Q_OBJECT
public:
    LocalSocketServer(::ELGO_PROC::Proc name, QObject *parent = nullptr);
    ~LocalSocketServer();

    /** @brief */
    bool SendEvent(ELGO_PROC::Proc proc, quint16 event, QByteArray &src);

    /** @brief */
    void RegisterEvent(quint16 event, func memFunc);

private:
    /** @brief */
    void AddToList(QLocalSocket *socket);

private slots:
    void clientConnection();
    void readEvent();
    void discardSocket();

protected:
    QString m_procName;
    QLocalServer m_server;
    QList<QLocalSocket *> m_connectedList;
    QLocalSocket *m_socket;

    QMap<quint16, func> m_funcMap;
};

#endif // LOCALSOCKETSERVER_H
