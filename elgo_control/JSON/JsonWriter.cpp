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
void JsonWriter::WriteDeviceLoginResponse(const Remote::Action action,
                                          const Remote::Result::Contents& results,
                                          QString& dest)
//========================================================
{
    QJsonDocument jsonDoc;
    QJsonObject jsonObj;

    // date
    QString dateTimeStr;
    JsonWriter::MakeDateTimeString(dateTimeStr);
    jsonObj["date"] = dateTimeStr;

    // action
    QString actionStr;
    JsonStringConverter::RemoteActionEnumToString(action, actionStr);
    jsonObj["action"] = actionStr;

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
void JsonWriter::WriteManageDeviceResponse(const Remote::Action action,
                                           const Remote::Result::Contents& results,
                                           QString& dest)
//========================================================
{
    QJsonDocument jsonDoc;
    QJsonObject jsonObj;

    // date
    QString dateTimeStr;
    JsonWriter::MakeDateTimeString(dateTimeStr);
    jsonObj["date"] = dateTimeStr;

    // action
    QString actionStr;
    JsonStringConverter::RemoteActionEnumToString(action, actionStr);
    jsonObj["action"] = actionStr;

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
void JsonWriter::WriteRotateDisplayResponse(const Remote::Action action,
                                            const Remote::Result::Contents& results,
                                            QString& dest)
//========================================================
{
    QJsonDocument jsonDoc;
    QJsonObject jsonObj;

    // date
    QString dateTimeStr;
    JsonWriter::MakeDateTimeString(dateTimeStr);
    jsonObj["date"] = dateTimeStr;

    // action
    QString actionStr;
    JsonStringConverter::RemoteActionEnumToString(action, actionStr);
    jsonObj["action"] = actionStr;

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
void JsonWriter::WriteDeviceOptionsResponse(const Remote::Action action,
                                            const Remote::Result::Contents& results,
                                            QString& dest)
//========================================================
{
    QJsonDocument jsonDoc;
    QJsonObject jsonObj;

    // date
    QString dateTimeStr;
    JsonWriter::MakeDateTimeString(dateTimeStr);
    jsonObj["date"] = dateTimeStr;

    // action
    QString actionStr;
    JsonStringConverter::RemoteActionEnumToString(action, actionStr);
    jsonObj["action"] = actionStr;

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
void JsonWriter::WriteUpdateWifiListResponse(const Remote::Action action,
                                 const Remote::Result::Contents& results,
                                 QString& dest)
//========================================================
{
    QJsonDocument jsonDoc;
    QJsonObject jsonObj;

    // date
    QString dateTimeStr;
    JsonWriter::MakeDateTimeString(dateTimeStr);
    jsonObj["date"] = dateTimeStr;

    // action
    QString actionStr;
    JsonStringConverter::RemoteActionEnumToString(action, actionStr);
    jsonObj["action"] = actionStr;

    // result
    bool bResult = false;
    if(Remote::Result::Status::UPDATE_WIFI_OK == results.status)
    {
        bResult = true;
    }
    QJsonValue jsonValue(bResult);
    jsonObj["result"] = jsonValue.toBool();

    // wifiList
    QJsonArray wifiArray;
    const int wifiCnt = results.wifiList.size();
    for(int idx = 0; idx < wifiCnt; idx++)
    {
        QJsonObject wifiObj;
        wifiObj["ssid"] = results.wifiList[idx].ssid;
        wifiObj["freq"] = results.wifiList[idx].freq;
        wifiObj["signal"] = results.wifiList[idx].signal;

        QJsonValue encValue(results.wifiList[idx].bEnc);
        wifiObj["encryption"] = encValue.toBool();

        wifiArray.push_back(wifiObj);
    }
    jsonObj["wifiList"] = wifiArray;

    jsonDoc.setObject(jsonObj);
    QByteArray compactBytes = jsonDoc.toJson(QJsonDocument::JsonFormat::Compact);
    dest = QString(compactBytes.toStdString().c_str());
    ELGO_CONTROL_LOG("Json String : %s", dest.toUtf8().constData());
}

//========================================================
void JsonWriter::WriteContentServerRenameEvent(const ContentSchema::Summary& src, QString& dest)
//========================================================
{
    WriteContentServerDefaultResponse(src, dest);
    ELGO_CONTROL_LOG("Json String : %s", dest.toStdString().c_str());
}

//========================================================
void JsonWriter::WriteContentServerAccessEvent(const ContentSchema::Summary& src, QString& dest)
//========================================================
{
    WriteContentServerDefaultResponse(src, dest);
    ELGO_CONTROL_LOG("Json String : %s", dest.toStdString().c_str());
}

//========================================================
void JsonWriter::WriteContentServerSinglePlayEvent(const ContentSchema::Summary& src, QString& dest)
//========================================================
{
    WriteContentServerDefaultResponse(src, dest);
    ELGO_CONTROL_LOG("Json String : %s", dest.toStdString().c_str());
}

//========================================================
void JsonWriter::WriteContentServerPlayScheduleEvent(const ContentSchema::Summary& src, QString& dest)
//========================================================
{
    WriteContentServerDefaultResponse(src, dest);
    ELGO_CONTROL_LOG("Json String : %s", dest.toStdString().c_str());
}

//========================================================
void JsonWriter::WriteContentServerScreenCaptureEvent(const ContentSchema::Summary& src, QString& dest)
//========================================================
{
    WriteContentServerDefaultResponse(src, dest);
    ELGO_CONTROL_LOG("Json String : %s", dest.toStdString().c_str());
}

//========================================================
void JsonWriter::WriteContentServerDisplayOnOffEvent(const ContentSchema::Summary& src, QString& dest)
//========================================================
{
    WriteContentServerDefaultResponse(src, dest);
    ELGO_CONTROL_LOG("Json String : %s", dest.toStdString().c_str());
}

//========================================================
void JsonWriter::WriteContentServerRebootEvent(const ContentSchema::Summary& src, QString& dest)
//========================================================
{
    WriteContentServerDefaultResponse(src, dest);
    ELGO_CONTROL_LOG("Json String : %s", dest.toStdString().c_str());
}

//========================================================
void JsonWriter::WriteContentServerDefaultResponse(const ContentSchema::Summary& src, QString& dest)
//========================================================
{
    QJsonDocument jsonDoc;
    QJsonObject jsonObj;

    // event
    QString event;
    JsonStringConverter::ContentServerEventEnumToString(src.event, event);
    jsonObj["event"] = event;

    // payload
    QJsonObject payloadObj;
    WriteContentServerPayload(src, payloadObj);
    jsonObj["payload"] = payloadObj;

    jsonDoc.setObject(jsonObj);
    QByteArray compactBytes = jsonDoc.toJson(QJsonDocument::JsonFormat::Compact);
    dest = QString(compactBytes.toStdString().c_str());
}

//========================================================
void JsonWriter::WriteContentServerPayload(const ContentSchema::Summary& src, QJsonObject& dest,
                                           const bool bIsError, const QString& errorStr)
//========================================================
{
    QJsonObject jsonObj;

    // src
    jsonObj["src"] = src.payload.src;

    // dst
    jsonObj["dst"] = src.payload.dest;

    // type
    QString type;
    JsonStringConverter::ContentServerPayloadTypeEnumToString(src.payload.type, type);
    jsonObj["type"] = type;

    if(false == bIsError)
    {
        // displayPower - Required on ACCESS Event
        if(ContentSchema::Event::ACCESS == src.event)
        {
            bool displayPower = src.payload.displayPower;
            QJsonValue displayPowerValue(displayPower);
            jsonObj["displayPower"] = displayPowerValue.toInt();
        }

        // path - Required on Capture Event
        if(ContentSchema::Event::SCREEN_CAPTURE == src.event)
        {
            jsonObj["path"] = src.payload.path;
        }
    }
    else
    {
        jsonObj["error"] = errorStr;
    }

    dest = jsonObj;
}

//========================================================
void JsonWriter::WriteContentServerErrorResponse(const ContentSchema::Summary& src, QString& dest,
                                                 const bool bIsError, const QString& errorStr)
//========================================================
{
    QJsonDocument jsonDoc;
    QJsonObject jsonObj;

    // event
    QString event;
    JsonStringConverter::ContentServerEventEnumToString(src.event, event);
    jsonObj["event"] = event;

    // playload
    QJsonObject payloadObj;
    WriteContentServerPayload(src, payloadObj, bIsError, errorStr);
    jsonObj["payload"] = payloadObj;

    jsonDoc.setObject(jsonObj);
    QByteArray compactBytes = jsonDoc.toJson(QJsonDocument::JsonFormat::Compact);
    dest = QString(compactBytes.toStdString().c_str());
    ELGO_CONTROL_LOG("Json String : %s", dest.toStdString().c_str());
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
