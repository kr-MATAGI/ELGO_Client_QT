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
    LocalSocketServer(::ELGO_SYS::Proc name, QObject *parent = nullptr);
    ~LocalSocketServer();

private:
    /** @brief */
    void AddToList(QLocalSocket *socket);

protected slots:
    virtual void ReadEventSlot() = 0;
    void ClientConnectionSlot();
    void DiscardSocketSlot();

protected:
    QString m_procName;
    QLocalServer m_server;
    QList<QLocalSocket *> m_connectedList;
};

#endif // LOCALSOCKETSERVER_H
