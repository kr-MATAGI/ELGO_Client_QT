// QT
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QByteArray>

// EFC
#include "Common/Deifinition.h"
#include "Common/EventList.h"
#include "LocalSocketEvent/EFCEvent.h"

// Control
#include "JSON/JsonParser.h"
#include "NetworkCtrl.h"
#include "Logger/ControlLogger.h"

//========================================================
NetworkCtrl::NetworkCtrl(QObject *parent)
    : QObject(parent)
//========================================================
{
    // init QNetworkAccessMananger
    m_netManager = new QNetworkAccessManager(this);

    // load server info
    QString xmlInfoPath;
    LoadServerInfoFromXML(xmlInfoPath);

    // Get remote version info from server
    GetRemoteVersionFromWAS();
}

//========================================================
NetworkCtrl::~NetworkCtrl()
//========================================================
{
    delete m_netManager;
    m_netManager = NULL;
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
CONNECT_INFO NetworkCtrl::GetConnectInfo()
//========================================================
{
    return m_connecInfo;
}

//========================================================
void NetworkCtrl::LoadServerInfoFromXML(QString &path)
//========================================================
{
    bool bExistedXML = false;
    QString xmlPath = path;

    if(bExistedXML)
    {
        // is recv info from elgo_main?
    }
    else
    {
        m_connecInfo.WAS_HOST = "demo.elgo.co.kr";
        m_connecInfo.WAS_PORT = 443;

        m_connecInfo.MAIN_SOCKET_HOST = "3.36.160.203";
        m_connecInfo.MAIN_SOCKET_PORT = 9000;

        m_connecInfo.TUNNELING_HOST = "3.36.160.203";
        m_connecInfo.TUNNELING_PORT = 4500;

        m_connecInfo.remoteVersion = "1.0";
        m_connecInfo.deviceVersion = "1.0";
    }
}

//========================================================
void NetworkCtrl::replyRemoteVersionFinished(QNetworkReply *reply)
//========================================================
{
    QString src = reply->readAll();
    m_connecInfo.remoteVersion = JsonParser::RemoteVersionParse(src);

    if(0 != qstrcmp(m_connecInfo.remoteVersion.toUtf8().constData(),
                    m_connecInfo.deviceVersion.toUtf8().constData()))
    {
        // TODO : Update Current Device
    }
}

//========================================================
void NetworkCtrl::GetRemoteVersionFromWAS()
//========================================================
{
    QUrl url = "https://"+ m_connecInfo.WAS_HOST + "/client/version/socket";
    ELGO_CONTROL_LOG("%s", url.toString().toUtf8().constData());

    connect(m_netManager, &QNetworkAccessManager::finished, this, &NetworkCtrl::replyRemoteVersionFinished);
    QNetworkRequest request(url);
    m_netManager->get(request);
}
