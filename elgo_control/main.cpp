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

static NetworkController *g_NetworkController = NetworkController::GetInstance();
static ControlEventHandler *g_EventHandler = NULL;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    g_EventHandler = new ControlEventHandler(ELGO_PROC::Proc::ELGO_CONTROL);

    // control -> main
    g_NetworkController->GetNetworkCtrl().SendControlIsReady();

    g_NetworkController->GetNetworkCtrl().TCPSenderTest();

    return a.exec();
}

