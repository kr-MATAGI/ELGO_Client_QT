#include "pthread.h"
#include "ELogger.h"


// TODO : Templete

//========================================================
void ELogger::ELOG_PRINT(const QString &str)
//========================================================
{
    qDebug("[%s:%d] %s", __FUNCTION__, __LINE__, str.toUtf8().constData());
}

//========================================================
void ELogger::ELOG_TH_PRINT(const QString &str)
//========================================================
{
    pthread_t id = pthread_self();
    qDebug("[TID:%d][%s:%d] %s", (int)id, __FUNCTION__, __LINE__, str.toUtf8().constData());
}

//========================================================
void ELogger::ELOG_FILE_PRINT(const QString &str)
//========================================================
{
    qDebug("[%s:%s:%d] %s", __FILE__, __FUNCTION__, __LINE__, str.toUtf8().constData());
}
