#ifndef DBDEFINITION_H
#define DBDEFINITION_H

#include <QString>

// for test
#if defined (Q_OS_LINUX) || defined (Q_OS_UNIX)
static const QString DEVICE_DB = "/home/jaehoon/Desktop/ELGO/device.db";
#elif defined (Q_OS_WIN32) || defined(Q_OS_WIN64) || defined(Q_OS_WINRT)
static const QString DEVICE_DB = "C:/Project/Qt/device.db";
#endif

namespace ClientDB
{
    enum DB_Table
    {
        ACCOUNT = 0,
        DEVICE = 1,
        DEVICE_SCHEDULE = 2,
        NETWORK = 3,
        OPTIONS = 4,
        MAX_DB_TABLE = 255
    };

    struct DEVICE
    {
        int deviceID;
        int userID;
        QString deviceName; // varchar(30)
    };
};

namespace DB_Query
{
    // device table
    const static QString CREATE_TABLE_DEVICE = "CREATE TABLE device ( "
                                                    "id VARCHAR(30) DEFAULT 'root' NOT NULL,"
                                                    "pw INTEGER DEFAULT 'root' NOT NULL,"
                                                    "name VARCHAR(30) DEFAULT 'ELGO');";

    const static QString INSERT_VALUES_DEVICE = "INSERT INTO device(id, pw, name)"
                                                    "VALUES(:id, :pw, :name);";

    const static QString SELECT_ALL_INFO_DEVICE = "SELECT * FROM device;";

    const static QString SELECT_DEVICE_NAME = "SELECT name FROM device;";

    const static QString UPDATE_PW_INFO_DEVICE = "UPDATE device SET pw=:pw;";

    const static QString UPDATE_DEVICE_NAME = "UPDATE device SET name=:name;";


    // account table - NOT USING
    /*
    const static QString CREATE_TABLE_ACCOUNT = "CREATE TABLE account ("
                                                    "type VARCHAR(30) NOT NULL,"
                                                    "id VARCHAR(30) NOT NULL,"
                                                    "pw VARCHAR(30) NOT NULL,"
                                                    "salt VARCHAR(30) NOT NULL);";

    const static QString INSERT_VALUES_ACCOUNT = "INSERT INTO account (type, id, pw, salt)"
                                                    "VALUES(?, ?, ?, ?);";

    const static QString SELECT_ALL_INFO_ACCOUNT = "SELECT * FROM account;";
    */


    // schedule table
    const static QString CREATE_TABLE_SCHEDULE = "CREATE TABLE schedule ("
                                                    "id VARCHAR(30) NOT NULL,"
                                                    "expiration_date DATETIME NOT NULL,"
                                                    "rule INTEGER NOT NULL,"
                                                    "command VARCHAR(30) NOT NULL);";

    const static QString SELECT_ALL_SCHEDULE = "SELECT * FROM schedule;";


    // network table
    const static QString CREATE_NETWORK_TABLE = "CREATE TABLE network ("
                                                    "ssid VARCHAR(30) NOT NULL,"
                                                    "pw VARCHAR(30) NOT NULL,"
                                                    "isEncryption BOOLEAN DEFAULT 0,"
                                                    "priority INTEGER DEFAULT 2);";

    const static QString SELECT_ALL_NETWORK_TABLE = "SELECT * FROM network;";
};

#endif // DBDEFINITION_H
