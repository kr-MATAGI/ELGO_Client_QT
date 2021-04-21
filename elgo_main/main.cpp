#include <QCoreApplication>
#include <QByteArray>
#include <QObject>
#include <QProcess>

// Main
#include "MainCtrl/MainController.h"
#include "Definition/DeviceDef.h"
#include "MainCtrl/MainCtrl.h"
#include "DB/MainDBCtrl.h"
#include "Logger/MainLogger.h"

// EFC
#include "Logger/ELogger.h"
#include "Event/MainEventHandler.h"
#include "LocalSocketEvent/EFCEvent.h"

static MainController *g_MainController = MainController::GetInstance();
static MainEventHandler *g_EventHandler = NULL;

bool StartProcess(::ELGO_SYS::Proc proc)
{
    bool retValue = false;

    QProcess process;
    QStringList args;
    QString program = g_MainController->GetMainCtrl().MakeProcessPath(proc);
    ELGO_MAIN_LOG("procPath : %s", program.toUtf8().constData());
    retValue = process.startDetached(program, args);
    process.waitForStarted();

    return retValue;
}

void Initialize()
{
    // Get This Device Info
    // to do : save ip to shared mem, checking network connection
    g_MainController->GetInstance()->GetMainCtrl().LoadCurrentDeviceInfo();
    ::DEVICE::Info deviceInfo = g_MainController->GetMainCtrl().GetDeviceInfo();
    ELGO_MAIN_LOG("OS : %s, Arch : %s, name : %s, ip : %s, mac : %s",
           ::DEVICE::OS_enum2str[deviceInfo.os],
            ::DEVICE::Arch_enum2str[deviceInfo.architec],
            deviceInfo.hostName.toUtf8().constData(), deviceInfo.ipAddr.ip.toUtf8().constData(),
            deviceInfo.ipAddr.mac.toUtf8().constData());

    // Check Resource Folder
    g_MainController->GetInstance()->GetMainCtrl().CheckResourceFolder();

    // Get DB info
    g_MainController->GetInstance()->GetDBCtrl().ConnectionDB();

    // Load Configuration
    g_MainController->GetInstance()->GetMainCtrl().LoadConfigurationInfo();

    // Check Display Sleep Status
    g_MainController->GetInstance()->GetMainCtrl().CheckDisplaySleepStatus();

    // Start Process
    // TODO : except code about 'false' result and recv proc started results
    const bool bIsStaredControl = StartProcess(::ELGO_SYS::Proc::ELGO_CONTROL);
    const bool bIsStaredViewer = StartProcess(::ELGO_SYS::Proc::ELGO_VIEWER);
    ELGO_MAIN_LOG("[elgo_main] StartProccess { contorl : %d, viewer : %d }", bIsStaredControl, bIsStaredViewer);
}

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    // IPC Event Listenner
    g_EventHandler = new MainEventHandler(ELGO_SYS::Proc::ELGO_MAIN);

    Initialize();

    return app.exec();
}
