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
#include "JSON/JsonWriter.h"
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

        ELGO_CONTROL_LOG("EMIT Signal - ConnectContentServer()");
    }
    else
    {
        ELGO_CONTROL_LOG("Error - Fail to get JWT from server");
    }

    emit m_socket->ConnectContentServerSignal();
}

//========================================================
void NetworkCtrl::SendControlIsReady()
//========================================================
{
    // Control -> Main : ReadyStatus
    QByteArray readyBytes;
    QDataStream readyStream(&readyBytes, QIODevice::WriteOnly);
    readyStream.setVersion(QDataStream::Qt_5_12);
    readyStream << ELGO_SYS::Proc::ELGO_CONTROL;

    /**
     *  @note
     *          ELGO_VIEWER, CONTROL -> ELGO_MAIN
     *          receive status of process started
     *  @param
     *          ELGO_PROC::Proc proc
     */
    const bool bReadyEventy = EFCEvent::SendEvent(ELGO_SYS::Proc::ELGO_MAIN,
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

    // for test - "d335fd2a4-88b4-9dba-bbdf-2dc416a3018";
    QString udid = QSysInfo::machineUniqueId().toStdString().c_str();
//    QString udid = "d335fd2a488b49dbabbdf2dc416a3018";

    QString os = QSysInfo::productType();
    JsonWriter::WriteGetJwtRequest(udid, os, sendJson);
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

        CURLcode curlCode = curl_easy_perform(curl);
        if(CURLE_OK == curlCode)
        {
            retValue = true;
            dest = dataBuffer.c_str();

            ELGO_CONTROL_LOG("jwt : %s", dest.toUtf8().constData());
        }
        else
        {
            QString errorStr = curl_easy_strerror(curlCode);
            ELGO_CONTROL_LOG("Error - resCode : %ld , str : %s, errorBuf : %s", resCode, errorStr.toUtf8().constData(), errorBuffer);
        }
        curl_slist_free_all(headers);
    }
    curl_easy_cleanup(curl);

    return retValue;
}

//========================================================
bool NetworkCtrl::UploadScreenCaptureImage(const QString& savedPath, QString& uploadedPath)
//========================================================
{
    bool retValue = false;


    CURL *curl = curl_easy_init();

    if(curl)
    {
        // url
        QString url = "https://";
        url.append(m_connecInfo.WAS_HOST);
        url.append("/api/device/capture/upload");
        ELGO_CONTROL_LOG("URL : %s", url.toUtf8().constData());

        // curl_form
        struct curl_httppost *formpost = NULL;
        struct curl_httppost *lastPtr = NULL;
        curl_formadd(&formpost,
                     &lastPtr,
                     CURLFORM_COPYNAME, "file",
                     CURLFORM_FILE, savedPath.toStdString().c_str(),
                     CURLFORM_END);

        // set_opt
        std::string responsePath;
        char errorBuffer[CURL_ERROR_SIZE] = {'\0',};
        curl_easy_setopt(curl, CURLOPT_URL, url.toUtf8().constData());
        curl_easy_setopt(curl, CURLOPT_TIMEOUT_MS, CONN_TIMEOUT::DEFAULT_TIMEOUT);
        curl_easy_setopt(curl, CURLOPT_HTTPPOST, formpost);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &responsePath);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &NetworkCtrl::WriteFunction);
        curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, errorBuffer);

        // for debug
        long resCode;
        ELGO_CONTROL_LOG("curl_version() : %s", curl_version());
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &resCode);

        CURLcode curlCode = curl_easy_perform(curl);
        if(CURLE_OK == curlCode)
        {
            retValue = true;
            uploadedPath = responsePath.c_str();
            ELGO_CONTROL_LOG("Response Path : %s", uploadedPath.toUtf8().constData());
        }
        else
        {
            QString errorStr = curl_easy_strerror(curlCode);
            ELGO_CONTROL_LOG("Error - resCode : %ld , str : %s, errorBuf : %s", resCode, errorStr.toUtf8().constData(), errorBuffer);
        }

        curl_formfree(formpost);
        formpost = NULL;
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
void NetworkCtrl::SetRemoteHostIP(const QString& newValue)
//========================================================
{
    m_connecInfo.REMOTE_HOST = newValue;
}

//========================================================
QString& NetworkCtrl::GetRemoteHostIP()
//========================================================
{
    return m_connecInfo.REMOTE_HOST;
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
void NetworkCtrl::SetDeviceName(const QString& value)
//========================================================
{
    m_deviceName = value;
}

//========================================================
QString& NetworkCtrl::GetDeviceName()
//========================================================
{
    return m_deviceName;
}

//========================================================
size_t NetworkCtrl::WriteFunction(void *ptr, size_t size, size_t nmemb, std::string* data)
//========================================================
{
    data->append((char*) ptr, size * nmemb);

    return size * nmemb;
}
