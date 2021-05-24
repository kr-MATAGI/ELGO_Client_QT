// QT
#include <QDataStream>

// EFC
#include "Common/Deifinition.h"
#include "LocalSocketEvent/EFCEvent.h"

// Control
#include "Logger/ControlLogger.h"
#include "ControlEventState.h"
#include "ControlThread/ControlThread.h"
#include "DownloadThread/DownloadThread.h"
#include "Definition/WifiDef.h"

//========================================================
ControlEventState::ControlEventState()
//========================================================
{
    m_threadPool = new QThreadPool;
    m_threadPool->setMaxThreadCount(MAX_THREAD_COUNT);

    // proc Event
    m_state.RegisterEvent(CONTROL_EVENT::Event::RECV_SERVER_INFO_FROM_MAIN,
                          &ControlEventState::RecvServerInfoFromMain);
    m_state.RegisterEvent(CONTROL_EVENT::Event::UPDATE_DISPLAY_SLEEP_STATUS,
                          &ControlEventState::RecvUpdateDisplaySleepStatus);
    m_state.RegisterEvent(CONTROL_EVENT::Event::RESPONSE_SCREEN_CAPTURE,
                          &ControlEventState::RecvResponseScreenCapture);
    m_state.RegisterEvent(CONTROL_EVENT::Event::UPDATE_WIFI_LIST,
                          &ControlEventState::RecvUpdateWifiList);
    m_state.RegisterEvent(CONTROL_EVENT::Event::WIFI_CONNECTION_RESULT,
                          &ControlEventState::RecvWifiConnectionResult);
}

//========================================================
ControlEventState::~ControlEventState()
//========================================================
{
    delete m_threadPool;
    m_threadPool = NULL;
}

//========================================================
void ControlEventState::ExecState(const quint16 event, const QByteArray &src)
//========================================================
{
    m_state.Exec(event, src);
}

//========================================================
void ControlEventState::RecvServerInfoFromMain(const QByteArray &src)
//========================================================
{
    /**
     *  @note
     *          ELGO_MAIN -> ELGO_CONTROL
     *          receive WIFI information from main process
     *  @param
     *          QString wasHost,
     *          quint16 wasHostPort,
     *          QString remoteTCPHost
     *          bool bIsDisplaySleep
     */

    ControlThread *thread = new ControlThread;
    thread->SetControlEvent(CONTROL_EVENT::Event::RECV_SERVER_INFO_FROM_MAIN);
    thread->SetRecvBytes(src);

    m_threadPool->start(thread);
}

//========================================================
void ControlEventState::RecvUpdateDisplaySleepStatus(const QByteArray& src)
//========================================================
{
    /**
     *  @note
     *          ELGO_MAIN -> ELGO_CONTROL
     *          Update display sleep status
     *  @param
     *          bool isDisplaySleep
     */

    QByteArray recvBytes = src;
    QDataStream out(&recvBytes, QIODevice::ReadOnly);
    bool bIsDisplaySleep = false;
    out >> bIsDisplaySleep;

    const bool prevStatus = NetworkController::GetInstance()->GetNetworkCtrl().GetDisplaySleepStatus();
    if(prevStatus != bIsDisplaySleep)
    {
        NetworkController::GetInstance()->GetNetworkCtrl().SetDisplaySleepStatus(bIsDisplaySleep);
        ELGO_CONTROL_LOG("Update Display sleep status : %d -> %d", prevStatus, bIsDisplaySleep);
    }
    else
    {
        ELGO_CONTROL_LOG("NOT Updated Display sleep status : %d", prevStatus);
    }
}

//========================================================
void ControlEventState::RecvResponseScreenCapture(const QByteArray& src)
//========================================================
{
    /**
     * @note
     *       ELGO_VIEWER -> ELGO_CONTORL
     *       Receive screen capture response from elgo_viewer
     * @param
     *       QString payload.src
     *       QString payload.dest
     *       QString imagePath
     *       bool    bIsSuccessed
     */
    ControlThread *thread = new ControlThread;
    thread->SetControlEvent(CONTROL_EVENT::Event::RESPONSE_SCREEN_CAPTURE);
    thread->SetRecvBytes(src);

    m_threadPool->start(thread);
}

