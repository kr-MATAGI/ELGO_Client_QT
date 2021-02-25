#include <QCoreApplication>
#include <QByteArray>
#include <QObject>
#include <QProcess>

// inner
#include "Definition/DeviceDef.h"
#include "MainCtrl/MainCtrl.h"
#include "DB/MainDBCtrl.h"

// EFC
#include "Logger/ELogger.h"
#include "Event/MainEventHandler.h"
#include "LocalSocketEvent/EFCEvent.h"

static MainCtrl *g_MainCtrl = NULL;
static MainEventHandler *g_EventHandler = NULL;

bool StartProcess(::ELGO_PROC::Proc proc)
{
    bool retValue = false;

    QProcess process;
    QStringList args;
    QString program = g_MainCtrl->MakeProcessPath(proc);
    qDebug("[elgo_main] procPath :%s", program.toUtf8().constData());
    retValue = process.startDetached(program, args);
    process.waitForStarted(1000);

    return retValue;
}

void Initialize()
{
    // Get This Device Info
    // to do : save ip to shared mem, checking network connection
    g_MainCtrl->LoadCurrentDeviceInfo();
    ::DEVICE::Info deviceInfo = g_MainCtrl->GetDeviceInfo();
    qDebug("OS : %s, Arch : %s, name : %s, ip : %s, mac : %s, netMask : %s",
           ::DEVICE::OS_enum2str[deviceInfo.os], ::DEVICE::Arch_enum2str[deviceInfo.architec],
            deviceInfo.hostName.toUtf8().constData(), deviceInfo.ip.toUtf8().constData(),
            deviceInfo.mac.toUtf8().constData(), deviceInfo.netMask.toUtf8().constData());

    // Get DB info
    g_MainCtrl->GetDBCtrl().ConnectionDB();

    // Check Wireless Internet
    g_MainCtrl->CheckingWirelessInternet();

    // Start Process
    // TODO : except code about 'false' result and recv proc started results
    const bool bIsStaredControl = StartProcess(::ELGO_PROC::Proc::ELGO_CONTROL);
    const bool bIsStaredViewer = StartProcess(::ELGO_PROC::Proc::ELGO_VIEWER);
    qDebug("[elgo_main] startProccess { contorl : %d, viewer : %d }", bIsStaredControl, bIsStaredViewer);
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    g_MainCtrl = new MainCtrl();
    g_EventHandler = new MainEventHandler(ELGO_PROC::Proc::ELGO_MAIN);

    Initialize();

    return a.exec();
}
