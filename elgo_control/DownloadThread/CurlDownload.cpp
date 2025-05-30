// QT
#include <QFile>

// Control
#include "CurlDownload.h"
#include "Logger/ControlLogger.h"

// Common
#include "Common/CommonDef.h"

// ext_libs
#include <curl/curl.h>

//========================================================
bool CurlDownload::DownloadNewsFeedXml(PlayJson::NewsCategory category, QString& dest)
//========================================================
{
    bool retValue = false;

    CURL *curl = curl_easy_init();
    if(curl)
    {
        std::string url = NEWS_BASE_URL;
        if(PlayJson::NewsCategory::LATEST == category)
        {
            url.append("browse/feed/");
        }
        else
        {
            url.append("category/news/");
            url.append(PlayJson::newscategoryEnumToStr[category].toStdString().c_str());
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

    return retValue;
}

//========================================================
bool CurlDownload::DownloadWeatherInfoJson(const int nx, const int ny, QString& dest)
//========================================================
{
    bool retValue = false;

    CURL *curl = curl_easy_init();
    if(curl)
    {
        /**
         *  @note   numOfRows = count(category) * count(make_time)
         */
        QString baseDate;
        QString baseTime;
        MakeWeatherBaseDateTimeStr(baseDate, baseTime);

        std::string url = WEATHER_BASE_URL;
        url.append(WEATHER_KEY);
        url.append("&numOfRows=200&pageNo=1&nx=");
        url.append(QString::number(nx).toStdString());
        url.append("&ny=");
        url.append(QString::number(ny).toStdString());
        url.append("&base_date=");
        url.append(baseDate.toStdString());
        url.append("&base_time=");
        url.append(baseTime.toStdString());
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

    return retValue;
}

//========================================================
bool CurlDownload::DownloadObjectJson(const QString& url, QString& dest)
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

    return retValue;
}

//========================================================
bool CurlDownload::DownloadResourceData(const ResourceJson::Resource& src)
//========================================================
{
    bool retValue = false;

    // save path
    std::string savePath = RESOURCE_DIR_PATH;
    savePath += ResourceJson::ResourceTypeEnumToStr[src.type].toStdString().c_str();
    savePath += "/";
    savePath += src.name.toStdString().c_str();

    // Check Downloaded File
    QFile file(QString(savePath.c_str()));
    if(true == file.exists())
    {
        retValue = true;
        ELGO_CONTROL_LOG("%s is already downloaded", src.name.toStdString().c_str());

        return retValue;
    }

    // timeout
    int downloadTimeout = DownloadDef::CURL_TIMEOUT::DEFAULT_TIME_OUT;
    if(ResourceJson::ResourceType::VIDEO == src.type)
    {
        downloadTimeout = DownloadDef::CURL_TIMEOUT::VIDEO_DOWNLOAD;
    }

    CURL *curl = curl_easy_init();
    if(curl)
    {
        FILE *file = std::fopen(savePath.c_str(), "wb");

        // url
        char *escapeName = curl_easy_escape(curl, src.name.toStdString().c_str(), src.name.toStdString().length());
        std::string url = src.url.toStdString();
        url += "/";
        url += escapeName;


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

    return retValue;
}

//========================================================
void CurlDownload::MakeWeatherBaseDateTimeStr(QString& baseDate, QString& baseTime)
//========================================================
{
    const QDateTime& currDateTime = QDateTime::currentDateTime();

    // ref - api guide
    const QVector<int> baseTimeHourList = {2,5,8,11,14,17,20,23};
    for(int idx = 0; idx < baseTimeHourList.size(); idx++)
    {
        if(currDateTime.time().hour() < baseTimeHourList[idx])
        {
            QDateTime reqDateTime = currDateTime;

            qint64 secEpoch = reqDateTime.toSecsSinceEpoch();
            if(0 >= (idx - 1))
            {
                secEpoch -= 10800; // 3 hour
            }
            else
            {
                secEpoch -= 3600; // 1 hour
            }
            reqDateTime.setSecsSinceEpoch(secEpoch);

            char dateBuffer[16] = {'\0',};
            sprintf(dateBuffer, "%d%02d%02d",
                    reqDateTime.date().year(),
                    reqDateTime.date().month(),
                    reqDateTime.date().day());

            char timeBuffer[16] = {'\0', };
            sprintf(timeBuffer, "%02d%02d",
                    reqDateTime.time().hour(),
                    reqDateTime.time().minute());

            baseDate = dateBuffer;
            baseTime = timeBuffer;

            break;
        }
    }
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
