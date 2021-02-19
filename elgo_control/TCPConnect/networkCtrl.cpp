#include "networkCtrl.h"

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QByteArray>

#include <JSON/jsonparser.h>

//========================================================
NetworkCtrl::NetworkCtrl(QObject *parent)
    : QObject(parent)
//========================================================
{
    // load server info
    QString xmlInfoPath;
    LoadServerInfoFromXML(xmlInfoPath);

    // Get remote version info from server
//    GetRemoteVersionFromWAS();
}

//========================================================
NetworkCtrl::~NetworkCtrl()
//========================================================
{

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

        m_connecInfo.MAIN_SOCKET_HOST = "15.165.125.181";
        m_connecInfo.MAIN_SOCKET_PORT = 9000;
        m_connecInfo.SUB_SOCKET_PROT = 9500;

        m_connecInfo.TUNNELING_HOST = "15.165.125.181";
        m_connecInfo.TUNNELING_PORT = 4500;
    }
}

//========================================================
void NetworkCtrl::GetRemoteVersionFromWAS()
//========================================================
{
//    QUrl url = "https://"+ m_connecInfo.WAS_HOST + "/client/version/socket";
//    qDebug() << url;

    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    QNetworkRequest request(QUrl("https://www.naver.com"));
    QNetworkReply *reply = manager->get(request);
    qDebug() << reply->readAll();

    delete manager;
    manager = NULL;
}
