#ifndef DBDEFINITION_H
#define DBDEFINITION_H

#include <QString>

static const QString DEVICE_DB_PATH = "C:/Project/Qt/device.db";

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
                                                "device_id NUMBER NOT NULL,"
                                                "user_id NUMBER NOT NULL,"
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
                                                    "expirDate VARCHAR(30) NOT NULL,"
                                                    "rule NUMBER NOT NULL,"
                                                    "command VARCHAR(30) NOT NULL);";
};

#endif // DBDEFINITION_H
