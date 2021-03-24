// QT
#include <QDateTime>

// Control
#include "JsonParser.h"
#include "JsonStringConverter.h"
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
Remote::Action JsonParser::PaseRemoteActionText(const QString &src)
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
bool JsonParser::ParseRemoteDeviceLogin(const QString &src, Remote::DeviceLogin & dest)
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
bool JsonParser::ParseRemoteManageDevice(const QString &src, Remote::ManageDevice& dest)
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
bool JsonParser::ParseRemoteRotateDevice(const QString &src, Remote::RotateDisplay& dest)
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
bool JsonParser::ParseRemoteDeviceOptions(const QString& src, Remote::DeviceOptions& dest)
//========================================================
{
    bool retValue = true;

    QJsonDocument jsonDoc = QJsonDocument::fromJson(src.toUtf8());
    QJsonObject jsonObj = jsonDoc.object();

    // Device Options
    if(jsonObj.end() != jsonObj.find("deviceOptions"))
    {
        QJsonObject deviceOptions = jsonObj["deviceOptions"].toObject();

        // displayOnOff
        if(deviceOptions.end() != deviceOptions.find("displayOnOff"))
        {
            const bool displayOnOff = deviceOptions["displayOnOff"].toBool();
            dest.displayOnOff = displayOnOff;
        }
        else
        {
            retValue = false;
            ELGO_CONTROL_LOG("Error - deviceOptions.displayOnOff is not existed");
        }

        // deviceMute
        if(deviceOptions.end() != deviceOptions.find("deviceMute"))
        {
            const bool deviceMute = deviceOptions["deviceMute"].toBool();
            dest.deviceMute = deviceMute;
        }
        else
        {
            retValue = false;
            ELGO_CONTROL_LOG("Error - deviceOptions.deviceMute is not existed");
        }

        // contentPause
        if(deviceOptions.end() != deviceOptions.find("contentPause"))
        {
            const bool contentPause = deviceOptions["contentPause"].toBool();
            dest.contentPause = contentPause;
        }
        else
        {
            retValue = false;
            ELGO_CONTROL_LOG("Error - deviceOptions.contentPause is not existed");
        }
    }
    else
    {
        retValue = false;
        ELGO_CONTROL_LOG("Error - deviceOptions Object is not existed");
    }

    return retValue;
}

//========================================================
bool JsonParser::ParseContentServerJsonResponse(const QString& src, ContentSchema::Summary& dest)
//========================================================
{
    bool retValue = true;

    QJsonDocument jsonDoc = QJsonDocument::fromJson(src.toUtf8());
    QJsonObject jsonObj = jsonDoc.object();

    // Event
    if(jsonObj.end() != jsonObj.find("event"))
    {
        QString event = jsonObj["event"].toString();
        dest.event = JsonStringConverter::ContentServerEventStringToEnum(event);
    }
    else
    {
        retValue = false;
        ELGO_CONTROL_LOG("Error - event Object is not existed");
    }

    // Payload
    if(jsonObj.end() != jsonObj.find("payload"))
    {
        ContentSchema::Payload paylod;
        QJsonObject playLoadObj = jsonObj["payload"].toObject();

        const bool bIsParsing = JsonParser::ParsePayloadResponse(playLoadObj, paylod);
        if(false == bIsParsing)
        {
            retValue = false;
            ELGO_CONTROL_LOG("Error - payload Object is not parsed");
        }

        dest.payload = paylod;
    }
    else
    {
        retValue = false;
        ELGO_CONTROL_LOG("Error - payload Object is not existed");
    }

    return retValue;
}

//========================================================
bool JsonParser::ParsePayloadResponse(const QJsonObject& payloadObj, ContentSchema::Payload& dest)
//========================================================
{
    bool retValue = true;

    // src - Required
    if(payloadObj.end() != payloadObj.find("src"))
    {
        QString src = payloadObj["src"].toString();
        dest.src = src;
    }
    else
    {
        retValue = false;
        ELGO_CONTROL_LOG("Error - payload.src Object is not existed");
    }

    // dest - Required
    if(payloadObj.end() != payloadObj.find("dst"))
    {
        QString dst = payloadObj["dst"].toString();
        dest.dest = dst;
    }
    else
    {
        retValue = false;
        ELGO_CONTROL_LOG("Error - payload.dst Object is not existed");
    }

    // type - Required
    if(payloadObj.end() != payloadObj.find("type"))
    {
        QString type = payloadObj["type"].toString();
        dest.type = JsonStringConverter::ContentServerPayloadTypeStringToEnum(type);
    }
    else
    {
        retValue = false;
        ELGO_CONTROL_LOG("Error - paylod.type Object is not existed");
    }

    // d_name (device name) - NOT Required
    if(payloadObj.end() != payloadObj.find("d_name"))
    {
        QString deviceName = payloadObj["d_name"].toString();
        dest.deviceName = deviceName;
    }

    // message - NOT Required
    if(payloadObj.end() != payloadObj.find("message"))
    {
        QString message = payloadObj["message"].toString();
        dest.message = message;
    }

    return retValue;
}

