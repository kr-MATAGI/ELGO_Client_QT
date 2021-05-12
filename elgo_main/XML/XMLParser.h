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
    bool GetBinaryVersionFromXML(QDomElement &element, DEVICE::INIT_CONFIG& dest);

    /** @brief */
    bool GetBinaryPathFromXML(QDomElement &element, DEVICE::INIT_CONFIG &dest);

    /** @brief */
    bool GetNetworkFromXML(QDomElement &element, DEVICE::INIT_CONFIG &dest);

    /** @brief */
    bool GetServerFromXML(QDomElement &element, DEVICE::INIT_CONFIG &dest);
}

#endif // XMLPARSER_H
