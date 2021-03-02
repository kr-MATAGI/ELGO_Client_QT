#ifndef XMLPARSER_H
#define XMLPARSER_H

// QT
#include <QtXml>

// Main
#include "Definition/DeviceDef.h"

namespace XMLParser
{
    /** @brief */
    bool LoadInitConfigurationXML(DEVICE::INIT_CONFIG &dest);

    /** @brief */
    bool GetBinaryPathfromInitConfig(QDomElement &element, DEVICE::INIT_CONFIG &dest);

    /** @brief */
    bool GetNetworkfromInitConfig(QDomElement &element, DEVICE::INIT_CONFIG &dest);

    /** @brief */
    bool GetServerfromInitConfig(QDomElement &element, DEVICE::INIT_CONFIG &dest);
}

#endif // XMLPARSER_H
