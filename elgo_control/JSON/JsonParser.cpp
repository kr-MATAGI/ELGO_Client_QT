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
    const QJsonDocument& doc = QJsonDocument::fromJson(src.toUtf8());
    const QJsonObject& jsonObj = doc.object();

    const QString& version = jsonObj["version"].toString();
    ELGO_CONTROL_LOG("Remote Version : %s", version.toUtf8().constData());

    return version;
}

//========================================================
void JsonParser::ParseGetJwtResponse(const QString& src, QString& dest)
//========================================================
{
    const QJsonDocument& jsonDoc = QJsonDocument::fromJson(src.toUtf8());
    const QJsonObject& jsonObj = jsonDoc.object();

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

    const QJsonDocument& doc = QJsonDocument::fromJson(src.toUtf8());
    const QJsonObject& jsonObj = doc.object();

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

    const QJsonDocument& doc = QJsonDocument::fromJson(src.toUtf8());
    const QJsonObject& jsonObj = doc.object();

    // deviceLogin
    if(jsonObj.end() != jsonObj.find("deviceLogin"))
    {
        const QJsonObject& deviceObj = jsonObj["deviceLogin"].toObject();

        // id
        if(deviceObj.end() != deviceObj.find("id"))
        {
            const QString& deviceId = deviceObj["id"].toString();
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
            const QString& devicePw = deviceObj["pw"].toString();
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

    const QJsonDocument& jsonDoc = QJsonDocument::fromJson(src.toUtf8());
    const QJsonObject& jsonObj = jsonDoc.object();

    // manageDevice
    if(jsonObj.end() != jsonObj.find("manageDevice"))
    {
        const QJsonObject& manageDevie = jsonObj["manageDevice"].toObject();

        // oldPassword
        if(manageDevie.end() != manageDevie.find("oldPassword"))
        {
            const QString& oldPw = manageDevie["oldPassword"].toString();
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
            const QString& newPw = manageDevie["newPassword"].toString();
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

    const QJsonDocument& jsonDoc = QJsonDocument::fromJson(src.toUtf8());
    const QJsonObject& jsonObj = jsonDoc.object();

    // Rotate Display
    if(jsonObj.end() != jsonObj.find("rotateDisplay"))
    {
        const QJsonObject& rotateDisplay = jsonObj["rotateDisplay"].toObject();

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

    const QJsonDocument& jsonDoc = QJsonDocument::fromJson(src.toUtf8());
    const QJsonObject& jsonObj = jsonDoc.object();

    // Device Options
    if(jsonObj.end() != jsonObj.find("deviceOptions"))
    {
        const QJsonObject& deviceOptions = jsonObj["deviceOptions"].toObject();

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

    const QJsonDocument& jsonDoc = QJsonDocument::fromJson(src.toUtf8());
    const QJsonObject& jsonObj = jsonDoc.object();

    // Event
    if(jsonObj.end() != jsonObj.find("event"))
    {
        const QString& event = jsonObj["event"].toString();
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
        const QJsonObject& playLoadObj = jsonObj["payload"].toObject();

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
        const QString& src = payloadObj["src"].toString();
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
        const QString& dst = payloadObj["dst"].toString();
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
        const QString& type = payloadObj["type"].toString();
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
        const QString& deviceName = payloadObj["d_name"].toString();
        dest.deviceName = deviceName;
    }

    // URL - if event name is 'singlePlay', 'url' is Required
    if(payloadObj.end() != payloadObj.find("url"))
    {
        const QString& url = payloadObj["url"].toString();
        dest.url = url;
    }

    // message - NOT Required
    if(payloadObj.end() != payloadObj.find("message"))
    {
        const QString& message = payloadObj["message"].toString();
        dest.message = message;
    }

    return retValue;
}

//========================================================
bool JsonParser::ParseSchedulesResponse(const QString& src, QList<ScheduleJson::PlaySchedules>& dest)
//========================================================
{
    bool retValue = true;

    const QJsonDocument& jsonDoc = QJsonDocument::fromJson(src.toUtf8());
    const QJsonObject& jsonObj = jsonDoc.object();

    if(jsonObj.end() != jsonObj.find("schedules"))
    {
        const QJsonObject& schedulesObj = jsonObj["schedules"].toObject();
        QJsonObject::const_iterator constIter = schedulesObj.constBegin();
        QJsonObject::const_iterator constEnd = schedulesObj.constEnd();
        while(constIter != constEnd)
        {
            ScheduleJson::PlaySchedules schedules;
            schedules.id = constIter.key();

            const QJsonArray& valueArray = constIter.value().toArray();
            QJsonArray::const_iterator arrIter = valueArray.constBegin();
            QJsonArray::const_iterator arrEnd = valueArray.constEnd();
            while(arrIter != arrEnd)
            {
                ScheduleJson::PlayScheduleData scheduleData;
                const QJsonObject& arrObj = arrIter->toObject();

                // start
                if(arrObj.end() != arrObj.find("start"))
                {
                    const QString& startStr = arrObj["start"].toString();
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
                    const QString& endStr = arrObj["end"].toString();
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
                    const QString& rule = arrObj["rule"].toString();
                    ScheduleJson::Cron cron;
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
                    const QString& name = arrObj["name"].toString();
                    JsonStringConverter::GetSchedulePlayDataIdInName(name, scheduleData.id);
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
void JsonParser::ParseResourceResponse(const QString& src, QList<ResourceJson::Resource>& dest)
//========================================================
{
    const QJsonDocument& jsonDoc = QJsonDocument::fromJson(src.toUtf8());
    const QJsonArray& resourceArray = jsonDoc.array();

    const int resourceSize = resourceArray.size();
    for(int idx = 0; idx < resourceSize; idx++)
    {
        ResourceJson::Resource resource;
        const QJsonObject& resourceObj = resourceArray[idx].toObject();

        // type
        if(resourceObj.end() != resourceObj.find("type"))
        {
            const QString type = resourceObj["type"].toString();
            resource.type = JsonStringConverter::ResourceTypeStringToEnum(type);
        }

        // name
        if(resourceObj.end() != resourceObj.find("name"))
        {
            const QString name = resourceObj["name"].toString();
            resource.name = name;
        }

        // size
        if(resourceObj.end() != resourceObj.find("size"))
        {
            const int size = resourceObj["size"].toInt();
            resource.size = size;
        }

        // url
        if(resourceObj.end() != resourceObj.find("url"))
        {
            const QString url = resourceObj["url"].toString();
            resource.url = url;
        }

        dest.push_back(resource);
    }
}

//========================================================
void JsonParser::ParseCustomPlayDataJson(const QString& src, ObjectJson::CustomPlayDataJson& dest)
//========================================================
{
    const QJsonDocument& jsonDoc = QJsonDocument::fromJson(src.toUtf8());
    const QJsonObject& jsonObj = jsonDoc.object();

    if(jsonObj.end() != jsonObj.find("page_data"))
    {
        const QJsonArray& pageDataArr = jsonObj["page_data"].toArray();
        if(0 < pageDataArr.size())
        {
            QList<ObjectJson::PageData> pageDataList;
            ParsePageDataJson(pageDataArr, pageDataList);
            dest.pageDataList = pageDataList;
        }
    }
    else
    {
        ELGO_CONTROL_LOG("Error - Not Existed customData.page_data");
    }
}

//========================================================
void JsonParser::ParseFixedPlayDataJson(const QString& src, ObjectJson::FixedPlayDataJson& dest)
//========================================================
{
    const QJsonDocument& jsonDoc = QJsonDocument::fromJson(src.toUtf8());
    const QJsonObject& jsonObj = jsonDoc.object();

    // lyaer Data
    if(jsonObj.end() != jsonObj.find("layer_data"))
    {

        QList<ObjectJson::LayerData> layerDataList;
        const QJsonArray& layerDataArr = jsonObj["layer_data"].toArray();
        ParseLayerDataJson(layerDataArr, layerDataList);

        dest.layerDataList = layerDataList;
    }
    else
    {
        ELGO_CONTROL_LOG("Error - Not Existed fixedPlay.layer_data");
    }

    // subtitle Data
    if(jsonObj.end() != jsonObj.find("subtitle_data"))
    {
        const QJsonArray& subtitleDataArr = jsonObj["subtitle_data"].toArray();
        if(0 < subtitleDataArr.size())
        {
            QList<ObjectJson::SubtitleData> subtitleDataList;
            ParseSubtitleDataJson(subtitleDataArr, subtitleDataList);

            dest.subtitleDataList = subtitleDataList;
        }
    }
}

//========================================================
void JsonParser::ParsePlayDataJson(const QString& src, ObjectJson::PlayData& dest)
//========================================================
{
    const QJsonDocument& jsonDoc = QJsonDocument::fromJson(src.toUtf8());
    const QJsonObject& playDataObj = jsonDoc.object();

    if(playDataObj.end() != playDataObj.find("play_data_id"))
    {
        const int id = playDataObj["play_data_id"].toInt();
        dest.id = id;
    }

    if(playDataObj.end() != playDataObj.find("pld_name"))
    {
        const QString& name = playDataObj["pld_name"].toString();
        dest.name = name;
    }

    if(playDataObj.end() != playDataObj.find("pld_memo"))
    {
        const QString& memo = playDataObj["pld_widht"].toString();
        dest.memo = memo;
    }

    if(playDataObj.end() != playDataObj.find("pld_width"))
    {
        const int width = playDataObj["pld_width"].toInt();
        dest.width = width;
    }

    if(playDataObj.end() != playDataObj.find("pld_height"))
    {
        const int height = playDataObj["pl_height"].toInt();
        dest.height =height;
    }

    if(playDataObj.end() != playDataObj.find("pld_orientation"))
    {
        const QString& orientationStr = playDataObj["pld_orientation"].toString();
        const ObjectJson::Orientation orientation = JsonStringConverter::OrientationTypeStringToEnum(orientationStr);
        dest.orientation = orientation;
    }

    if(playDataObj.end() != playDataObj.find("pld_type"))
    {
        const QString& playDataTypeStr = playDataObj["pld_type"].toString();
        const ObjectJson::PlayDataType playDataType = JsonStringConverter::PlayDataTypeStringToEnum(playDataTypeStr);
        dest.playDataType = playDataType;
    }
}

//========================================================
void JsonParser::ParsePageDataJson(const QJsonArray& pageDataArr, QList<ObjectJson::PageData>& dest)
//========================================================
{
    const int pageDataArrSize = pageDataArr.size();
    for(int idx = 0; idx < pageDataArrSize; idx++)
    {
        ObjectJson::PageData pageData;

        const QJsonObject& pageDataObj = pageDataArr[idx].toObject();
        if(pageDataObj.end() != pageDataObj.find("pgd_duration"))
        {
            const qint64 duration = static_cast<qint64>( pageDataObj["pgd_duration"].toInt());
            pageData.duration = duration;
        }

        // layer data
        const QJsonArray& layerDataArr = pageDataObj["layer_data"].toArray();
        ParseLayerDataJson(layerDataArr, pageData.layerDataList);

        // subtitle data
        const QJsonArray& subtitleDataArr = pageDataObj["subtitle_data"].toArray();
        ParseSubtitleDataJson(subtitleDataArr, pageData.subtitleDataList);

        dest.push_back(pageData);
    }
}

//========================================================
void JsonParser::ParseLayerDataJson(const QJsonArray& layerDataArr, QList<ObjectJson::LayerData>& dest)
//========================================================
{
    const int layDataArrSize = layerDataArr.size();
    for(int idx = 0; idx < layDataArrSize; idx++)
    {
        ObjectJson::LayerData layerData;

        const QJsonObject& layerDataObj = layerDataArr[idx].toObject();
        if(layerDataObj.end() != layerDataObj.find("ld_top"))
        {
            const int top = layerDataObj["ld_top"].toInt();
            layerData.top = top;
        }

        if(layerDataObj.end() != layerDataObj.find("ld_left"))
        {
            const int left = layerDataObj["ld_left"].toInt();
            layerData.left = left;
        }

        if(layerDataObj.end() != layerDataObj.find("ld_width"))
        {
            const int width = layerDataObj["ld_width"].toInt();
            layerData.width = width;
        }

        if(layerDataObj.end() != layerDataObj.find("ld_height"))
        {
            const int height = layerDataObj["ld_height"].toInt();
            layerData.height = height;
        }

        // layer content
        if(layerDataObj.end() != layerDataObj.find("ld_content"))
        {
            const QJsonObject& layerContentObj = layerDataObj["ld_content"].toObject();
            ParseLayerContentJson(layerContentObj, layerData.layerContent);
        }

        dest.push_back(layerData);
    }
}

//========================================================
void JsonParser::ParseLayerContentJson(const QJsonObject& layerContentObj, ObjectJson::LayerContent& dest)
//========================================================
{
    if(layerContentObj.end() != layerContentObj.find("type"))
    {
        const QString& typeString = layerContentObj["type"].toString();
        const QStringList& dividedType = typeString.split("/");

        const QString& contentTypeStr = dividedType[0];
        const QString& mediaTypeStr = dividedType[1];
        const ObjectJson::ContentType contentType
                = JsonStringConverter::ContentTypeStringToEnum(contentTypeStr);
        const ObjectJson::MediaType mediaType
                = JsonStringConverter::MediaTypeStringToEunum(mediaTypeStr);

        dest.contentType.contentType = contentType;
        dest.contentType.mediaType = mediaType;
    }

    if(layerContentObj.end() != layerContentObj.find("name"))
    {
        const QString& name = layerContentObj["name"].toString();
        dest.name = name;
    }

    if(layerContentObj.end() != layerContentObj.find("color"))
    {
        const QString& fontColor = layerContentObj["color"].toString();
        dest.fontColor = fontColor;
    }

    if(layerContentObj.end() != layerContentObj.find("bg_color"))
    {
        const QString& backgroundColor = layerContentObj["bg_color"].toString();
        dest.backgroundColor = backgroundColor;
    }

    if(layerContentObj.end() != layerContentObj.find("bg_opacity"))
    {
        const bool backgroundOpacity = layerContentObj["bg_opacity"].toBool();
        dest.bBackgroundOpacity = backgroundOpacity;
    }

    // clock
    if(ObjectJson::MediaType::CLOCK == dest.contentType.mediaType)
    {
        if(layerContentObj.end() != layerContentObj.find("hour_type"))
        {
            const QString& hourTypeStr = layerContentObj["hour_type"].toString();
            const ObjectJson::HourType hourType
                    = JsonStringConverter::HourTypeStringToEnum(hourTypeStr);
            dest.hourType = hourType;
        }
    }

    // date
    if(ObjectJson::MediaType::DATE == dest.contentType.mediaType)
    {
        if(layerContentObj.end() != layerContentObj.find("date_type"))
        {
           const QString& dateTypeStr = layerContentObj["date_type"].toString();
           const ObjectJson::DateType dateType
                   = JsonStringConverter::DateTypeStringToEnum(dateTypeStr);
           dest.dateType = dateType;
        }
    }

    // weather
    if(ObjectJson::MediaType::WEATHER == dest.contentType.mediaType)
    {
        if(layerContentObj.end() != layerContentObj.find("area"))
        {
            const QString& metropolCityStr = layerContentObj["area"].toString();
            dest.metropolCity = metropolCityStr;
        }

        if(layerContentObj.end() != layerContentObj.find("area_name"))
        {
            const QString& metropolCityNameStr = layerContentObj["area_name"].toString();
            dest.metropolCityName = metropolCityNameStr;
        }

        if(layerContentObj.end() != layerContentObj.find("area2"))
        {
            const QString& cityStr = layerContentObj["area2"].toString();
            dest.city = cityStr;
        }

        if(layerContentObj.end() != layerContentObj.find("area2_name"))
        {
            const QString& cityNameStr = layerContentObj["area2_name"].toString();
            dest.cityName = cityNameStr;
        }
    }

    // news
    if(ObjectJson::MediaType::NEWS == dest.contentType.mediaType)
    {
        if(layerContentObj.end() != layerContentObj.find("category"))
        {
            const QString& newsCategoryStr = layerContentObj["category"].toString();
            const ObjectJson::NewsCategory newsCategory =
                    JsonStringConverter::NewsCategoryStringToEnum(newsCategoryStr);
            dest.newsCategory = newsCategory;
        }

        if(layerContentObj.end() != layerContentObj.find("news_count"))
        {
            const QString& newsCountStr = layerContentObj["news_count"].toString();
            const int newsCount = newsCountStr.toInt();
            dest.newsBoxCount = newsCount;
        }

        if(layerContentObj.end() != layerContentObj.find("font_size"))
        {
            const QString& newsFontSizeStr = layerContentObj["font_size"].toString();
            const int newsFontSize = newsFontSizeStr.toInt();
            dest.newsfontSize = newsFontSize;
        }

        if(layerContentObj.end() != layerContentObj.find("news_bg_color"))
        {
            const QString& newsBoxBackgroundColor = layerContentObj["news_bg_color"].toString();
            dest.newsBoxColor = newsBoxBackgroundColor;
        }

        if(layerContentObj.end() != layerContentObj.find("news_bg_opacity"))
        {
            const bool newsBoxOpacity = layerContentObj["news_bg_opacity"].toBool();
            dest.bNewsBoxOpacity = newsBoxOpacity;
        }
    }
}

//========================================================
void JsonParser::ParseSubtitleDataJson(const QJsonArray& subtitleDataArr, QList<ObjectJson::SubtitleData>& dest)
//========================================================
{
    const int subtitleArrSize = subtitleDataArr.size();
    for(int idx = 0; idx < subtitleArrSize; idx++)
    {
        ObjectJson::SubtitleData subtitleData;

        const QJsonObject& subtitleObj = subtitleDataArr[idx].toObject();
        if(subtitleObj.end() != subtitleObj.find("sd_top"))
        {
            const int top = subtitleObj["sd_top"].toInt();
            subtitleData.top = top;
        }

        if(subtitleObj.end() != subtitleObj.find("sd_left"))
        {
            const int left = subtitleObj["sd_left"].toInt();
            subtitleData.left = left;
        }

        if(subtitleObj.end() != subtitleObj.find("sd_width"))
        {
            const int width = subtitleObj["sd_width"].toInt();
            subtitleData.width = width;
        }

        if(subtitleObj.end() != subtitleObj.find("sd_height"))
        {
            const int height = subtitleObj["sd_height"].toInt();
            subtitleData.height = height;
        }

        if(subtitleObj.end() != subtitleObj.find("sd_text"))
        {
            const QString& text = subtitleObj["sd_text"].toString();
            subtitleData.text = text;
        }

        if(subtitleObj.end() != subtitleObj.find("sd_fixed"))
        {
            const int fixedInteger = subtitleObj["sd_fixed"].toInt();
            if(1 == fixedInteger)
            {
                subtitleData.bIsFixed = true;
            }
            else
            {
                subtitleData.bIsFixed = false;
            }
        }

        if(subtitleObj.end() != subtitleObj.find("sd_orientation"))
        {
            const QString& subtitleOriStr = subtitleObj["sd_orientation"].toString();
            const ObjectJson::Orientation orientation
                    = JsonStringConverter::OrientationTypeStringToEnum(subtitleOriStr);
            subtitleData.orientation = orientation;
        }

        if(false == subtitleData.bIsFixed)
        {
            if(subtitleObj.end() != subtitleObj.find("sd_direction"))
            {
                const QString& subtitleDirStr = subtitleObj["sd_direction"].toString();
                const ObjectJson::SubtitleDirection direction
                        = JsonStringConverter::SubtitleDirectionStringToEnum(subtitleDirStr);
                subtitleData.direction = direction;
            }

            if(subtitleObj.end() != subtitleObj.find("sd_behavior"))
            {
                const QString& subtitleActionStr = subtitleObj["sd_behavior"].toString();
                const ObjectJson::SubtitleAction action
                        = JsonStringConverter::SubtitleActionStringToEnum(subtitleActionStr);
                subtitleData.action = action;
            }
        }

        if(subtitleObj.end() != subtitleObj.find("sd_speed"))
        {
            const int speed = subtitleObj["sd_speed"].toInt();
            subtitleData.speed = speed;
        }

        if(subtitleObj.end() != subtitleObj.find("sd_background_color"))
        {
            const QString& backgroundColor = subtitleObj["sd_background_color"].toString();
            subtitleData.backgroundColor = backgroundColor;
        }

        if(subtitleObj.end() != subtitleObj.find("sd_font_color"))
        {
            const QString& fontColor = subtitleObj["sd_font_color"].toString();
            subtitleData.fontColor = fontColor;
        }

        if(subtitleObj.end() != subtitleObj.find("sd_font_size"))
        {
            const int fontSize = subtitleObj["sd_font_size"].toInt();
            subtitleData.fontSize = fontSize;
        }

        dest.push_back(subtitleData);
    }
}

//========================================================
void JsonParser::ParsePowerSchedulesJson(const QString& src, ScheduleJson::PowerSchedules& dest)
//========================================================
{
    const QJsonDocument& jsonDoc = QJsonDocument::fromJson(src.toUtf8());
    const QJsonObject& jsonObj = jsonDoc.object();

    // on
    if(jsonObj.end() != jsonObj.find("on"))
    {
        const QJsonObject& onSchdulesObj = jsonObj["on"].toObject();
        const int onSchedulesObjSize = onSchdulesObj.size();

        if(0 < onSchedulesObjSize)
        {
            ScheduleJson::PowerScheduleData powerSchedule;
            QJsonObject::const_iterator constIter = onSchdulesObj.constBegin();
            QJsonObject::const_iterator constEnd = onSchdulesObj.constEnd();
            while(constIter != constEnd)
            {
                powerSchedule.id = constIter.key();
                const QJsonObject& scheduleDataObj =  constIter.value().toObject();

                if(scheduleDataObj.end() != scheduleDataObj.find("start"))
                {
                    QDateTime startTime;
                    const QString& startTimeStr = scheduleDataObj["start"].toString();
                    JsonStringConverter::ScheduleDateTimeStringToQDateTime(startTimeStr, startTime);
                    powerSchedule.startTime = startTime;
                }

                if(scheduleDataObj.end() != scheduleDataObj.find("end"))
                {
                    QDateTime endTime;
                    const QString& endTimeStr = scheduleDataObj["end"].toString();
                    JsonStringConverter::ScheduleDateTimeStringToQDateTime(endTimeStr, endTime);
                    powerSchedule.endTime = endTime;
                }

                if(scheduleDataObj.end() != scheduleDataObj.find("rule"))
                {
                    ScheduleJson::Cron cron;
                    const QString& ruleStr = scheduleDataObj["rule"].toString();
                    JsonStringConverter::CronCommandStringToStruct(ruleStr, cron);
                    powerSchedule.cron = cron;
                }

                dest.onScheduleList.push_back(powerSchedule);
                ++constIter;
            }
        }
    }

    // off
    if(jsonObj.end() != jsonObj.find("off"))
    {
        const QJsonObject& offScheduleObj = jsonObj["off"].toObject();
        const int offSchedulesObjSize = offScheduleObj.size();

        if(0 < offSchedulesObjSize)
        {
            ScheduleJson::PowerScheduleData powerSchedule;
            QJsonObject::const_iterator constIter = offScheduleObj.constBegin();
            QJsonObject::const_iterator constEnd = offScheduleObj.constEnd();

            while(constIter != constEnd)
            {
                powerSchedule.id = constIter.key();
                const QJsonObject& scheduleDataObj = constIter.value().toObject();

                if(scheduleDataObj.end() != scheduleDataObj.find("start"))
                {
                    QDateTime startTime;
                    const QString& startTimeStr = scheduleDataObj["start"].toString();
                    JsonStringConverter::ScheduleDateTimeStringToQDateTime(startTimeStr, startTime);
                    powerSchedule.startTime = startTime;
                }

                if(scheduleDataObj.end() != scheduleDataObj.find("end"))
                {
                    QDateTime endTime;
                    const QString& endTimeStr = scheduleDataObj["end"].toString();
                    JsonStringConverter::ScheduleDateTimeStringToQDateTime(endTimeStr, endTime);
                    powerSchedule.endTime = endTime;
                }

                if(scheduleDataObj.end() != scheduleDataObj.find("rule"))
                {
                    ScheduleJson::Cron cron;
                    const QString& ruleStr = scheduleDataObj["rule"].toString();
                    JsonStringConverter::CronCommandStringToStruct(ruleStr, cron);
                    powerSchedule.cron = cron;
                }

                dest.offScheduleList.push_back(powerSchedule);
                ++constIter;
            }
        }
    }
}

//========================================================
void JsonParser::ParseSinglePlaySchedulesJson(const QString& src, ScheduleJson::SinglePlaySchedules& dest)
//========================================================
{
    const QJsonDocument& jsonDoc = QJsonDocument::fromJson(src.toUtf8());
    const QJsonObject& jsonObj = jsonDoc.object();

    if(jsonObj.end() != jsonObj.find("schedules"))
    {
        const QJsonObject& schedulesObj = jsonObj["schedules"].toObject();
        if(schedulesObj.end() != schedulesObj.find("single"))
        {
            const QJsonArray& singleArray = schedulesObj["single"].toArray();
            const int singleArraySize = singleArray.size();
            if(0 < singleArraySize)
            {
                for(int idx = 0; idx < singleArraySize; idx++)
                {
                    ScheduleJson::SinglePlayData singlePlayData;
                    const QJsonObject& singleObj = singleArray[idx].toObject();

                    if(singleObj.end() != singleObj.find("start"))
                    {
                        QDateTime startTime;
                        const QString& startTimeStr = singleObj["start"].toString();
                        JsonStringConverter::ScheduleDateTimeStringToQDateTime(startTimeStr, startTime);
                        singlePlayData.startTime = startTime;
                    }

                    if(singleObj.end() != singleObj.find("end"))
                    {
                        QDateTime endTime;
                        const QString& endTimeStr = singleObj["end"].toString();
                        JsonStringConverter::ScheduleDateTimeStringToQDateTime(endTimeStr, endTime);
                        singlePlayData.endTime = endTime;
                    }

                    if(singleObj.end() != singleObj.find("rule"))
                    {
                        ScheduleJson::Cron cron;
                        const QString& ruleStr = singleObj["rule"].toString();
                        JsonStringConverter::CronCommandStringToStruct(ruleStr, cron);
                        singlePlayData.cron = cron;
                    }

                    if(singleObj.end() != singleObj.find("name"))
                    {
                        const QString& name = singleObj["name"].toString();
                        JsonStringConverter::GetSchedulePlayDataIdInName(name, singlePlayData.id);
                    }

                    dest.schduleList.push_back(singlePlayData);
                }
            }
        }
        else
        {
            ELGO_CONTROL_LOG("Error - Not Existed single array");
        }
    }
    else
    {
        ELGO_CONTROL_LOG("Error - Not Existed schedules object");
    }
}

//========================================================
bool JsonParser::ParseWeatherInfoJsonResponse(const QString& src, DownloadDef::Weather::Response& dest)
//========================================================
{
    bool retValue = false;

    const QJsonDocument& jsonDoc = QJsonDocument::fromJson(src.toUtf8());
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
                const QString& t1h = itemObj["fcstValue"].toString();
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
                const QString& vec = itemObj["fcstValue"].toString();
                dest.SetVec(vec);
            }
            else if(0 == strcmp("WSD", category.c_str()))
            {
                const QString& wsd = itemObj["fcstValue"].toString();
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
