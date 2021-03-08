
#include "JsonParser.h"

//QT
#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>

// Control
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
Remote::Action JsonParser::RemoteContorlActionTextPase(const QString &src)
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
bool JsonParser::RemoteControlDeviceLoginParse(const QString &src, Remote::Summary& dest)
//========================================================
{
    bool retValue = true;

    QJsonDocument doc = QJsonDocument::fromJson(src.toUtf8());
    QJsonObject jsonObj = doc.object();

    // action Parse
    Remote::Action action = Remote::Action::NONE_ACTION;
    if(jsonObj.end() != jsonObj.find("action"))
    {
        action = static_cast<Remote::Action>(jsonObj["action"].toInt());
        dest.action = action;
    }
    else
    {
        retValue = false;
        ELGO_CONTROL_LOG("Error - Action Object is not existed");
    }

    // deviceLogin
    if(jsonObj.end() != jsonObj.find("deviceLogin"))
    {
        Remote::DeviceJson deviceJson;
        QJsonObject deviceObj = jsonObj["deviceLogin"].toObject();

        // id
        if(deviceObj.end() != deviceObj.find("id"))
        {
            QString deviceId = deviceObj["id"].toString();
            deviceJson.id = deviceId;
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
            deviceJson.pw = devicePw;
        }
        else
        {
            retValue = false;
            ELGO_CONTROL_LOG("Error - deviceLogin.pw is not existed");
        }

        dest.device = deviceJson;
    }
    else
    {
        retValue = false;
        ELGO_CONTROL_LOG("Error - DeviceLogin Object is not existed");
    }

    return retValue;
}
