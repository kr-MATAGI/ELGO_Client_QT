#ifndef SHARDMEMORY_H
#define SHARDMEMORY_H

#include <QSharedMemory>
#include <QBuffer>

class ShmCtrl
{
public:
    explicit ShmCtrl();

public:
    /** @brief */
    void ShmWrite(const QString &key, QBuffer &buffer);

    /** @brief */
    void ShmRead(const QString &key, QBuffer &buffer);

};

#endif // SHARDMEMORY_H
