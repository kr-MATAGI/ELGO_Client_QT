#include "ShmCtrl.h"

#include <QDebug>
#include <QDataStream>

//========================================================
ShmCtrl::ShmCtrl()
//========================================================
{

}

//========================================================
ShmCtrl::~ShmCtrl()
//========================================================
{
    foreach(QSharedMemory *shm, m_shmList)
    {
        delete shm;
        shm = NULL;
    }
}

//========================================================
bool ShmCtrl::ShmWrite(const QString &key, QBuffer &buffer)
//========================================================
{
    bool retValue = true;

    QSharedMemory *shmem = new QSharedMemory;
    shmem->setKey(key);
    if(shmem->isAttached())
    {
        if(false == shmem->detach())
        {
            qDebug() << __FUNCTION__ <<"Failed Detach SHM";
            retValue = false;

            return retValue;
        }
    }

    buffer.open(QBuffer::ReadWrite);
    const int bufSize = buffer.size();
    if(false == shmem->create(bufSize))
    {
        qDebug() << __FUNCTION__ <<" SHM Error - " << shmem->errorString().toUtf8().constData();
        retValue = false;
    }
    else
    {
        shmem->lock();

        char *to = (char*)shmem->data();
        const char *from = buffer.data().data();
        memcpy(to, from, qMin(shmem->size(), bufSize));

        shmem->unlock();
    }

    m_shmList.push_back(shmem);

    return retValue;
}

//========================================================
bool ShmCtrl::ShmWrite(const QString &key, QByteArray &bytes)
//========================================================
{

    QBuffer buffer(&bytes);
    const bool retValue = ShmWrite(key, buffer);

    return retValue;
}

//========================================================
bool ShmCtrl::ShmRead(const QString &key, QBuffer &buffer)
//========================================================
{
    bool retValue = true;

    QSharedMemory shmem(key);
    if(false == shmem.attach())
    {
        qDebug() << __FUNCTION__ <<" SHM Error -" << shmem.errorString().toUtf8().constData();

        retValue = false;
    }
    else
    {
        QDataStream in(&buffer);

        shmem.lock();

        buffer.setData((char*)shmem.constData(), shmem.size());
        buffer.open(QBuffer::ReadOnly);

        shmem.unlock();
        shmem.detach();
    }

    return retValue;
}

//========================================================
bool ShmCtrl::ShmRead(const QString &key, QByteArray &bytes)
//========================================================
{
    QBuffer buffer(&bytes);
    const bool retValue = ShmRead(key, buffer);

    return retValue;
}
