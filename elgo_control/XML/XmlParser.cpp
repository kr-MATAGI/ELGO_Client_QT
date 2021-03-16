
// QT
#include <QXmlStreamReader>

// Control
#include "XmlParser.h"
#include "Logger/ControlLogger.h"

//========================================================
bool XmlParser::ParseRssNewsFeedResponse(const QString& src, QStringList& dest)
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

    return retValue;
}
