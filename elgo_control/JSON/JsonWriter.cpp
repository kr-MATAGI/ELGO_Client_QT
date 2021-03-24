// QT
#include <QDateTime>

// Control
#include "JsonWriter.h"
#include "JsonStringConverter.h"
#include "Logger/ControlLogger.h"


//========================================================
void JsonWriter::WriteGetJwtRequest(const QString& udid, const QString& os, std::string& dest)
//========================================================
{
    QJsonDocument jsonDoc;
    QJsonObject jsonObj;

    // uuid & os
    QString beautifyUdid;
    JsonWriter::GetBeautifyUDID(udid, beautifyUdid);
    jsonObj["uuid"] = beautifyUdid;
    jsonObj["os"] = os;

    jsonDoc.setObject(jsonObj);
    QByteArray compactBytes = jsonDoc.toJson(QJsonDocument::JsonFormat::Compact);
    dest = compactBytes.toStdString().c_str();
}

//========================================================
void JsonWriter::WriteDeviceLoginResponse(const Remote::Result::Contents& results, QString& dest)
//========================================================
{
    QJsonDocument jsonDoc;
    QJsonObject jsonObj;

    // date
    QString dateTimeStr;
    JsonWriter::MakeDateTimeString(dateTimeStr);
    jsonObj["date"] = dateTimeStr;

    // result
    bool bResult = false;
    if( Remote::Result::Status::DEVICE_LOGIN_OK == results.status)
    {
        bResult = true;
    }
    QJsonValue jsonValue(bResult);
    jsonObj["result"] = jsonValue.toBool();


    jsonDoc.setObject(jsonObj);
    QByteArray compactBytes = jsonDoc.toJson(QJsonDocument::JsonFormat::Compact);
    dest = QString(compactBytes.toStdString().c_str());
    ELGO_CONTROL_LOG("Json string : %s", dest.toUtf8().constData());
}

//========================================================
void JsonWriter::WriteManageDeviceResponse(const Remote::Result::Contents& results, QString& dest)
//========================================================
{
    QJsonDocument jsonDoc;
    QJsonObject jsonObj;

    // date
    QString dateTimeStr;
    JsonWriter::MakeDateTimeString(dateTimeStr);
    jsonObj["date"] = dateTimeStr;

    // result
    bool bResult = false;
    if(Remote::Result::Status::MANAGE_DEVICE_OK == results.status)
    {
        bResult = true;
    }
    QJsonValue jsonValue(bResult);
    jsonObj["result"] = jsonValue.toBool();

    jsonDoc.setObject(jsonObj);
    QByteArray compactBytes = jsonDoc.toJson(QJsonDocument::JsonFormat::Compact);
    dest = QString(compactBytes.toStdString().c_str());
    ELGO_CONTROL_LOG("Json string : %s", dest.toUtf8().constData());
}

//========================================================
void JsonWriter::WriteRotateDisplayResponse(const Remote::Result::Contents& results, QString& dest)
//========================================================
{
    QJsonDocument jsonDoc;
    QJsonObject jsonObj;

    // date
    QString dateTimeStr;
    JsonWriter::MakeDateTimeString(dateTimeStr);
    jsonObj["date"] = dateTimeStr;

    // result
    bool bResult = false;
    if(Remote::Result::Status::ROTATE_DISPLAY_OK == results.status)
    {
        bResult = true;
    }
    QJsonValue jsonValue(bResult);
    jsonObj["result"] = jsonValue.toBool();

    jsonDoc.setObject(jsonObj);
    QByteArray compactBytes = jsonDoc.toJson(QJsonDocument::JsonFormat::Compact);
    dest = QString(compactBytes.toStdString().c_str());
    ELGO_CONTROL_LOG("Json string : %s", dest.toUtf8().constData());
}

