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

static NetworkController *g_NetworkController = NetworkController::GetInstance();
static RemoteControlServer *g_RemoteTCPServer = RemoteControlServer::GetInstance();
static ControlEventHandler *g_EventHandler = NULL;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    g_EventHandler = new ControlEventHandler(ELGO_SYS::Proc::ELGO_CONTROL);

    // control -> main
    g_NetworkController->GetNetworkCtrl().SendControlIsReady();

    return a.exec();
}

