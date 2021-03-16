#ifndef CURLDOWNLOADER_H
#define CURLDOWNLOADER_H

// QT
#include <QString>

/** @brief */
enum NewsCategory
{
    LATEST = 1,
    HEAD_LINE = 2,
    POLITICS = 3,
    ECONOMY =4,
    SOCIETY = 5,
    LOCAL = 6,
    INTERNAL = 7,
    CULTURE = 8,
    SPORTS = 9,
    WEATHER = 10
};

/** @brief */
enum CURL_TIMEOUT
{
    NEWS_FEED = 4 * 1000
};

namespace CurlDownloader
{
    /** @brief */
    bool DownloadNewsFeedXml(NewsCategory category, QString& dest);

    /** @brief */
    static size_t WriteFunction(void *ptr, size_t size, size_t nmemb, std::string* data);
}

#endif // CURLDOWNLOADER_H
