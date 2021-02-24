#ifndef DBDEFINITION_H
#define DBDEFINITION_H

#include <QString>

// for test
static const QString DEVICE_DB = "C:/Project/Qt/device.db";

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
                                                    "uuid VARCHAR(30) NOT NULL,"
                                                    "user_id INTEGER NOT NULL,"
                                                    "deviceName VARCHAR(30));";

    const static QString INSERT_VALUES_DEVICE = "INSERT INTO device(device_id, user_id, deviceName)"
                                                    "VALUES(?, ?, ?);";

    const static QString SELECT_ALL_INFO_DEVICE = "SELECT * FROM device;";


    // account table
    const static QString CREATE_TABLE_ACCOUNT = "CREATE TABLE account ("
                                                    "type VARCHAR(30) NOT NULL,"
                                                    "id VARCHAR(30) NOT NULL,"
                                                    "pw VARCHAR(30) NOT NULL,"
                                                    "salt VARCHAR(30) NOT NULL);";

    const static QString INSERT_VALUES_ACCOUNT = "INSERT INTO account (type, id, pw, salt)"
                                                    "VALUES(?, ?, ?, ?);";

    const static QString SELECT_ALL_INFO_ACCOUNT = "SELECT * FROM account;";


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