//========================================================
bool JsonParser::ParseSchedulesResponse(const QString& src, QList<ContentSchema::Schedules>& dest)
//========================================================
{
    bool retValue = true;

    QJsonDocument jsonDoc = QJsonDocument::fromJson(src.toUtf8());
    const QJsonObject& jsonObj = jsonDoc.object();

    if(jsonObj.end() != jsonObj.find("schedules"))
    {
        const QJsonObject& schedulesObj = jsonObj["schedules"].toObject();
        QJsonObject::const_iterator constIter = schedulesObj.constBegin();
        QJsonObject::const_iterator constEnd = schedulesObj.constEnd();
        while(constIter != constEnd)
        {
            ContentSchema::Schedules schedules;
            schedules.id = constIter.key();

            const QJsonArray valueArray = constIter.value().toArray();
            QJsonArray::const_iterator arrIter = valueArray.constBegin();
            QJsonArray::const_iterator arrEnd = valueArray.constEnd();
            while(arrIter != arrEnd)
            {
                ContentSchema::ScheduleData scheduleData;
                const QJsonObject arrObj = arrIter->toObject();

                // start
                if(arrObj.end() != arrObj.find("start"))
                {
                    QString startStr = arrObj["start"].toString();
                    QDateTime startDateTime;
                    JsonStringConverter::ScheduleDateTimeStringToQDateTime(startStr, startDateTime);
                    scheduleData.startTime = startDateTime;
                }
                else
                {
                    retValue = false;
                    ELGO_CONTROL_LOG("Error - start object is not existed in array");
                }

                // end
                if(arrObj.end() != arrObj.find("end"))
                {
                    QString endStr = arrObj["end"].toString();
                    QDateTime endDateTime;
                    JsonStringConverter::ScheduleDateTimeStringToQDateTime(endStr, endDateTime);
                    scheduleData.endTime = endDateTime;
                }
                else
                {
                    retValue = false;
                    ELGO_CONTROL_LOG("Error - end object is not existed in array");
                }

                // rule
                if(arrObj.end() != arrObj.find("rule"))
                {
                    QString rule = arrObj["rule"].toString();
                    ContentSchema::Cron cron;
                    JsonStringConverter::CronCommandStringToStruct(rule, cron);
                    scheduleData.cron = cron;
                }
                else
                {
                    retValue = false;
                    ELGO_CONTROL_LOG("Error - rule object is not existed in array");
                }

                // name
                if(arrObj.end() != arrObj.find("name"))
                {
                    QString name = arrObj["name"].toString();
                    scheduleData.name = name;
                }
                else
                {
                    retValue = false;
                    ELGO_CONTROL_LOG("Error - name object is not existed in array");
                }

                schedules.scheduleList.push_back(scheduleData);
                ++arrIter;
            }

            dest.push_back(schedules);
            ++constIter;
        }
    }
    else
    {
        retValue = false;
        ELGO_CONTROL_LOG("Error - schedules object is not existed");
    }

    return retValue;
}

//========================================================
bool JsonParser::ParseWeatherInfoJsonResponse(const QString& src, DownloadDef::Weather::Response& dest)
//========================================================
{
    bool retValue = false;

    QJsonDocument jsonDoc = QJsonDocument::fromJson(src.toUtf8());
    const QJsonObject& jsonObj = jsonDoc.object();

    if(jsonObj.end() != jsonObj.find("response"))
    {
        const QJsonObject& responseObj = jsonObj["response"].toObject();
        if(responseObj.end() != responseObj.find("body"))
        {
            const QJsonObject& bodyObj = responseObj["body"].toObject();
            if(bodyObj.end() != bodyObj.find("items"))
            {
                const QJsonObject& itmesObj = bodyObj["items"].toObject();
                retValue = JsonParser::ParseWeatherItemsJsonResponse(itmesObj, dest);
            }
            else
            {
                ELGO_CONTROL_LOG("Error - Not existed items object");
            }
        }
        else
        {
            ELGO_CONTROL_LOG("Error - Not existed body object");
        }
    }
    else
    {
        ELGO_CONTROL_LOG("Error - Not existed response object");
    }

    return retValue;
}

//========================================================
bool JsonParser::ParseWeatherItemsJsonResponse(const QJsonObject& itemsObj, DownloadDef::Weather::Response& dest)
//========================================================
{
    bool retValue = true;

    if(false == itemsObj.isEmpty())
    {
        const QJsonArray& itemArray = itemsObj["item"].toArray();
        const int itemArraySize = itemArray.size();
        for(int idx = 0; idx < itemArraySize; idx++)
        {
            const QJsonObject& itemObj = itemArray[idx].toObject();
            std::string category = itemObj["category"].toString().toStdString();
            if(0 == strcmp("PTY", category.c_str()))
            {
                const DownloadDef::Weather::PTY pty =
                        static_cast<DownloadDef::Weather::PTY>(itemObj["fcstValue"].toInt());
                dest.SetPty(pty);
            }
            else if(0 == strcmp("SKY", category.c_str()))
            {
                const DownloadDef::Weather::SKY sky =
                        static_cast<DownloadDef::Weather::SKY>(itemObj["fcstValue"].toInt());
                dest.SetSky(sky);
            }
            else if(0 == strcmp("T1H", category.c_str()))
            {
                const QString t1h = itemObj["fcstValue"].toString();
                dest.SetT1h(t1h);
            }
            else if(0 == strcmp("RN1", category.c_str()))
            {
                const int rn1 = itemObj["fcstValue"].toInt();
                dest.SetRn1(rn1);
            }
            else if(0 == strcmp("REH", category.c_str()))
            {
                const int reh = itemObj["fcstValue"].toInt();
                dest.SetReh(reh);
            }
            else if(0 == strcmp("VEC", category.c_str()))
            {
                const QString vec = itemObj["fcstValue"].toString();
                dest.SetVec(vec);
            }
            else if(0 == strcmp("WSD", category.c_str()))
            {
                const QString wsd = itemObj["fcstValue"].toString();
                dest.SetWsd(wsd);
            }
        }
    }
    else
    {
        retValue = false;
        ELGO_CONTROL_LOG("Error - Items Object is Empty");
    }

    return retValue;
}
