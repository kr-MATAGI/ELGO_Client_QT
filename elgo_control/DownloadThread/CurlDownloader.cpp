// Control
#include "CurlDownloader.h"
#include "Logger/ControlLogger.h"

// ext_libs
#include <curl/curl.h>

#define NEWS_BASE_URL    "https://www.yonhapnewstv.co.kr/"
#define WEATHER_BASE_URL    "http://apis.data.go.kr/1360000/VilageFcstInfoService/getUltraSrtFcst?serviceKey="
#define WEATHER_KEY "0E%2Fz8rMxyn%2FK9t68RkJW2vlWcYJJN1M%2FF9Ai56KLbT2mEeDaZXSI24liwFcTuTKXM1RHJzmOA8Nwr5YWXpphhg%3D%3D"

//========================================================
bool CurlDownloader::DownloadNewsFeedXml(DownloadDef::NewsCategory category, QString& dest)
//========================================================
{
    bool retValue = false;

    CURL *curl = curl_easy_init();
    if(curl)
    {
        std::string url = NEWS_BASE_URL;
        if(DownloadDef::NewsCategory::LATEST == category)
        {
            url.append("browse/feed/");
        }
        else
        {
            url.append("category/news/");
            url.append(DownloadDef::categoryEnumToStr[category]);
            url.append("/feed");
        }
        ELGO_CONTROL_LOG("URL - %s", url.c_str());

        std::string dataBuffer;
        char errorBuffer[CURL_ERROR_SIZE] = {'\0',};
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_TIMEOUT_MS, DownloadDef::CURL_TIMEOUT::NEWS_FEED);
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
bool CurlDownloader::DownloadWeatherInfoJson(const DownloadDef::Weather::Request& req, QString& dest)
//========================================================
{
    bool retValue = false;

    CURL *curl = curl_easy_init();
    if(curl)
    {
        // make request url
        /**
         *  @note   numOfRows = count(category) * count(make_time)
         */
        std::string url = WEATHER_BASE_URL;
        url.append(WEATHER_KEY);
        url.append("&numOfRows=200&pageNo=1&nx=");
        url.append(QString::number(req.nx).toStdString());
        url.append("&ny=");
        url.append(QString::number(req.ny).toStdString());
        url.append("&base_date=");
        url.append(req.baseDate.toStdString());
        url.append("&base_time=");
        url.append(req.baseTime.toStdString());
        url.append("&dataType=JSON");
        ELGO_CONTROL_LOG("URL : %s", url.c_str());

        std::string dataBuffer;
        char errorBuffer[CURL_ERROR_SIZE] = {'\0',};
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_TIMEOUT_MS, DownloadDef::CURL_TIMEOUT::WEATHER_INFO);
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
            ELGO_CONTROL_LOG("%s", dataBuffer.c_str());
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