//========================================================
void ControlEventState::RecvUpdateWifiList(const QByteArray& src)
//========================================================
{
    /**
     * @note
     *       ELGO_MAIN -> ELGO_CONTROL
     *       Finish searching and update wifi list
     * @param
     *       int    wifiCnt
     *       [Loop]
     *       QString    ssid
     *       int    freq
     *       int    signal
     *       bool   bEncryption
     *       [End Loop]
     */
    QByteArray copySrc = src;
    QDataStream recvStream(&copySrc, QIODevice::ReadOnly);
    QVector<WifiInfo> wifiInfoList;
    int wifiCnt = 0;

    recvStream >> wifiCnt;
    ELGO_CONTROL_LOG("WifiCnt : %d", wifiCnt);
    for(int idx = 0; idx < wifiCnt; idx++)
    {
        WifiInfo wifi;
        recvStream >> wifi.ssid;
        recvStream >> wifi.freq;
        recvStream >> wifi.signal;
        recvStream >> wifi.bEnc;
        ELGO_CONTROL_LOG("wifi - {ssid: %s, freq: %d, signal: %d, enc: %d}",
                         wifi.ssid.toStdString().c_str(), wifi.freq,
                         wifi.signal, wifi.bEnc);

        wifiInfoList.push_back(wifi);
    }

    // Send to ELGO_Remote
    Remote::Result::Contents contents;
    contents.status = Remote::Result::Status::UPDATE_WIFI_LIST_OK;
    contents.wifiList = wifiInfoList;
    RemoteControlServer::GetInstance()->SendRemoteResponse(Remote::Action::UPDATE_WIFI_LIST,
                                                           contents);
}

//========================================================
void ControlEventState::RecvWifiConnectionResult(const QByteArray& src)
//========================================================
{
    /**
     * @note
     *          ELGO_MAIN -> ELGO_CONTROL
     *          New WIFI connection result
     * @param
     *          bool bIsConnect
     *          QString newIP
     */

    QByteArray copySrc = src;
    QDataStream dataStream(&copySrc, QIODevice::ReadOnly);
    bool bIsConnect = false;
    QString newIP;
    dataStream >> bIsConnect;
    dataStream >> newIP;
    ELGO_CONTROL_LOG("isConnect: %d, newIP: %s", bIsConnect, newIP.toStdString().c_str());

    // Send to ELGO Remote
    Remote::Result::Contents contents;
    if(true == bIsConnect)
    {
        contents.status = Remote::Result::Status::CONNECT_WIFI_OK;

        NetworkController::GetInstance()->GetNetworkCtrl().SetConnectInfo(newIP);
        const QString& deviceName = NetworkController::GetInstance()->GetNetworkCtrl().GetDeviceName();
        emit RemoteControlServer::GetInstance()->RemoteClientDisconnect();

        // Display New QR Code
        QByteArray sendBytes;
        QDataStream sendStream(&sendBytes, QIODevice::WriteOnly);
        sendStream << newIP;
        sendStream << deviceName;

        /**
         * @note
         *       ELGO_CONTROL -> ELGO_VIEWER
         *       Viewer will make qr code image and display.
         * @param
         *       QString    ip
         *       QString    deviceName
         */
        const bool bSendEvent = EFCEvent::SendEvent(ELGO_SYS::Proc::ELGO_VIEWER,
                                                    VIEWER_EVENT::Event::MAKE_QRCODE,
                                                    sendBytes);
        if(false == bSendEvent)
        {
            ELGO_CONTROL_LOG("Erorr - Send Event: %d", VIEWER_EVENT::Event::MAKE_QRCODE);
        }
    }
    else
    {
        contents.status = Remote::Result::Status::CONNECT_WIFI_FAIL;
        RemoteControlServer::GetInstance()->SendRemoteResponse(Remote::Action::CONNECT_WIFI,
                                                               contents);
    }


}
