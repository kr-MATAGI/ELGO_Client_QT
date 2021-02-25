#include "ShmCtrl.h"

#include <QDebug>
#include <QDataStream>

//========================================================
ShmCtrl::ShmCtrl()
//========================================================
{

}

//========================================================
void ShmCtrl::ShmWrite(const QString &key, QBuffer &buffer)
//========================================================
{
    QSharedMemory shmem(key);

    if(shmem.isAttached())
    {
        if(false == shmem.detach())
        {
            qDebug() << __FUNCTION__ <<"Not Connected SHM";
            return;
        }
    }

    buffer.open(QBuffer::ReadWrite);
    QDataStream out(&buffer);
    const int bufSize = buffer.size();

    if(false == shmem.create(bufSize))
    {
        qDebug() << __FUNCTION__ <<"Not created SHM Segment";
        return;
    }

    shmem.lock();

    char *to = (char*)shmem.data();
    const char *from = buffer.data().data();
    memcpy(to, from, qMin(shmem.size(), bufSize));

    shmem.unlock();

}

//========================================================
void ShmCtrl::ShmRead(const QString &key, QBuffer &buffer)
//========================================================
{
    QSharedMemory shmem(key);

    if(false == shmem.attach())
    {
        qDebug() << __FUNCTION__ <<" Not Connected SHM";
        return;
    }
    QDataStream in(&buffer);

    shmem.lock();
    buffer.setData((char*)shmem.constData(), shmem.size());
    buffer.open(QBuffer::ReadOnly);
    shmem.unlock();
    shmem.detach();
}
