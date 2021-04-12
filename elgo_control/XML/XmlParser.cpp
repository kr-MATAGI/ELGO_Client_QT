
// QT
#include <QXmlStreamReader>

// Control
#include "XmlParser.h"
#include "Logger/ControlLogger.h"

//========================================================
bool XmlParser::ParseRssNewsFeedResponse(const QString& src, QVector<QString>& dest)
//========================================================
{
    bool retValue = true;

    QXmlStreamReader xmlReader(src);

    bool bPassedItemElement = false;
    while(false == xmlReader.atEnd())
    {
        xmlReader.readNext();
        if(xmlReader.isEndDocument())
        {
            break;
        }

        if(true == xmlReader.isStartElement())
        {
            QStringRef elementName = xmlReader.name();
            if("item" == elementName)
            {
                bPassedItemElement = true;
            }
            else if(true == bPassedItemElement && "title" == elementName)
            {
                QXmlStreamAttributes attributes = xmlReader.attributes();
                QString readText = xmlReader.readElementText();
                dest.push_back(readText);
            }
        }
    }

    if(true == xmlReader.hasError())
    {
        retValue = false;
        ELGO_CONTROL_LOG("Error - %s", xmlReader.errorString().data());
    }
    else
    {
        ELGO_CONTROL_LOG("News feed parsing results (Size :%d)", dest.size());
        foreach(const QString& item, dest)
        {
            ELGO_CONTROL_LOG("%s", item.toUtf8().constData());
        }
    }

    return retValue;
}
