#ifndef XMLPARSER_H
#define XMLPARSER_H

// QT
#include <QString>

namespace XmlParser
{
    /** @brief */
    bool ParseRssNewsFeedResponse(const QString& src, QStringList& dest);
}

#endif // XMLPARSER_H
