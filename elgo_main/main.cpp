// QT
#include <QCoreApplication>
#include <QProcess>

// Main
#include "MainCtrl/MainController.h"
#include "Definition/DeviceDef.h"
#include "MainCtrl/MainCtrl.h"
#include "DB/MainDBCtrl.h"
#include "Logger/MainLogger.h"
#include "Utils/DeviceManager.h"

// EFC
#include "Logger/ELogger.h"
#include "Event/MainEventHandler.h"
#include "LocalSocketEvent/EFCEvent.h"

static MainController *g_MainController = MainController::GetInstance();
static MainEventHandler *g_EventHandler = NULL;

void Initialize()
{
    // Get This Device Info
    g_MainController->GetInstance()->GetMainCtrl().LoadCurrentDeviceInfo();
    ::DEVICE::Info deviceInfo = g_MainController->GetMainCtrl().GetDeviceInfo();
    ELGO_MAIN_LOG("OS : %s, Arch : %s, name : %s, ip : %s, mac : %s",
                ::DEVICE::OS_enum2str[deviceInfo.os].toStdString().c_str(),
                ::DEVICE::Arch_enum2str[deviceInfo.architec].toStdString().c_str(),
                deviceInfo.hostName.toUtf8().constData(),
                deviceInfo.ipAddr.ip.toUtf8().constData(),
                deviceInfo.ipAddr.mac.toUtf8().constData());

    // Check Resource Folder
    g_MainController->GetInstance()->GetMainCtrl().CheckDefaultFolder();

    // Get DB info
    g_MainController->GetInstance()->GetDBCtrl().InitializeDB();

    // Load Configuration
    g_MainController->GetInstance()->GetMainCtrl().LoadConfigurationInfo();

    // Check Internet Connection;
    g_MainController->GetInstance()->GetMainCtrl().CheckInternetConnection();

    // Check Display Sleep Status
    g_MainController->GetInstance()->GetMainCtrl().CheckDisplaySleepStatus();

    // Start Process
    const QString& viewerPath = g_MainController->GetMainCtrl().MakeProcessPath(::ELGO_SYS::Proc::ELGO_VIEWER);
    const bool bIsStartedViewer = DeviceManager::StartProcess(viewerPath);
    ELGO_MAIN_LOG("StartProccess - { viewer: %d }", bIsStartedViewer);
}

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    // IPC Event Listenner
    g_EventHandler = new MainEventHandler(ELGO_SYS::Proc::ELGO_MAIN);

    Initialize();

    return app.exec();
}
