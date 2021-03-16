// Control
#include "CurlDownloader.h"
#include "Logger/ControlLogger.h"

// ext_libs
#include <curl/curl.h>

#define BASE_URL    "https://www.yonhapnewstv.co.kr/"
static const char* categoryEnumToStr[] = { "latest", "headline", "politics",
                                           "economy", "society", "local",
                                            "international", "culture", "sports", "weather"};

//========================================================
bool CurlDownloader::DownloadNewsFeedXml(NewsCategory category, QString& dest)
//========================================================
{
    bool retValue = false;

    CURL *curl = curl_easy_init();
    if(curl)
    {
        std::string url = BASE_URL;
        if(NewsCategory::LATEST == category)
        {
            url.append("browse/feed/");
        }
        else
        {
            url.append("category/news/");
            url.append(categoryEnumToStr[category]);
            url.append("/feed");
        }
        ELGO_CONTROL_LOG("URL - %s", url.c_str());

        std::string dataBuffer;
        char errorBuffer[CURL_ERROR_SIZE] = {'\0',};
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_TIMEOUT_MS, 1000);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &dataBuffer);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &CurlDownloader::WriteFunction);
        curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, errorBuffer);

        //for debug
        long resCode;
        ELGO_CONTROL_LOG("curl_version : %s", curl_version());
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &resCode);

        CURLcode curlCode = curl_easy_perform(curl);
        if(CURLE_OK == curlCode)
        {
            retValue = true;
            dest = dataBuffer.c_str();
        }
        else
        {
            QString errorStr = curl_easy_strerror(curlCode);
            ELGO_CONTROL_LOG("Error - {%s}, {%s}", errorStr.toUtf8().constData(), errorBuffer);
        }
    }
    curl_easy_cleanup(curl);
    curl = NULL;

    return retValue;
}

//========================================================
size_t CurlDownloader::WriteFunction(void *ptr, size_t size, size_t nmemb, std::string* data)
//========================================================
{
    data->append((char*) ptr, size * nmemb);

    return size * nmemb;
}
