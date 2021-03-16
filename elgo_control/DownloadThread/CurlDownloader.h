#ifndef CURLDOWNLOADER_H
#define CURLDOWNLOADER_H

// QT
#include <QString>

// Control
#include "DownloadDef.h"

namespace CurlDownloader
{
    /** @brief */
    bool DownloadNewsFeedXml(DownloadDef::NewsCategory category, QString& dest);

    /** @brief */
    bool DownloadWeatherInfoJson(const DownloadDef::Weather::Request& req, QString& dest);

    /** @brief */
    static size_t WriteFunction(void *ptr, size_t size, size_t nmemb, std::string* data);
}

#endif // CURLDOWNLOADER_H
