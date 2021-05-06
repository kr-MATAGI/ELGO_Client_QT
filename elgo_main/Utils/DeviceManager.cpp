// QT
#include <QProcess>

// Main
#include "DeviceManager.h"
#include "Logger/MainLogger.h"

//========================================================
void DeviceManager::UpdateSleepStatus(const DEVICE::OS os, const bool isSleep)
//========================================================
{
    QProcess *process = new QProcess;
    QString cmdStr;
    QStringList args;

    if( (DEVICE::OS::LINUX == os) || (DEVICE::OS::UBUNTU == os) )
    {
        cmdStr = "/usr/bin/xset";
        args << "-display";
        args << ":0.0";
        args << "dpms";
        args << "force";

        if(true == isSleep)
        {
            args << "off";
        }
        else
        {
            args << "on";
        }
    }
    else if( (DEVICE::OS::WINDOWS == os) || (DEVICE::OS::WINRT == os) )
    {

    }
    else if(DEVICE::OS::ANDROID == os)
    {

    }

    process->start(cmdStr, args);
    process->waitForFinished();
    ELGO_MAIN_LOG("cmdStr: %s, args: force: %s",
                  cmdStr.toStdString().c_str(), args.back().toStdString().c_str());

    process->deleteLater();
}

//========================================================
void DeviceManager::SystemReboot(const DEVICE::OS os)
//========================================================
{
    QProcess *process = new QProcess;
    QString cmdStr;
    QStringList args;

    if( (DEVICE::OS::LINUX == os) || (DEVICE::OS::UBUNTU == os) )
    {
        cmdStr = "/bin/sh";
        args << "-c";
        args << "reboot";
    }
    else if( (DEVICE::OS::WINDOWS == os) || (DEVICE::OS::WINRT == os) )
    {

    }
    else if(DEVICE::OS::ANDROID == os)
    {

    }
    ELGO_MAIN_LOG("cmdStr: %s, args: %s",
                  cmdStr.toStdString().c_str(), args.back().toStdString().c_str());

    process->start(cmdStr, args);
    process->waitForFinished();

    process->deleteLater();

}
