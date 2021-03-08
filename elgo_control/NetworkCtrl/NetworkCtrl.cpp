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

}

//========================================================
NetworkCtrl::~NetworkCtrl()
//========================================================
{

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
void NetworkCtrl::ReplyRemoteVersionFinishedSlot(QNetworkReply *reply)
//========================================================
{
    QString src = reply->readAll();
    m_connecInfo.remoteVersion = JsonParser::LatestVersionParse(src);

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

    /*
    connect(m_netManager, &QNetworkAccessManager::finished, this, &NetworkCtrl::ReplyRemoteVersionFinishedSlot);
    QNetworkRequest request(url);
    m_netManager->get(request);
    */
}
