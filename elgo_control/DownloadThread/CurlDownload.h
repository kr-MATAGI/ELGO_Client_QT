#ifndef CURLDOWNLOAD_H
#define CURLDOWNLOAD_H

// Control
#include "DownloadDef.h"
#include "JSON/Schema/ContentServerSchema.h"

namespace CurlDownload
{
    /** @brief */
    bool DownloadNewsFeedXml(DownloadDef::NewsCategory category, QString& dest);

    /** @brief */
    bool DownloadWeatherInfoJson(const DownloadDef::Weather::Request& req, QString& dest);

    /** @brief */
    bool DownloadResourceList(const QString& url, QString& dest);

    /** @brief */
    bool DownloadResourceData(const ContentSchema::Resource& src);

    /** @brief */
    static size_t WriteFunction(void *ptr, size_t size, size_t nmemb, std::string* data);

    /** @brief */
    static size_t WirteFileFunction(void *ptr, size_t size, size_t nmemb, FILE *stream);
};

#endif // CURLDOWNLOAD_H
