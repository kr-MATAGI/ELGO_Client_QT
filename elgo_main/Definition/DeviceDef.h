#ifndef DEFINITION_H
#define DEFINITION_H

// QT
#include <QString>

#define INIT_CONFIG_FILE_PATH "/opt/ELGO/config.xml"

namespace DEVICE
{
    /** @brief */
    enum Architec
    {
        UNKOWN_ARCH = 0,
        ARM = 1,
        ARM64 = 2,
        I386 = 3,
        IA64 = 4,
        MIPS = 5,
        MIPS64= 6,
        POWER = 7,
        POWER64 = 8,
        SPARC = 9,
        SPARCV9 = 10,
        X86_64 = 11,
        ANOTHER_ARCH = 255
    };
    static const QString Arch_enum2str[] = { "unkown", "arm", "arm64",
                                           "i386", "ia64", "mips", "mips64",
                                           "power", "power64", "sparc", "sparcv9",
                                           "x86_64", "another"};
    DEVICE::Architec ArchitecString2Enum(QString &str);

    /** @brief */
    enum OS
    {
        UNKOWN_OS = 0,
        WINDOWS = 1,
        WINRT = 2,
        LINUX = 3,
        ANDROID = 4,
        MAC = 5,
        DEBIAN = 6,
        IOS = 7,
        TV_OS = 8,
        WATCH_OS = 9,
        DARWIN = 10,
        UBUNTU = 11,
        ANOTHER_OS = 255
    };
    static const QString OS_enum2str[] = { "UNKOWN", "Windows", "WinRT", "Linux",
                                           "Android", "Mac", "debian",
                                           "ios", "tvos", "watchos",
                                           "DarWin", "Ubuntu", "another" };
    DEVICE::OS OSString2Enum(QString &str);

    /** @brief */
    struct Stroage
    {
        int totalStorage; // Mbyte
        int freeStorage; // Mbyte
    };

    /** @brief */
    struct IPADDR
    {
        QString ip;
        QString mac;
    };

    /** @brief */
    struct Info
    {
        Architec architec;
        OS os;
        QString hostName;

        IPADDR ipAddr;
        Stroage storage;
    }; 

    /** @brief */
    struct InternetConfig
    {
        QString ssid;
        QString pw;
    };

    /** @brief */
    struct ServerConfig
    {
        QString wasHost;
        quint16 wasHostPort;

        QString remoteTCPHost;
        quint16 remoteTCPPort;;
    };

    /** @brief */
    struct INIT_CONFIG
    {
        QString version;
        QString binaryPath;
        InternetConfig internet;
        ServerConfig server;
    };
}

#endif // DEFINITION_H
