
// Main
#include "XMLParser.h"
#include "Logger/MainLogger.h"

//========================================================
bool XMLParser::LoadInitConfigurationXML(DEVICE::INIT_CONFIG &dest)
//========================================================
{
    bool retValue = false;

    QDomDocument xmlDoc;
    QFile xmlFile(INIT_CONFIG_FILE_PATH);
    if(false == xmlFile.open(QIODevice::ReadOnly))
    {
        ELGO_MAIN_LOG("ERROR - Not opened XML Config File : %s", INIT_CONFIG_FILE_PATH);
        return retValue;
    }
    if(false == xmlDoc.setContent(&xmlFile))
    {
        ELGO_MAIN_LOG("ERROR - Not Set XML file to Doc");
        return retValue;
    }

    QDomElement root = xmlDoc.documentElement();
    if("configuration" == root.tagName())
    {
        if(false == root.isNull())
        {
            root = root.firstChildElement();
            const bool bReadVersion = GetBinaryVersionFromXML(root, dest);
            root = root.nextSiblingElement();
            const bool bReadBinaryPath = GetBinaryPathFromXML(root, dest);
            root = root.nextSiblingElement();
            const bool bReadNetworkInfo = GetNetworkFromXML(root, dest);
            root = root.nextSiblingElement();
            const bool bReadServerInfo = GetServerFromXML(root, dest);

            if( (true == bReadVersion && true == bReadBinaryPath) &&
                (true == bReadNetworkInfo && true == bReadServerInfo) )
            {
                retValue = true;
            }
            else
            {
                ELGO_MAIN_LOG("ReadVersion: %d ReadBinaryPath : %d, ReadNetworkInfo : %d, ReadServerInfo : %d",
                              bReadVersion, bReadBinaryPath, bReadNetworkInfo, bReadServerInfo);
            }
        }
        else
        {
            ELGO_MAIN_LOG("ERORR - Root is NULL");
        }
    }
    else
    {
        ELGO_MAIN_LOG("ERORR - Root tagName : %s", root.tagName().toUtf8().constData());
    }


    return retValue;
}

//========================================================
bool XMLParser::GetBinaryVersionFromXML(QDomElement &element, DEVICE::INIT_CONFIG& dest)
//========================================================
{
    bool retValue = false;

    QDomElement versionElement = element;
    if("version" == versionElement.tagName())
    {
        if(false == versionElement.isNull())
        {
            dest.version = versionElement.firstChild().toText().data();
        }
        else
        {
            ELGO_MAIN_LOG("ERROR - version is NULL");
        }
    }
    else
    {
        ELGO_MAIN_LOG("ERORR - version tagName : %s",
                      versionElement.tagName().toUtf8().constData());
    }

    return retValue;
}

//========================================================
bool XMLParser::GetBinaryPathFromXML(QDomElement &element, DEVICE::INIT_CONFIG &dest)
//========================================================
{
    bool retValue = false;

    QDomElement binaryElement = element;
    if("binary" == binaryElement.tagName())
    {
        if(false == binaryElement.isNull())
        {
            QDomElement child = binaryElement.firstChildElement();
            if(false == child.isNull())
            {
                if("path" == child.tagName())
                {
                    retValue = true;
                    dest.binaryPath = child.firstChild().toText().data();
                }
                else
                {
                    ELGO_MAIN_LOG("ERORR - binary child %s", child.tagName().toUtf8().constData());
                }
            }
            else
            {
                ELGO_MAIN_LOG("ERORR - binary <path> is NULL");
            }
        }
        else
        {
            ELGO_MAIN_LOG("ERORR - binary is NULL");
        }
    }
    else
    {
        ELGO_MAIN_LOG("ERORR - binary tagName : %s", binaryElement.tagName().toUtf8().constData());
    }

    return retValue;
}

//========================================================
bool XMLParser::GetNetworkFromXML(QDomElement &element, DEVICE::INIT_CONFIG &dest)
//========================================================
{
    bool retValue = false;

    QDomElement networkElement = element;
    if("network" == networkElement.tagName())
    {
        if(false == networkElement.isNull())
        {
            QDomElement child = networkElement.firstChildElement();
            while(false == child.isNull())
            {
                if("ssid" == child.tagName())
                {
                    retValue = true;
                    dest.internet.ssid = child.firstChild().toText().data();
                }
                else if("password" == child.tagName())
                {
                    retValue = true;
                    dest.internet.pw = child.firstChild().toText().data();
                }
                else
                {
                    retValue = false;
                    ELGO_MAIN_LOG("ERORR - Unknown network child : %s", child.tagName().toUtf8().constData());
                }

                child = child.nextSiblingElement();
            }
        }
        else
        {
            ELGO_MAIN_LOG("ERORR - network is NULL");
        }
    }
    else
    {
        ELGO_MAIN_LOG("ERORR - network tagName : %s", networkElement.tagName().toUtf8().constData());
    }

    return retValue;
}

//========================================================
bool XMLParser::GetServerFromXML(QDomElement &element, DEVICE::INIT_CONFIG &dest)
//========================================================
{
    bool retValue = false;

    QDomElement serverElement = element;
    if("server" == serverElement.tagName())
    {
        if(false == serverElement.isNull())
        {
            // deviceConfig
            QDomElement serverUrlElement = serverElement.firstChildElement();
            if("wasHost" == serverUrlElement.tagName())
            {
                if(false == serverUrlElement.isNull())
                {
                    QDomElement wasHostChild = serverUrlElement.firstChildElement();
                    if(false == wasHostChild.isNull() && "url" == wasHostChild.tagName())
                    {
                        retValue = true;
                        dest.server.wasHost = wasHostChild.firstChild().toText().data();
                        wasHostChild = wasHostChild.nextSiblingElement();
                    }

                    if( true == retValue && false == wasHostChild.isNull() && "port" == wasHostChild.tagName())
                    {
                        dest.server.wasHostPort = wasHostChild.firstChild().toText().data().toInt();
                    }
                    else
                    {
                        retValue = false;
                    }
                }
                else
                {
                    ELGO_MAIN_LOG("ERORR - network is NULL");
                }
            }
            else
            {
                ELGO_MAIN_LOG("ERORR - deviceConfig tagName : %s",
                              serverUrlElement.tagName().toUtf8().constData());
            }
        }
        else
        {
            ELGO_MAIN_LOG("ERORR - server is NULL");
        }
    }
    else
    {
        ELGO_MAIN_LOG("ERORR - server tagName : %s", serverElement.tagName().toUtf8().constData());
    }

    return retValue;
}
