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
#include <curl/curl.h>

//========================================================
NetworkCtrl::NetworkCtrl(QObject *parent)
    : QObject(parent)
//========================================================
{
    m_socket = new ContentWebSocket;
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
        const bool bIsConnected = m_socket->ConnectContentWebSocket();
        if(false == bIsConnected)
        {
            ELGO_CONTROL_LOG("Error - Websocket is not connected with server");
        }
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
void NetworkCtrl::GetBeautifyUDID(const QString& src, QString& dest)
//========================================================
{
    const int hyphenIdx[] = {8,12,16,20};
    int cnt = 0;
    for(int idx = 0; idx < src.length(); idx++)
    {
        dest.append(src[idx]);
        if(idx == hyphenIdx[cnt])
        {
            dest.append('-');
            cnt++;
        }
    }
}

//========================================================
bool NetworkCtrl::GetAccessibleJwtFromServer(QString& dest)
//========================================================
{
    bool retValue = false;

    QString sendJson;
    QString udid;
    QString machineId = QSysInfo::machineUniqueId().toStdString().c_str();
    GetBeautifyUDID(machineId, udid);
    QString os = QSysInfo::productType();
    JsonParser::WriteGetJwtRequest(udid, os, sendJson);
    ELGO_CONTROL_LOG("sendJson : %s", sendJson.toUtf8().constData());

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

        char dataBuffer[JWT_STR_SIZE] = {'\0',};
        char errorBuffer[CURL_ERROR_SIZE] = {'\0', };
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_URL, url.toUtf8().constData());
        curl_easy_setopt(curl, CURLOPT_TIMEOUT_MS, CONN_TIMEOUT::GET_JWT_TIMEOUT);
        curl_easy_setopt(curl ,CURLOPT_POST, 1L);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, sendJson.toUtf8().constData());
        curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, sendJson.size());
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, dataBuffer);
        curl_easy_setopt(curl ,CURLOPT_WRITEFUNCTION, &NetworkCtrl::WriteCallBack);
        curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, errorBuffer);

        // for debug
        // curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
        // qDebug () << curl_version(;

        CURLcode res = curl_easy_perform(curl);
        if(CURLE_OK == res)
        {
            retValue = true;
            dest = dataBuffer;

            long resCode;
            curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &resCode);
            ELGO_CONTROL_LOG("response Code : %ld, %s", resCode, dest.toUtf8().constData());
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
size_t NetworkCtrl::WriteCallBack(char *contents, size_t size, size_t nmemb, void *userData)
//========================================================
{
    ((std::string*)userData)->append((char*)contents, size * nmemb);
    return size&nmemb;
}
