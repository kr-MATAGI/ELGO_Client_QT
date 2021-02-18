#include <QCoreApplication>
#include <QObject>
#include <QProcess>

#include "Definition/DeviceDef.h"
#include "MainCtrl/mainctrl.h"

static MainCtrl *g_MainCtrl = NULL;

void testSlot()
{
    qDebug("started");
}

bool StartProcess(::ELGO_PROC::Proc proc)
{
    bool retValue = false;

    QProcess *process = new QProcess;
    QStringList args;
    QString program = g_MainCtrl->MakeProcessPath(proc);
    qDebug("%s", program.toUtf8().constData());
    process->start(program, args);
    retValue = process->waitForStarted(3000);

    return retValue;
}

bool Initialize()
{
    bool retValue = false;

    // Get This Device Info
    // to do : save ip to shared mem, checking network connection
    g_MainCtrl->LoadCurrentDeviceInfo();
    ::DEVICE::Info deviceInfo = g_MainCtrl->GetDeviceInfo();
    qDebug("OS : %s, Arch : %s, name : %s, ip : %s, mac : %s, netMask : %s",
           ::DEVICE::OS_enum2str[deviceInfo.os], ::DEVICE::Arch_enum2str[deviceInfo.architec],
            deviceInfo.hostName.toUtf8().constData(), deviceInfo.ip.toUtf8().constData(),
            deviceInfo.mac.toUtf8().constData(), deviceInfo.netMask.toUtf8().constData());


    // Start Process
    // TODO : except code about 'false' result
    const bool bIsStaredControl = StartProcess(::ELGO_PROC::Proc::ELGO_CONTROL);
    const bool bIsStaredViewer = StartProcess(::ELGO_PROC::Proc::ELGO_VIEWER);
    qDebug("%d %d", bIsStaredControl, bIsStaredViewer);

    return retValue;
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    g_MainCtrl = new MainCtrl();

    Initialize();


    return a.exec();
}
