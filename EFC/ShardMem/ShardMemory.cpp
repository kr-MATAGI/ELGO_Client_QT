#include "ShardMemory.h"

#include <QDebug>
#include <QDataStream>

//========================================================
ShardMemory::ShardMemory(const QString &key)
//========================================================
{
    m_key = key;
    m_sharedMem = new QSharedMemory(m_key);
}

//========================================================
void ShardMemory::ShmWrite(QBuffer &buffer)
//========================================================
{
    if(m_sharedMem->isAttached())
    {
        if(false == m_sharedMem->detach())
        {
            qDebug() << __FUNCTION__ <<" Not Connected SHM";
            return;
        }
    }

    buffer.open(QBuffer::ReadWrite);
    QDataStream out(&buffer);
    // <<
    const int bufSize = buffer.size();

    if(false == m_sharedMem->create(bufSize))
    {
        qDebug() << __FUNCTION__ <<" Not created SHM Segment";
        return;
    }

    m_sharedMem->lock();

    char *to = (char*)m_sharedMem->data();
    const char *from = buffer.data().data();
    memcpy(to, from, qMin(m_sharedMem->size(), bufSize));

    m_sharedMem->unlock();

}

//========================================================
void ShardMemory::ShmRead(QBuffer &buffer)
//========================================================
{
    if(false == m_sharedMem->attach())
    {
        qDebug() << __FUNCTION__ <<" Not Connected SHM";
        return;
    }

    QDataStream in(&buffer);

    m_sharedMem->lock();
    buffer.setData((char*)m_sharedMem->constData(), m_sharedMem->size());
    buffer.open(QBuffer::ReadOnly);
    m_sharedMem->unlock();
    m_sharedMem->detach();
}
