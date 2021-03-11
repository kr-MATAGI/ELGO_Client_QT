//QT
#include <QJsonDocument>
#include <QJsonObject>
#include <QString>

// Control
#include "JsonParser.h"
#include "Logger/ControlLogger.h"

//========================================================
QString JsonParser::LatestVersionParse(const QString &src)
//========================================================
{
    QJsonDocument doc = QJsonDocument::fromJson(src.toUtf8());
    QJsonObject jsonObj = doc.object();
    QString version = jsonObj["version"].toString();
    ELGO_CONTROL_LOG("Remote Version : %s", version.toUtf8().constData());

    return version;
}

//========================================================
void JsonParser::ParseGetJwtResponse(const QString& src, QString& dest)
//========================================================
{
    QJsonDocument jsonDoc = QJsonDocument::fromJson(src.toUtf8());
    QJsonObject jsonObj = jsonDoc.object();
    if(jsonObj.end() != jsonObj.find("newToken"))
    {
        dest = jsonObj["newToken"].toString();
    }
    else
    {
        ELGO_CONTROL_LOG("Error - newToken Object is no existed");
    }
}

//========================================================
Remote::Action JsonParser::PaseRemoteContorlActionText(const QString &src)
//========================================================
{
    Remote::Action action = Remote::Action::NONE_ACTION;

    QJsonDocument doc = QJsonDocument::fromJson(src.toUtf8());
    QJsonObject jsonObj = doc.object();
    if(jsonObj.end() != jsonObj.find("action"))
    {
        action = static_cast<Remote::Action>(jsonObj["action"].toInt());
    }
    else
    {
        ELGO_CONTROL_LOG("Error - Action Object is not existed");
    }

    return action;
}

//========================================================
bool JsonParser::ParseRemoteControlDeviceLogin(const QString &src, Remote::DeviceLogin & dest)
//========================================================
{
    bool retValue = true;

    QJsonDocument doc = QJsonDocument::fromJson(src.toUtf8());
    QJsonObject jsonObj = doc.object();

    // deviceLogin
    if(jsonObj.end() != jsonObj.find("deviceLogin"))
    {
        QJsonObject deviceObj = jsonObj["deviceLogin"].toObject();

        // id
        if(deviceObj.end() != deviceObj.find("id"))
        {
            QString deviceId = deviceObj["id"].toString();
            dest.id = deviceId;
        }
        else
        {
            retValue = false;
            ELGO_CONTROL_LOG("Error - deviceLogin.id is not existed");
        }

        // pw
        if(deviceObj.end() != deviceObj.find("pw"))
        {
            QString devicePw = deviceObj["pw"].toString();
            dest.pw = devicePw;
        }
        else
        {
            retValue = false;
            ELGO_CONTROL_LOG("Error - deviceLogin.pw is not existed");
        }
    }
    else
    {
        retValue = false;
        ELGO_CONTROL_LOG("Error - deviceLogin Object is not existed");
    }

    return retValue;
}

//========================================================
bool JsonParser::ParseRemoteControlManageDevice(const QString &src, Remote::ManageDevice& dest)
//========================================================
{
    bool retValue = true;

    QJsonDocument jsonDoc = QJsonDocument::fromJson(src.toUtf8());
    QJsonObject jsonObj = jsonDoc.object();

    // manageDevice
    if(jsonObj.end() != jsonObj.find("manageDevice"))
    {
        QJsonObject manageDevie = jsonObj["manageDevice"].toObject();

        // oldPassword
        if(manageDevie.end() != manageDevie.find("oldPassword"))
        {
            QString oldPw = manageDevie["oldPassword"].toString();
            dest.oldPw = oldPw;
        }
        else
        {
            retValue = false;
            ELGO_CONTROL_LOG("Error - manageDevice.oldPassword is not existed");
        }

        // newPassword
        if(manageDevie.end() != manageDevie.find("newPassword"))
        {
            QString newPw = manageDevie["newPassword"].toString();
            dest.newPw = newPw;
        }
        else
        {
            retValue = false;
            ELGO_CONTROL_LOG("Error - manageDevice.newPassword is not existed");
        }
    }
    else
    {
        retValue = false;
        ELGO_CONTROL_LOG("Error - manageDevice Object is not existed");
    }

    return retValue;
}

//========================================================
bool JsonParser::ParseRemoteControlRotateDevice(const QString &src, Remote::RotateDisplay& dest)
//========================================================
{
    bool retValue = true;

    QJsonDocument jsonDoc = QJsonDocument::fromJson(src.toUtf8());
    QJsonObject jsonObj = jsonDoc.object();

    // Rotate Display
    if(jsonObj.end() != jsonObj.find("rotateDisplay"))
    {
        QJsonObject rotateDisplay = jsonObj["rotateDisplay"].toObject();

        // newHeading
        if(rotateDisplay.end() != rotateDisplay.find("newHeading"))
        {
            Remote::Heading heading = static_cast<Remote::Heading>(rotateDisplay["newHeading"].toInt());
            dest.heading = heading;
        }
        else
        {
            retValue = false;
            ELGO_CONTROL_LOG("Error - rotateDisplay.newHeading is not existed");
        }
    }
    else
    {
        retValue = false;
        ELGO_CONTROL_LOG("Error - rotateDisplay Object is not existed");
    }

    return retValue;
}

//========================================================
void JsonParser::MakeDateTimeString(QString& dest)
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

//========================================================
void JsonParser::WriteGetJwtRequest(const QString& uuid, const QString& os, QString& dest)
//========================================================
{
    QJsonDocument jsonDoc;
    QJsonObject jsonObj;

    // uuid & os
    jsonObj["uuid"] = uuid;
    jsonObj["os"] = os;

    jsonDoc.setObject(jsonObj);
    QByteArray compactBytes = jsonDoc.toJson(QJsonDocument::JsonFormat::Compact);
    dest = QString(compactBytes.toStdString().c_str());
}

//========================================================
void JsonParser::WriteDeviceLoginResponse(const Remote::Result::Contents& results, QString& dest)
//========================================================
{
    QJsonDocument jsonDoc;
    QJsonObject jsonObj;

    // date
    QString dateTimeStr;
    JsonParser::MakeDateTimeString(dateTimeStr);
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
void JsonParser::WriteManageDeviceResponse(const Remote::Result::Contents& results, QString& dest)
//========================================================
{
    QJsonDocument jsonDoc;
    QJsonObject jsonObj;

    // date
    QString dateTimeStr;
    JsonParser::MakeDateTimeString(dateTimeStr);
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
void JsonParser::WriteRotateDisplayResponse(const Remote::Result::Contents& results, QString& dest)
//========================================================
{
    QJsonDocument jsonDoc;
    QJsonObject jsonObj;

    // date
    QString dateTimeStr;
    JsonParser::MakeDateTimeString(dateTimeStr);
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
