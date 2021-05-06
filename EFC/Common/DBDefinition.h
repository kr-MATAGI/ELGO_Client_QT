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

namespace DB_Query
{
    static const char* CHECK_DEVICE_TABLE = "SELECT COUNT(*) AS COUNT FROM sqlite_master "
                                            "WHERE name=:table;";

    /// device.db
    // device table
    static const char* CREATE_TABLE_DEVICE = "CREATE TABLE device ( "
                                                    "id VARCHAR(30) DEFAULT 'root' NOT NULL,"
                                                    "pw INTEGER DEFAULT 'root' NOT NULL,"
                                                    "name VARCHAR(30) DEFAULT 'ELGO');";

    static const char* INSERT_VALUES_DEVICE = "INSERT INTO device(id, pw, name)"
                                                    "VALUES(:id, :pw, :name);";

    static const char* SELECT_ALL_INFO_DEVICE = "SELECT * FROM device;";

    static const char* SELECT_DEVICE_NAME = "SELECT name FROM device;";

    static const char* UPDATE_PW_INFO_DEVICE = "UPDATE device SET pw=:pw;";

    static const char* UPDATE_DEVICE_NAME = "UPDATE device SET name=:name;";


    /// schedule.db
    // playSchedule table
    static const char* CREATE_TABLE_PLAY_SCHEDULE = "CREATE TABLE playSchedule ("
                                                        "id VARCHAR(30) PRIMARY KEY NOT NULL,"
                                                        "startDate TEXT NOT NULL,"
                                                        "endDate TEXT NOT NULL,"
                                                        "playDataId VARCHAR(30) NOT NULL,"
                                                        "playDataType INTEGER NOT NULL,"
                                                        "cron BLOB NOT NULL);";

    static const char* SELECT_ALL_PLAY_SCHEDULE = "SELECT * FROM playSchedule;";

    static const char* SELECT_ID_PLAY_SCHEDULE = "SELECT id FROM playSchedule;";

    static const char* INSERT_PLAY_SCHEDULE = "INSERT OR REPLACE INTO playSchedule("
                                                    "id, startDate, endDate, playDataId, playDataType, cron) "
                                                    "VALUES("
                                                    ":id, :startDate, :endDate, :playDataId, :playDataType, :cron);";

    static const char* DELETE_ALL_PLAY_SCHEDULE = "DELETE FROM playSchedule;";

    static const char* DELETE_PLAY_SCHEDULE_BY_ID = "DELETE FROM playSchedule WHERE id=:id;";

    // playData table
    static const char* CREATE_TABLE_PLAY_DATA = "CREATE TABLE playData ("
                                                    "id VARCHAR(30) NOT NULL,"
                                                    "type INTEGER NOT NULL,"
                                                    "data BLOB NOT NULL,"
                                                    "PRIMARY KEY(id, type));";

    static const char* INSERT_PLAY_DATA = "INSERT OR REPLACE INTO playData(id, type, data) "
                                                "VALUES(:id, :type, :data);";

    static const char* SELECT_PLAY_DATA = "SELECT data FROM playData WHERE id=:id AND type=:type;";

    // powerSchedule table
    static const char* CREATE_TABLE_POWER_SCHEDULE = "CREATE TABLE powerSchedule ("
                                                        "id VARCHAR(30) NOT NULL,"
                                                        "isOn BOOLEAN NOT NULL,"
                                                        "startDate TEXT NOT NULL,"
                                                        "endDate TEXT NOT NULL,"
                                                        "cron BLOB NOT NULL,"
                                                        "PRIMARY KEY(id, isOn));";

    static const char* INSERT_POWER_SCHEDULE = "INSERT OR REPLACE INTO powerSchedule("
                                                    "id, isOn, startDate, endDate, cron) "
                                                    "VALUES("
                                                    ":id, :isOn, :startDate, :endDate, :cron);";

    static const char* DELETE_POWER_SCHEDULE_BY_ID = "DELETE FROM powerSchedule WHERE id=:id;";

};

#endif // DBDEFINITION_H
