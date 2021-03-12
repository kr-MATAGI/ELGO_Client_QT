
// Control
#include "Logger/ControlLogger.h"
#include "JsonStringConverter.h"

//========================================================
ContentSchema::Event JsonStringConverter::ContentServerEventStringToEnum(const QString& src)
//========================================================
{
    ContentSchema::Event retValue = ContentSchema::Event::NONE_EVENT;

    if(0 == strcmp("ready", src.toStdString().c_str()))
    {
        retValue = ContentSchema::Event::READY;
    }
    else if(0 == strcmp("access", src.toStdString().c_str()))
    {
        retValue = ContentSchema::Event::ACCESS;
    }
    else if(0 == strcmp("error", src.toStdString().c_str()))
    {
        retValue = ContentSchema::Event::ERROR;
    }
    else
    {
        ELGO_CONTROL_LOG("Error - Unkwon Event %s", src.toUtf8().constData());
    }

    return retValue;
}

//========================================================
ContentSchema::PayloadType JsonStringConverter::ContentServerPayloadTypeStringToEnum(const QString& src)
//========================================================
{
    ContentSchema::PayloadType retValue = ContentSchema::PayloadType::NONE_TYPE;

    if(0 == strcmp("once", src.toStdString().c_str()))
    {
        retValue = ContentSchema::PayloadType::ONCE;
    }
    else if(0 == strcmp("request", src.toStdString().c_str()))
    {
        retValue = ContentSchema::PayloadType::REQUEST;
    }
    else if(0 == strcmp("response", src.toStdString().c_str()))
    {
        retValue = ContentSchema::PayloadType::RESPONSE;
    }
    else if(0 == strcmp("progress", src.toStdString().c_str()))
    {
        retValue = ContentSchema::PayloadType::PROGRESS;
    }
    else
    {
        ELGO_CONTROL_LOG("Error - Unkwon payload type : %s", src.toUtf8().constData());
    }

    return retValue;
}

//========================================================
void JsonStringConverter::ContentServerEventEnumToString(const ContentSchema::Event event, QString& dest)
//========================================================
{
    if(ContentSchema::Event::ACCESS == event)
    {
        dest = "access";
    }
    else if(ContentSchema::Event::ERROR == event)
    {
        dest = "error";
    }
    else if(ContentSchema::Event::READY == event)
    {
        dest = "ready";
    }
    else
    {
        dest = "none";
    }
}

//========================================================
void JsonStringConverter::ContentServerPayloadTypeEnumToString(const ContentSchema::PayloadType type, QString& dest)
//========================================================
{
    if(ContentSchema::PayloadType::ONCE == type)
    {
        dest = "once";
    }
    else if(ContentSchema::PayloadType::PROGRESS == type)
    {
        dest = "progress";
    }
    else if(ContentSchema::PayloadType::REQUEST == type)
    {
        dest = "request";
    }
    else if(ContentSchema::PayloadType::RESPONSE == type)
    {
        dest = "response";
    }
    else
    {
        dest = "none";
    }
}
