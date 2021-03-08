#ifndef JSONPARSER_H
#define JSONPARSER_H

// QT
#include <QString>

// Control
#include "Schema/RemoteSchema.h"

namespace JsonParser
{
    /** @brief */
    QString LatestVersionParse(const QString &src);

    // elgo_remote -> elgo_control
    /** @brief */
    Remote::Action RemoteContorlActionTextPase(const QString &src);

    /** @brief */
    bool RemoteControlDeviceLoginParse(const QString &src, Remote::Summary& dest);

}

#endif // JSONPARSER_H
