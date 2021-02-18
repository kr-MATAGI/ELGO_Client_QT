#include "DeviceDef.h"

//========================================================
DEVICE::Architec DEVICE::ArchitecString2Enum(QString &str)
//========================================================
{
    ::DEVICE::Architec retValue = ::DEVICE::Architec::UNKOWN_ARCH;

    if(0 == strcmp(str.toUtf8().constData(), "arm"))
        retValue = ::DEVICE::Architec::ARM;
    else if(0 == strcmp(str.toUtf8().constData(), "arm64"))
        retValue = ::DEVICE::Architec::ARM64;
    else if(0 == strcmp(str.toUtf8().constData(), "i386"))
        retValue = ::DEVICE::Architec::I386;
    else if(0 == strcmp(str.toUtf8().constData(), "ia64"))
        retValue = ::DEVICE::Architec::IA64;
    else if(0 == strcmp(str.toUtf8().constData(), "mips"))
        retValue = ::DEVICE::Architec::MIPS;
    else if(0 == strcmp(str.toUtf8().constData(), "mips64"))
        retValue = ::DEVICE::Architec::MIPS64;
    else if(0 == strcmp(str.toUtf8().constData(), "power"))
        retValue = ::DEVICE::Architec::POWER;
    else if(0 == strcmp(str.toUtf8().constData(), "power64"))
        retValue = ::DEVICE::Architec::POWER64;
    else if(0 == strcmp(str.toUtf8().constData(), "sparc"))
        retValue = ::DEVICE::Architec::SPARC;
    else if(0 == strcmp(str.toUtf8().constData(), "sparcv9"))
        retValue = ::DEVICE::Architec::SPARCV9;
    else if(0 == strcmp(str.toUtf8().constData(), "x86_64"))
        retValue = ::DEVICE::Architec::X86_64;
    else
    {
        qDebug("another architecture : %s", str.toUtf8().constData());
        retValue = ::DEVICE::Architec::ANOTHER_ARCH;
    }

    return retValue;
}

//========================================================
DEVICE::OS DEVICE::OSString2Enum(QString &str)
//========================================================
{
    ::DEVICE::OS retValue = ::DEVICE::OS::UNKOWN_OS;

    if(0 == strcmp(str.toUtf8().constData(), "android"))
        retValue = ::DEVICE::OS::ANDROID;
    else if(0 == strcmp(str.toUtf8().constData(), "linux"))
        retValue = ::DEVICE::OS::LINUX;
    else if( (0 == strcmp(str.toUtf8().constData(), "osx")) ||
            (0 == strcmp(str.toUtf8().constData(), "macos")) )
        retValue = ::DEVICE::OS::MAC;
    else if(0 == strcmp(str.toUtf8().constData(), "ios"))
        retValue = ::DEVICE::OS::IOS;
    else if(0 == strcmp(str.toUtf8().constData(), "tvos"))
        retValue = ::DEVICE::OS::TV_OS;
    else if(0 == strcmp(str.toUtf8().constData(), "watchos"))
        retValue = ::DEVICE::OS::WATCH_OS;
    else if(0 == strcmp(str.toUtf8().constData(), "darwin"))
        retValue = ::DEVICE::OS::DARWIN;
    else if(0 == strcmp(str.toUtf8().constData(), "debian"))
        retValue = ::DEVICE::OS::DEBIAN;
    else if(0 == strcmp(str.toUtf8().constData(), "winrt"))
        retValue = ::DEVICE::OS::WINRT;
    else if(0 == strcmp(str.toUtf8().constData(), "windows"))
        retValue = ::DEVICE::OS::WINDOWS;
    else
    {
        qDebug("Another os : %s", str.toUtf8().constData());
        retValue = ::DEVICE::OS::ANOTHER_OS;
    }

    return retValue;
}
