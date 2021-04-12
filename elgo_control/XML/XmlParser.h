#ifndef XMLPARSER_H
#define XMLPARSER_H

// QT
#include <QString>

namespace XmlParser
{
    /** @brief */
    bool ParseRssNewsFeedResponse(const QString& src, QVector<QString>& dest);
}

#endif // XMLPARSER_H
