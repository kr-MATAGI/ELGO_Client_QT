#ifndef DBDEFINITION_H
#define DBDEFINITION_H

#include <QString>

// for test
#if defined (Q_OS_LINUX) || defined (Q_OS_UNIX)
static const char* DEVICE_DB = "/home/jaehoon/바탕화면/ELGO/device.db";
static const char* SCHEDULE_DB = "/home/jaehoon/바탕화면/ELGO/schedule.db";
#elif defined (Q_OS_WIN32) || defined(Q_OS_WIN64) || defined(Q_OS_WINRT)
static const char* DEVICE_DB = "C:/Project/Qt/device.db";
static const char* SCHEDULE_DB = "C:/Project/Qt/schedule.db";
#endif

#define ZERO_RESULT 0

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
    const static char* CHECK_DEVICE_TABLE = "SELECT COUNT(*) AS COUNT FROM sqlite_master "
                                            "WHERE name=:table;";

    /// device.db
    // device table
    const static char* CREATE_TABLE_DEVICE = "CREATE TABLE device ( "
                                                    "id VARCHAR(30) DEFAULT 'root' NOT NULL,"
                                                    "pw INTEGER DEFAULT 'root' NOT NULL,"
                                                    "name VARCHAR(30) DEFAULT 'ELGO');";

    const static char* INSERT_VALUES_DEVICE = "INSERT INTO device(id, pw, name)"
                                                    "VALUES(:id, :pw, :name);";

    const static char* SELECT_ALL_INFO_DEVICE = "SELECT * FROM device;";

    const static char* SELECT_DEVICE_NAME = "SELECT name FROM device;";

    const static char* UPDATE_PW_INFO_DEVICE = "UPDATE device SET pw=:pw;";

    const static char* UPDATE_DEVICE_NAME = "UPDATE device SET name=:name;";


    /// schedule.db
    // playSchedule table
    const static char* CREATE_TABLE_PLAY_SCHEDULE = "CREATE TABLE playSchedule ("
                                                        "id VARCHAR(30) NOT NULL,"
                                                        "startDate TEXT NOT NULL,"
                                                        "endDate TEXT NOT NULL,"
                                                        "playDataId VARCHAR(30) NOT NULL,"
                                                        "playDataType INTEGER NOT NULL,"
                                                        "cron BLOB NOT NULL);";

    const static char* SELECT_ALL_PLAY_SCHEDULE = "SELECT * FROM playSchedule;";

    const static char* SELECT_ID_PLAY_SCHEDULE = "SELECT id FROM playSchedule;";

    const static char* INSERT_PLAY_SCHEDULE = "INSERT INTO playSchedule(id, startDate, endDate, playDataId, playDataType, cron) "
                                                    "VALUES(:id, :startDate, :endDate, :playDataId, :playDataType, :cron);";

};

#endif // DBDEFINITION_H