//========================================================
void JsonWriter::WriteDeviceOptionsResponse(const Remote::Result::Contents& results, QString& dest)
//========================================================
{
    QJsonDocument jsonDoc;
    QJsonObject jsonObj;

    // date
    QString dateTimeStr;
    JsonWriter::MakeDateTimeString(dateTimeStr);
    jsonObj["date"] = dateTimeStr;

    // result
    bool bResult = false;
    if(Remote::Result::Status::DEVICE_OPTIONS_OK == results.status)
    {
        bResult = true;
    }
    QJsonValue jsonValue(bResult);
    jsonObj["result"] = jsonValue.toBool();

    jsonDoc.setObject(jsonObj);
    QByteArray compactBytes = jsonDoc.toJson(QJsonDocument::JsonFormat::Compact);
    dest = QString(compactBytes.toStdString().c_str());
    ELGO_CONTROL_LOG("Json String : %s", dest.toUtf8().constData());
}

//========================================================
void JsonWriter::WriteContentServerAccessRequest(const ContentSchema::Summary& src, QString& dest)
//========================================================
{
    QJsonDocument jsonDoc;
    QJsonObject jsonObj;

    // event
    QString event;
    ContentSchema::Event evnetEnum = ContentSchema::Event::ACCESS;
    JsonStringConverter::ContentServerEventEnumToString(evnetEnum, event);
    jsonObj["event"] = event;

    // playload
    QJsonObject payloadObj;
    JsonWriter::WriteContentServerPayloadRequest(src.payload, payloadObj);
    jsonObj["payload"] = payloadObj;

    jsonDoc.setObject(jsonObj);
    QByteArray compactBytes = jsonDoc.toJson(QJsonDocument::JsonFormat::Compact);
    dest = QString(compactBytes.toStdString().c_str());
    ELGO_CONTROL_LOG("Json string : %s", dest.toStdString().c_str());
}

//========================================================
void JsonWriter::WriteContentServerPayloadRequest(const ContentSchema::Payload& src, QJsonObject& dest)
//========================================================
{
    QJsonObject jsonObj;

    // src
    jsonObj["src"] = src.dest;

    // dst
    jsonObj["dst"] = src.src;

    // type
    QString type;
    ContentSchema::PayloadType enumType = ContentSchema::PayloadType::ONCE;
    JsonStringConverter::ContentServerPayloadTypeEnumToString(enumType, type);
    jsonObj["type"] = type;

    // displayPower - Required on ACCESS Event
    bool displayPower = src.displayPower;
    QJsonValue displayPowerValue(displayPower);
    jsonObj["displayPower"] = displayPowerValue.toInt();

    dest = jsonObj;
}

//========================================================
void JsonWriter::GetBeautifyUDID(const QString& src, QString& dest)
//========================================================
{
    const int hyphenIdx[] = {8,12,16,20};
    int cnt = 0;
    for(int idx = 0; idx < src.length(); idx++)
    {
        dest.append(src[idx]);
        if(idx == hyphenIdx[cnt])
        {
            dest.append('-');
            cnt++;
        }
    }
}

//========================================================
void JsonWriter::GetWeatherRequestBaseDateTime(QString& baseDate, QString& baseTime)
//========================================================
{
    const QDateTime dateTime = QDateTime::currentDateTime();

    // date
    const int year = dateTime.date().year();
    const int month = dateTime.date().month();
    const int day = dateTime.date().day();

    char dateBuf[16] = {'\0',};
    sprintf(dateBuf, "%d%02d%02d", year, month, day);
    baseDate = dateBuf;

    // time
    const int hour = dateTime.time().hour();
    const int min = dateTime.time().minute();

    char timeBuf[16] = {'\0', };
    sprintf(timeBuf, "%02d%02d", hour, min);
    baseTime = timeBuf;
}

//========================================================
void JsonWriter::MakeDateTimeString(QString& dest)
//========================================================
{
    // date - yyyy-mm-dd_hh:mm:ss.msec
    QDateTime dateTime = QDateTime::currentDateTime();
    QDate date = dateTime.date();
    QString year = QString::number(date.year());
    QString month = QString::number(date.month());
    QString day = QString::number(date.day());

    // yyyy-mm-dd
    QString dateStr;
    dateStr.append(year);
    dateStr.append("-");
    dateStr.append(month);
    dateStr.append("-");
    dateStr.append(day);

    // hh:mm::ss.msec
    QString timeStr = dateTime.time().toString();
    timeStr.append(".");
    timeStr.append(QString::number(dateTime.time().msec()));

    QString dateTimeStr = dateStr + '_' + timeStr;
    dest = dateTimeStr;
}
