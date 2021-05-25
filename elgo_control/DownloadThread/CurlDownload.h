#ifndef CURLDOWNLOAD_H
#define CURLDOWNLOAD_H

// Control
#include "DownloadDef.h"
#include "Common/Interface/ContentsPlayDataDef.h"
#include "JSON/Schema/ContentServerSchema.h"

namespace CurlDownload
{
    /** @brief */
    bool DownloadNewsFeedXml(PlayJson::NewsCategory category, QString& dest);

    /** @brief */
    bool DownloadWeatherInfoJson(const int nx, const int ny, QString& dest);

    /** @brief */
    bool DownloadResourceList(const QString& url, QString& dest);

    /** @brief */
    bool DownloadResourceData(const ResourceJson::Resource& src);
    /** @brief */
    bool DownloadObjectJson(const QString& url, QString& dest);

    /** @brief */
    void MakeWeatherBaseDateTimeStr(QString& baseDate, QString& baseTime);


    /** @brief */
    static size_t WriteFunction(void *ptr, size_t size, size_t nmemb, std::string* data);

    /** @brief */
    static size_t WirteFileFunction(void *ptr, size_t size, size_t nmemb, FILE *stream);
};

#endif // CURLDOWNLOAD_H
