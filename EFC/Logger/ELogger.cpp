#include "ELogger.h"

//QT
#include <QString>
#include <QDebug>
#include <QDateTime>

// C++11
#include <cstdarg>

//========================================================
void ELogger::ELGO_LOG_CONSOLE(const char *procName, const char* fileName, const int line, const char *funcName,
                      const char *format, ...)
//========================================================
{
    va_list ap;
    va_start(ap, format);

    QDateTime dateTime = QDateTime::currentDateTime();
    QString timeStr = dateTime.time().toString();
    timeStr.append(".");
    timeStr.append(QString::number(dateTime.time().msec()));

    QDate date = dateTime.date();
    QString year = QString::number(date.year());
    QString month = QString::number(date.month());
    QString day = QString::number(date.day());

    QString dateStr;
    dateStr.append(year);
    dateStr.append("-");
    dateStr.append(month);
    dateStr.append("-");
    dateStr.append(day);

    QString dateTimeStr = dateStr + ":" + timeStr;
    QString customFileName = QString(fileName).split("/", Qt::SkipEmptyParts).back();
    QString formatLog = QString::vasprintf(format, ap);
    QString printLog = QString("[%1][%2][%3:%4:%5] %6")
                        .arg(dateTimeStr)
                        .arg(procName)
                        .arg(customFileName)
                        .arg(funcName)
                        .arg(line)
                        .arg(formatLog);
    qDebug() << printLog;
    va_end(ap);
}
