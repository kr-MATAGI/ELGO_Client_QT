#ifndef SHARDMEMORY_H
#define SHARDMEMORY_H

// QT
#include <QSharedMemory>
#include <QBuffer>

// ShardMem
#include "ShmDefinition.h"

class ShmCtrl
{
public:
    explicit ShmCtrl();
    ~ShmCtrl();

public:
    /** @brief */
    bool ShmWrite(const QString &key, QBuffer &buffer);

    /** @brief */
    bool ShmWrite(const QString &key, QByteArray &bytes);

    /** @brief */
    bool ShmRead(const QString &key, QBuffer &buffer);

    /** @brief */
    bool ShmRead(const QString &key, QByteArray &bytes);

private:
    QVector<QSharedMemory *> m_shmList;
};

#endif // SHARDMEMORY_H
