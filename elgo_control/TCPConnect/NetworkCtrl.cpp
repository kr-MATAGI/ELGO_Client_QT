#include "NetworkCtrl.h"

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QByteArray>

#include "JSON/JsonParser.h"

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

    // TODO : send network access result to elgo_main
}

//========================================================
NetworkCtrl::~NetworkCtrl()
//========================================================
{
    delete m_netManager;
    m_netManager = NULL;
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
    xmlPath += "/serverInfo.ini";

    if(bExistedXML)
    {
        // TODO : XML Load feature
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
    qDebug() << url;

    connect(m_netManager, &QNetworkAccessManager::finished, this, &NetworkCtrl::replyRemoteVersionFinished);
    QNetworkRequest request(url);
    m_netManager->get(request);
}
