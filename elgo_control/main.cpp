//QT
#include <QCoreApplication>
#include <QDataStream>

// EFC
#include "Common/Deifinition.h"
#include "Common/EventList.h"
#include "Event/ControlEventHandler.h"
#include "LocalSocketEvent/EFCEvent.h"

// Contorl
#include "NetworkCtrl/NetworkController.h"
#include "NetworkCtrl/RemoteControl/RemoteControlServer.h"

#include "DownloadThread/CurlDownloader.h"
#include "JSON/JsonParser.h"

static NetworkController *g_NetworkController = NetworkController::GetInstance();
static RemoteControlServer *g_RemoteTCPServer = RemoteControlServer::GetInstance();
static ControlEventHandler *g_EventHandler = NULL;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    g_EventHandler = new ControlEventHandler(ELGO_PROC::Proc::ELGO_CONTROL);

    // control -> main
    g_NetworkController->GetNetworkCtrl().SendControlIsReady();

    DownloadDef::Weather::Request request;
    request.baseDate = "20210316";
    request.baseTime = "0800";
    request.nx = 97;
    request.ny = 73;
    QString dest;
    CurlDownloader::DownloadWeatherInfoJson(request, dest);

    DownloadDef::Weather::Response response;
    bool retValue = JsonParser::ParseWeatherInfoJsonResponse(dest, response);
    qDebug() << response.vec;


    return a.exec();
}

