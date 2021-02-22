#ifndef LOCALSERVER_H
#define LOCALSERVER_H

#include <QLocalServer>

class LocalServer : public QLocalServer
{
    Q_OBJECT

public:
    explicit LocalServer(QObject *parent = nullptr);
    virtual ~LocalServer();

public:
    /** brief */
    void LocalServerStart(QString &name);
    /** brief */
    void SetServerName(const QString &name);

private slots:
    /** brief */
    void ClientConnection();

private:
    QLocalServer *m_server;
    QString m_name;

};

#endif // LOCALSERVER_H
