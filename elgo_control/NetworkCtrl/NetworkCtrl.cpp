// QT
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QByteArray>
#include <QUuid>

// EFC
#include "Common/Deifinition.h"
#include "Common/EventList.h"
#include "LocalSocketEvent/EFCEvent.h"

// Control
#include "JSON/JsonParser.h"
#include "NetworkCtrl.h"
#include "Logger/ControlLogger.h"
#include "Definition/ServerInfo.h"

// ext_libs
#include "ext_libs/libcurl/include/curl/curl.h"

//========================================================
NetworkCtrl::NetworkCtrl(QObject *parent)
    : QObject(parent)
    , m_bDisplaySleep(false)
//========================================================
{
    m_socket = new ContentWebSocket(this);
}

//========================================================
NetworkCtrl::~NetworkCtrl()
//========================================================
{
    delete m_socket;
    m_socket = NULL;
}

//========================================================
void NetworkCtrl::ConnectContentWebSocketToServer()
//========================================================
{
    QString recvStr;
    const bool bIsGetJwt = GetAccessibleJwtFromServer(recvStr);
    if(true == bIsGetJwt)
    {
        JsonParser::ParseGetJwtResponse(recvStr, m_jwt);
        emit m_socket->ConnectContentServerSignal();
        ELGO_CONTROL_LOG("EMIT Signal - ConnectContentServer()");
    }
    else
    {
        ELGO_CONTROL_LOG("Error - Fail to get JWT from server");
    }
}

//========================================================
void NetworkCtrl::SendControlIsReady()
//========================================================
{
    // Control -> Main : ReadyStatus
    QByteArray readyBytes;
    QDataStream readyStream(&readyBytes, QIODevice::WriteOnly);
    readyStream.setVersion(QDataStream::Qt_5_12);
    readyStream << ELGO_PROC::Proc::ELGO_CONTROL;

    /**
     *  @brief  receive status of process started
     *  @param  ELGO_PROC::Proc proc
     */
    const bool bReadyEventy = EFCEvent::SendEvent(ELGO_PROC::Proc::ELGO_MAIN,
                                                  MAIN_EVENT::PROCESS_IS_READY, readyBytes);
    if(false == bReadyEventy)
    {
        ELGO_CONTROL_LOG("SendEvent Error - %d", MAIN_EVENT::PROCESS_IS_READY);
    }
}

//========================================================
bool NetworkCtrl::GetAccessibleJwtFromServer(QString& dest)
//========================================================
{
    bool retValue = false;

    std::string sendJson;
    QString udid = QSysInfo::machineUniqueId().toStdString().c_str();
    QString os = QSysInfo::productType();
    JsonParser::WriteGetJwtRequest(udid, os, sendJson);
    ELGO_CONTROL_LOG("sendJson : %s", sendJson.c_str());

    CURL *curl = curl_easy_init();
    if(curl)
    {
        struct curl_slist *headers = NULL;
        headers = curl_slist_append(headers, "Accept: application/json");
        headers = curl_slist_append(headers, "Content-Type: application/json");
        headers = curl_slist_append(headers, "charset: utf-8");

        QString url = "https://";
        url += m_connecInfo.WAS_HOST;
        url += JWT_URL;
        ELGO_CONTROL_LOG("URL : %s", url.toUtf8().constData());

        std::string dataBuffer;
        char errorBuffer[CURL_ERROR_SIZE] = {'\0', };
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_URL, url.toUtf8().constData());
        curl_easy_setopt(curl, CURLOPT_TIMEOUT_MS, CONN_TIMEOUT::GET_JWT_TIMEOUT);
        curl_easy_setopt(curl ,CURLOPT_POST, 1L);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, sendJson.c_str());
        curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, sendJson.size());
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &dataBuffer);
        curl_easy_setopt(curl ,CURLOPT_WRITEFUNCTION, &NetworkCtrl::WriteFunction);
        curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, errorBuffer);

        // for debug
        // curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
        long resCode;
        ELGO_CONTROL_LOG("curl_version() : %s", curl_version());
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &resCode);

        CURLcode res = curl_easy_perform(curl);
        ELGO_CONTROL_LOG("cURL Response Code : %ld", resCode);
        if(CURLE_OK == res)
        {
            retValue = true;
            dest = dataBuffer.c_str();

            ELGO_CONTROL_LOG("jwt : %s", dest.toUtf8().constData());
        }
        else
        {
            QString errorStr = curl_easy_strerror(res);
            ELGO_CONTROL_LOG("Error - str : %s, errorBuf : %s", errorStr.toUtf8().constData(), errorBuffer);
        }
        curl_slist_free_all(headers);
    }
    curl_easy_cleanup(curl);
    curl = NULL;

    return retValue;
}

//========================================================
ContentWebSocket& NetworkCtrl::GetContentWebSocket()
//========================================================
{
    return *m_socket;
}

//========================================================
void NetworkCtrl::SetConnectInfo(const CONNECT_INFO& newValue)
//========================================================
{
    m_connecInfo = newValue;
}

//========================================================
CONNECT_INFO NetworkCtrl::GetConnectInfo()
//========================================================
{
    return m_connecInfo;
}

//========================================================
QString& NetworkCtrl::GetJWTString()
//========================================================
{
    return m_jwt;
}

//========================================================
void NetworkCtrl::SetDisplaySleepStatus(const bool newValue)
//========================================================
{
    m_bDisplaySleep = newValue;
}

//========================================================
bool NetworkCtrl::GetDisplaySleepStatus()
//========================================================
{
    return m_bDisplaySleep;
}

//========================================================
size_t NetworkCtrl::WriteFunction(void *ptr, size_t size, size_t nmemb, std::string* data)
//========================================================
{
    data->append((char*) ptr, size * nmemb);

    return size * nmemb;
}
