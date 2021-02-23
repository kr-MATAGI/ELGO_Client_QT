#ifndef SHARDMEMORY_H
#define SHARDMEMORY_H

#include <QSharedMemory>
#include <QBuffer>

class ShmCtrl
{
public:
    explicit ShmCtrl(const QString &key);

public:
    /** brief */
    void ShmWrite(QBuffer &buffer);

    /** brief */
    void ShmRead(QBuffer &buffer);

private :
    QString m_key;
    QSharedMemory *m_sharedMem;

};

#endif // SHARDMEMORY_H
