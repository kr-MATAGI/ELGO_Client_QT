#ifndef LOCALCLIENT_H
#define LOCALCLIENT_H

#include <QLocalSocket>

class LocalClient : public QObject
{
    Q_OBJECT

public:
    explicit LocalClient(QObject *parent = nullptr);
    virtual ~LocalClient();

private slots:
    /** @brief */
    void ReadToReady();
    /** @brief */
    void ReadData();

private:
    QLocalSocket *m_socket;
};

#endif // LOCALCLIENT_H
