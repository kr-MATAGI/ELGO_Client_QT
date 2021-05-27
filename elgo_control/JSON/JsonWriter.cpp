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
void JsonWriter::WriteDateActionResultResponse(const Remote::Action action,
                                   const Remote::Result::Contents& contents,
                                   QJsonObject& dest)
//========================================================
{
    // date
    QString dateTimeStr;
    JsonWriter::MakeDateTimeString(dateTimeStr);
    dest["date"] = dateTimeStr;

    // action
    QString actionStr;
    JsonStringConverter::RemoteActionEnumToString(action, actionStr);
    dest["action"] = actionStr;

    // result
    bool bResult = false;
    if( (Remote::Result::Status::DEVICE_LOGIN_OK == contents.status) ||
        (Remote::Result::Status::MANAGE_DEVICE_OK == contents.status) ||
        (Remote::Result::Status::ROTATE_DISPLAY_OK == contents.status) ||
        (Remote::Result::Status::DEVICE_OPTIONS_OK == contents.status) ||
        (Remote::Result::Status::UPDATE_WIFI_LIST_OK == contents.status) ||
        (Remote::Result::Status::CONNECT_WIFI_OK == contents.status) ||
        (Remote::Result::Status::USER_LOGIN_OK == contents.status) )
    {
        bResult = true;
    }

    QJsonValue jsonValue(bResult);
    dest["result"] = jsonValue.toBool();
}

//========================================================
void JsonWriter::WriteDeviceLoginResponse(const Remote::Action action,
                                          const Remote::Result::Contents& contents,
                                          QString& dest)
//========================================================
{
    QJsonDocument jsonDoc;
    QJsonObject jsonObj;

    WriteDateActionResultResponse(action, contents, jsonObj);

    jsonDoc.setObject(jsonObj);
    QByteArray compactBytes = jsonDoc.toJson(QJsonDocument::JsonFormat::Compact);
    dest = QString(compactBytes.toStdString().c_str());
    ELGO_CONTROL_LOG("Json string : %s", dest.toUtf8().constData());
}

//========================================================
void JsonWriter::WriteManageDeviceResponse(const Remote::Action action,
                                           const Remote::Result::Contents& contents,
                                           QString& dest)
//========================================================
{
    QJsonDocument jsonDoc;
    QJsonObject jsonObj;

    WriteDateActionResultResponse(action, contents, jsonObj);

    jsonDoc.setObject(jsonObj);
    QByteArray compactBytes = jsonDoc.toJson(QJsonDocument::JsonFormat::Compact);
    dest = QString(compactBytes.toStdString().c_str());
    ELGO_CONTROL_LOG("Json string : %s", dest.toUtf8().constData());
}

//========================================================
void JsonWriter::WriteRotateDisplayResponse(const Remote::Action action,
                                            const Remote::Result::Contents& contents,
                                            QString& dest)
//========================================================
{
    QJsonDocument jsonDoc;
    QJsonObject jsonObj;

    WriteDateActionResultResponse(action, contents, jsonObj);

    jsonDoc.setObject(jsonObj);
    QByteArray compactBytes = jsonDoc.toJson(QJsonDocument::JsonFormat::Compact);
    dest = QString(compactBytes.toStdString().c_str());
    ELGO_CONTROL_LOG("Json string : %s", dest.toUtf8().constData());
}

//========================================================
void JsonWriter::WriteDeviceOptionsResponse(const Remote::Action action,
                                            const Remote::Result::Contents& contents,
                                            QString& dest)
//========================================================
{
    QJsonDocument jsonDoc;
    QJsonObject jsonObj;

    WriteDateActionResultResponse(action, contents, jsonObj);

    jsonDoc.setObject(jsonObj);
    QByteArray compactBytes = jsonDoc.toJson(QJsonDocument::JsonFormat::Compact);
    dest = QString(compactBytes.toStdString().c_str());
    ELGO_CONTROL_LOG("Json String : %s", dest.toUtf8().constData());
}

//========================================================
void JsonWriter::WriteUpdateWifiListResponse(const Remote::Action action,
                                 const Remote::Result::Contents& contents,
                                 QString& dest)
//========================================================
{
    QJsonDocument jsonDoc;
    QJsonObject jsonObj;

    WriteDateActionResultResponse(action, contents, jsonObj);

    // wifiList
    QJsonArray wifiArray;
    const int wifiCnt = contents.wifiList.size();
    for(int idx = 0; idx < wifiCnt; idx++)
    {
        QJsonObject wifiObj;
        wifiObj["ssid"] = contents.wifiList[idx].ssid;
        wifiObj["freq"] = contents.wifiList[idx].freq;
        wifiObj["signal"] = contents.wifiList[idx].signal;

        QJsonValue encValue(contents.wifiList[idx].bEnc);
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
void JsonWriter::WriteConnectWifiResultResponse(const Remote::Action action,
                                                const Remote::Result::Contents& contents,
                                                QString& dest)
//========================================================
{
    QJsonDocument jsonDoc;
    QJsonObject jsonObj;

    WriteDateActionResultResponse(action, contents, jsonObj);

    jsonDoc.setObject(jsonObj);
    QByteArray compactBytes = jsonDoc.toJson(QJsonDocument::JsonFormat::Compact);
    dest = QString(compactBytes.toStdString().c_str());
    ELGO_CONTROL_LOG("Json String : %s", dest.toUtf8().constData());
}

//========================================================
void JsonWriter::WriteRemoteUserLoginResponse(const Remote::Action action,
                                              const Remote::Result::Contents& contents,
                                              QString& dest)
//========================================================
{
    QJsonDocument jsonDoc;
    QJsonObject jsonObj;

    WriteDateActionResultResponse(action, contents, jsonObj);

    // os
    jsonObj["os"] = contents.os;

    // udid
    QString beautifyUdid;
    JsonWriter::GetBeautifyUDID(contents.udid, beautifyUdid);
    jsonObj["udid"] = beautifyUdid;

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
void JsonWriter::WriteContentServerClearPlayScheduleEvent(const ContentSchema::Summary& src, QString& dest)
//========================================================
{
    WriteContentServerDefaultResponse(src, dest);
    ELGO_CONTROL_LOG("Json String : %s", dest.toStdString().c_str());
}

//========================================================
void JsonWriter::WriteContentServerPowerScheduleEvent(const ContentSchema::Summary& src, QString& dest)
//========================================================
{
    WriteContentServerDefaultResponse(src, dest);
    ELGO_CONTROL_LOG("Json String : %s", dest.toStdString().c_str());
}

//========================================================
void JsonWriter::WriteContentServerClearPowerScheduleEvent(const ContentSchema::Summary& src, QString& dest)
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
            if(true == displayPower)
            {
                jsonObj["displayPower"] = 1;
            }
            else
            {
                jsonObj["displayPower"] = 0;
            }
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
                                                 const QString& errorStr)
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
    WriteContentServerPayload(src, payloadObj, true, errorStr);
    jsonObj["payload"] = payloadObj;

    jsonDoc.setObject(jsonObj);
    QByteArray compactBytes = jsonDoc.toJson(QJsonDocument::JsonFormat::Compact);
    dest = QString(compactBytes.toStdString().c_str());
    ELGO_CONTROL_LOG("Json String : %s", dest.toStdString().c_str());
}

//========================================================
void JsonWriter::WriteContentServerProgressResponse(const ContentSchema::Summary& src, QString& dest)
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
