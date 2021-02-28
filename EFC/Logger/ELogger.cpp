#include "ELogger.h"

#include <QString>
#include <QDebug>

#include <cstdarg>

//========================================================
void ELogger::ELGO_LOG_CONSOLE(const char *procName, const char* fileName, const int line, const char *funcName,
                      const char *format, ...)
//========================================================
{
    va_list ap;
    va_start(ap, format);

    QString formatLog = QString::vasprintf(format, ap);
    QString printLog = QString("[%1][%2:%3:%4] %5")
                        .arg(procName)
                        .arg(fileName)
                        .arg(funcName)
                        .arg(line)
                        .arg(formatLog);
    qDebug() << printLog;
    va_end(ap);
}
