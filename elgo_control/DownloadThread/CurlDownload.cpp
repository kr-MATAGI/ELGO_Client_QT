// Control
#include "CurlDownload.h"
#include "Logger/ControlLogger.h"

// ext_libs
#include <curl/curl.h>

#define NEWS_BASE_URL    "https://www.yonhapnewstv.co.kr/"
#define WEATHER_BASE_URL    "http://apis.data.go.kr/1360000/VilageFcstInfoService/getUltraSrtFcst?serviceKey="
#define WEATHER_KEY "0E%2Fz8rMxyn%2FK9t68RkJW2vlWcYJJN1M%2FF9Ai56KLbT2mEeDaZXSI24liwFcTuTKXM1RHJzmOA8Nwr5YWXpphhg%3D%3D"

#define RESOURCE_SAVE_PATH  "/home/jaehoon/바탕화면/ELGO/resource/"

//========================================================
bool CurlDownload::DownloadNewsFeedXml(DownloadDef::NewsCategory category, QString& dest)
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
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &CurlDownload::WriteFunction);
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
bool CurlDownload::DownloadWeatherInfoJson(const DownloadDef::Weather::Request& req, QString& dest)
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
        curl_easy_setopt(curl, CURLOPT_TIMEOUT_MS, DownloadDef::CURL_TIMEOUT::DEFAULT_TIME_OUT);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &dataBuffer);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &CurlDownload::WriteFunction);
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
bool CurlDownload::DownloadResourceList(const QString& url, QString& dest)
//========================================================
{
    bool retValue = false;

    CURL *curl = curl_easy_init();
    if(curl)
    {
        std::string dataBuffer;
        char errorBuffer[CURL_ERROR_SIZE] = {'\0', };
        curl_easy_setopt(curl, CURLOPT_URL, url.toStdString().c_str());
        curl_easy_setopt(curl, CURLOPT_TIMEOUT_MS, DownloadDef::CURL_TIMEOUT::DEFAULT_TIME_OUT);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &dataBuffer);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &CurlDownload::WriteFunction);
        curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, errorBuffer);

        // for debug
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
            ELGO_CONTROL_LOG("Error - %ld: {%s}, {%s}", resCode, errorStr.toUtf8().constData(), errorBuffer);
        }
    }

    curl_easy_cleanup(curl);
    curl = NULL;

    return retValue;
}

//========================================================
bool CurlDownload::DownloadObjectResource(const QString& url, QString& dest)
//========================================================
{
    bool retValue = false;
    ELGO_CONTROL_LOG("url - %s", url.toUtf8().constData());

    CURL *curl = curl_easy_init();
    if(curl)
    {
        std::string dataBuffer;
        char errorBuffer[CURL_ERROR_SIZE] = {'\0', };
        curl_easy_setopt(curl, CURLOPT_URL, url.toStdString().c_str());
        curl_easy_setopt(curl, CURLOPT_TIMEOUT_MS, DownloadDef::CURL_TIMEOUT::DEFAULT_TIME_OUT);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &dataBuffer);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &CurlDownload::WriteFunction);
        curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, errorBuffer);

        // for debug
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
            ELGO_CONTROL_LOG("Error - %ld: {%s}, {%s}", resCode, errorStr.toUtf8().constData(), errorBuffer);
        }


    }
    curl_easy_cleanup(curl);
    curl = NULL;

    return retValue;
}

//========================================================
bool CurlDownload::DownloadResourceData(const ResourceJson::Resource& src)
//========================================================
{
    bool retValue = false;

    // url
    std::string url = src.url.toStdString();
    url += "/";
    url += src.name.toStdString();

    // save path
    std::string savePath = RESOURCE_SAVE_PATH;
    savePath += ResourceJson::ResourceTypeEnumToStr[src.type];
    savePath += "/";
    savePath += src.name.toStdString().c_str();

    // timeout
    int downloadTimeout = DownloadDef::CURL_TIMEOUT::DEFAULT_TIME_OUT;
    if(ResourceJson::ResourceType::VIDEO == src.type)
    {
        downloadTimeout = DownloadDef::CURL_TIMEOUT::VIDEO_DOWNLOAD_TIME_OUT;
    }

    CURL *curl = curl_easy_init();
    if(curl)
    {
        FILE *file = std::fopen(savePath.c_str(), "wb");

        char errorBuffer[CURL_ERROR_SIZE] = {'\0', };
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_TIMEOUT_MS, downloadTimeout);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, file);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &CurlDownload::WirteFileFunction);
        curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, errorBuffer);

        // for debug
        long resCode;
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &resCode);

        CURLcode curlCode = curl_easy_perform(curl);
        if(CURLE_OK == curlCode)
        {
            retValue = true;
            ELGO_CONTROL_LOG("Download - %s", savePath.c_str());
        }
        else
        {
            QString errorStr = curl_easy_strerror(curlCode);
            ELGO_CONTROL_LOG("Error - %s : {%s}, {%s}", url.c_str(), errorStr.toUtf8().constData(), errorBuffer);
        }

        fclose(file);
    }
    curl_easy_cleanup(curl);
    curl = NULL;

    return retValue;
}

//========================================================
size_t CurlDownload::WriteFunction(void *ptr, size_t size, size_t nmemb, std::string* data)
//========================================================
{
    data->append((char*) ptr, size * nmemb);

    return size * nmemb;
}

//========================================================
size_t CurlDownload::WirteFileFunction(void *ptr, size_t size, size_t nmemb, FILE *stream)
//========================================================
{
    size_t written = fwrite(ptr, size, nmemb, stream);

    return written;
}
