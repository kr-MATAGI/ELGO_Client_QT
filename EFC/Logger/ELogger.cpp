#include "pthread.h"
#include "ELogger.h"

//========================================================
void ELogger::ELOG_Print(const QString &str)
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
