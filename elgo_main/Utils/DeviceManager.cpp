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
        args << "sleep 5 && reboot";
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

//========================================================
void DeviceManager::DeviceMute(const DEVICE::OS os, const bool bIsMute)
//========================================================
{
    QProcess *process = new QProcess;
    QString cmdStr;
    QStringList args;

    if( (DEVICE::OS::LINUX == os) || (DEVICE::OS::UBUNTU == os) )
    {
        cmdStr = "/bin/sh";
        args << "-c";
        if(true == bIsMute)
        {
            args << "amixer -q -D pulse sset Master off";
        }
        else
        {
            args << "amixer -q -D pulse sset Master on";
        }
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

//========================================================
void DeviceManager::RotateScreen(const DEVICE::OS os, quint8 heading)
//========================================================
{
    QProcess *process = new QProcess;
    QString cmdStr;
    QStringList args;

    if( (DEVICE::OS::LINUX == os) || (DEVICE::OS::UBUNTU == os) )
    {
        cmdStr = "/bin/sh";
        args << "-c";
        if(1 == heading)
        {
            args << "xrandr -o normal";
        }
        else if(2 == heading)
        {
            args << "xrandr -o right";
        }
        else if(3 == heading)
        {
            args << "xrandr -o inverted";
        }
        else if(4 == heading)
        {
            args << "xrandr -o left";
        }
        else
        {
            ELGO_MAIN_LOG("ERROR - Unknown Heading: %u", heading);
        }
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

//========================================================
bool DeviceManager::StartProcess(const QString& procPath)
//========================================================
{
    bool retValue = false;

    QProcess *newProcess = new QProcess;
    QString cmd = procPath;
    QStringList args;
    ELGO_MAIN_LOG("Process Path: %s", procPath.toStdString().c_str());
    retValue = newProcess->startDetached(cmd, args);
    newProcess->waitForStarted();
    newProcess->waitForFinished();

    newProcess->deleteLater();

    return retValue;
}
