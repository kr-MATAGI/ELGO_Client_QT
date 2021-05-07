// QT
#include <QSqlQuery>

// Common
#include "Common/Interface/ScheduleImpl.h"
#include "Common/Interface/ContentsPlayDataImpl.h"

// Main
#include "MainDBCtrl.h"
#include "Logger/MainLogger.h"

//========================================================
MainDBCtrl::MainDBCtrl(QObject *parent)
    : QObject(parent)
//========================================================
{
    m_mutex = new QMutex;
}

//========================================================
MainDBCtrl::~MainDBCtrl()
//========================================================
{
    delete m_mutex;
    m_mutex = NULL;
}

//========================================================
void MainDBCtrl::InitializeDB()
//========================================================
{
    m_mutex->lock();

    // Checking device.db's tables
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");

    MakeDefulatTable(db, DEVICE_DB, "device");
    MakeDefulatTable(db, SCHEDULE_DB, "playSchedule");
    MakeDefulatTable(db, SCHEDULE_DB, "playData");
    MakeDefulatTable(db, SCHEDULE_DB, "powerSchedule");
    MakeDefulatTable(db, SCHEDULE_DB, "playingInfo");

    m_mutex->unlock();
}

//========================================================
void MainDBCtrl::MakeDefulatTable(QSqlDatabase& db, const char* dbPath,
                                  const QString& tableName)
//========================================================
{
    db.setDatabaseName(dbPath);
    const bool bIsOpened = db.open();
    if(false == bIsOpened)
    {
        ELGO_MAIN_LOG("ERROR - DB Open: %s", dbPath);
        db.close();

        return;
    }

    QSqlQuery query(db);
    query.prepare(DB_Query::CHECK_DEVICE_TABLE);
    query.bindValue(":table", tableName);

    const bool bIsChecked = query.exec();
    if(true == bIsChecked)
    {
        int countIdx = 0;
        int countValue = 0;
        while(query.next())
        {
            countIdx = query.record().indexOf("COUNT");
            countValue = query.value(countIdx).toInt();
            ELGO_MAIN_LOG("%s table - count: {idx: %d, value: %d}",
                          tableName.toStdString().c_str(),
                          countIdx, countValue);
        }

        if(ZERO_RESULT == countValue)
        {
            query.clear();
            if(0 == strcmp("device", tableName.toStdString().c_str()))
            {
                query.prepare(DB_Query::CREATE_TABLE_DEVICE);
                const bool bIsCreated = query.exec();
                if(true == bIsCreated)
                {
                    // id: root, pw : root, name : elgo
                    const int randMax = 1000000;
                    const int randMin = 1;
                    int randomValue = (double)std::rand() / RAND_MAX * (randMax - randMin) + randMin;
                    query.prepare(DB_Query::INSERT_VALUES_DEVICE);

                    query.bindValue(":id", "root");
                    query.bindValue(":pw", "root");
                    query.bindValue(":name", QString("elgo_d_" + QString::number(randomValue)));
                    const bool bIsInserted = query.exec();
                    if(false == bIsInserted)
                    {
                        ELGO_MAIN_LOG("ERROR - Failed query.exec(): %s{%s}",
                                      DB_Query::INSERT_VALUES_DEVICE,
                                      query.lastError().text().toStdString().c_str());
                    }
                }
                else
                {
                    ELGO_MAIN_LOG("ERROR - Failed query.exec(): %s{%s}",
                                  DB_Query::CREATE_TABLE_DEVICE,
                                  query.lastError().text().toStdString().c_str());
                }
            }
            else
            {
                QString prepareQuery;
                if(0 == strcmp("playSchedule", tableName.toStdString().c_str()))
                {
                    prepareQuery = DB_Query::CREATE_TABLE_PLAY_SCHEDULE;
                }
                else if(0 == strcmp("powerSchedule", tableName.toStdString().c_str()))
                {
                    prepareQuery = DB_Query::CREATE_TABLE_POWER_SCHEDULE;
                }
                else if(0 == strcmp("playData", tableName.toStdString().c_str()))
                {
                    prepareQuery = DB_Query::CREATE_TABLE_PLAY_DATA;
                }
                else if(0 == strcmp("playingInfo", tableName.toStdString().c_str()))
                {
                    prepareQuery = DB_Query::CREATE_TABLE_PLAYING_INFO;
                }
                query.prepare(prepareQuery);
                const bool bIsCreated = query.exec();
                if(false == bIsCreated)
                {
                    ELGO_MAIN_LOG("ERROR - Failed query.exec(): %s{%s}",
                                  prepareQuery.toStdString().c_str(),
                                  query.lastError().text().toStdString().c_str());
                }
            }
        }
    }
    else
    {
        ELGO_MAIN_LOG("ERROR - Failed query.exec(): %s", DB_Query::CHECK_DEVICE_TABLE);
    }

    db.close();
}

//========================================================
void MainDBCtrl::UpdateNewPlaySchedule(const QVector<ScheduleJson::PlaySchedule>& playScheduleList)
//========================================================
{
    m_mutex->lock();

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(SCHEDULE_DB);
    const bool bIsOpened = db.open();
    if(false == bIsOpened)
    {
        ELGO_MAIN_LOG("ERROR - DB Open: %s", SCHEDULE_DB);
        db.close();
        m_mutex->unlock();

        return;
    }

    // Insert or Replace data to DB
    QSqlQuery query(db);
    QVector<ScheduleJson::PlaySchedule>::const_iterator scheduleIter = playScheduleList.constBegin();
    while(scheduleIter != playScheduleList.constEnd())
    {
        const QString& id = (*scheduleIter).id;

        QVector<ScheduleJson::PlayScheduleData>::const_iterator dataIter = (*scheduleIter).scheduleList.constBegin();
        while(dataIter != (*scheduleIter).scheduleList.constEnd())
        {
            query.prepare(DB_Query::INSERT_PLAY_SCHEDULE);
            query.bindValue(":id", id);
            query.bindValue(":startDate", (*dataIter).startTime);
            query.bindValue(":endDate", (*dataIter).endTime);
            query.bindValue(":playDataId", (*dataIter).playDataId);
            query.bindValue(":playDataType", (*dataIter).type);

            QByteArray bytes;
            QDataStream dataStream(&bytes, QIODevice::WriteOnly);
            dataStream << (*dataIter).cron;
            query.bindValue(":cron", bytes);
            ELGO_MAIN_LOG("INSERT - {id: %s, startDate: %s, endDate: %s, playData{id: %d, type: %d}",
                          id.toStdString().c_str(), (*dataIter).startTime.toString().toStdString().c_str(),
                          (*dataIter).endTime.toString().toStdString().c_str(),
                          (*dataIter).playDataId, (*dataIter).type);

            const bool bInserted = query.exec();
            if(false == bInserted)
            {
                ELGO_MAIN_LOG("ERROR - Failed query.exec(): %s{%s}",
                              DB_Query::INSERT_PLAY_SCHEDULE,
                              query.lastError().text().toStdString().c_str());
            }

            ++dataIter;
        }
        ++scheduleIter;
    }

    db.close();
    m_mutex->unlock();
}

//========================================================
void MainDBCtrl::ClearAllPlaySchedule()
//========================================================
{
    m_mutex->lock();

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(SCHEDULE_DB);
    const bool bIsOpened = db.open();
    if(false == bIsOpened)
    {
        ELGO_MAIN_LOG("ERROR - DB Open: %s", SCHEDULE_DB);
        db.close();
        m_mutex->unlock();

        return;
    }

    QSqlQuery query(db);
    query.prepare(DB_Query::DELETE_ALL_PLAY_SCHEDULE);
    const bool bIsDel = query.exec();
    if(false == bIsDel)
    {
        ELGO_MAIN_LOG("ERROR - Failed query.exec(): %s{%s}",
                      DB_Query::DELETE_ALL_PLAY_SCHEDULE,
                      query.lastError().text().toStdString().c_str());
    }
    db.close();

    m_mutex->unlock();
}

//========================================================
void MainDBCtrl::DeletePlayScheduleById(const QString& scheduleId)
//========================================================
{
    m_mutex->lock();

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(SCHEDULE_DB);
    const bool bIsOpened = db.open();
    if(false == bIsOpened)
    {
        ELGO_MAIN_LOG("ERROR - DB Open: %s", SCHEDULE_DB);
        db.close();
        m_mutex->unlock();

        return;
    }

    QSqlQuery query(db);
    query.prepare(DB_Query::DELETE_PLAY_SCHEDULE_BY_ID);
    query.bindValue(":id", scheduleId);
    const bool bIsDel = query.exec();
    if(false == bIsDel)
    {
        ELGO_MAIN_LOG("ERROR - Failed query.exec(): %s{%s}",
                      DB_Query::DELETE_PLAY_SCHEDULE_BY_ID,
                      query.lastError().text().toStdString().c_str());
    }

    db.close();
    m_mutex->unlock();
}

//========================================================
void MainDBCtrl::GetAllPlayScheduleList(QVector<ScheduleJson::PlaySchedule>& dest)
//========================================================
{
    m_mutex->lock();

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(SCHEDULE_DB);
    const bool bIsOpened = db.open();
    if(false == bIsOpened)
    {
        ELGO_MAIN_LOG("ERROR - DB Open: %s", SCHEDULE_DB);
        db.close();
        m_mutex->unlock();

        return;
    }

    QSqlQuery query(db);
    query.prepare(DB_Query::SELECT_ALL_PLAY_SCHEDULE);
    const bool bIsSelected = query.exec();
    if(true == bIsSelected)
    {
        while(query.next())
        {
            ScheduleJson::PlayScheduleData scheduleData;

            const int idIdx = query.record().indexOf("id");
            const QString idValue = query.value(idIdx).toString();

            const int startDateIdx = query.record().indexOf("startDate");
            scheduleData.startTime = query.value(startDateIdx).toDateTime();

            const int endDateIdx = query.record().indexOf("endDate");
            scheduleData.endTime = query.value(endDateIdx).toDateTime();

            const int playDataIdIdx = query.record().indexOf("playDataId");
            scheduleData.playDataId = query.value(playDataIdIdx).toInt();

            const int playDataTypeIdx = query.record().indexOf("playDataType");
            scheduleData.type = static_cast<PlayJson::PlayDataType>(query.value(playDataTypeIdx).toInt());

            const int cronIdx = query.record().indexOf("cron");
            QByteArray cronBytes = query.value(cronIdx).toByteArray();
            QDataStream cronStream(&cronBytes, QIODevice::ReadOnly);
            cronStream >> scheduleData.cron;

            QVector<ScheduleJson::PlaySchedule>::iterator iter = dest.begin();
            bool bIsExisted = false;
            while(iter != dest.end())
            {
                if(0 == strcmp(iter->id.toStdString().c_str(), idValue.toStdString().c_str()))
                {
                    bIsExisted = true;

                    iter->scheduleList.push_back(scheduleData);
                    ELGO_MAIN_LOG("PUSH - { id: %s, playData{id: %d, type: %d}, start: %s, end: %s }",
                                  idValue.toStdString().c_str(), scheduleData.playDataId, scheduleData.type,
                                  scheduleData.startTime.toString().toStdString().c_str(),
                                  scheduleData.endTime.toString().toStdString().c_str());

                    break;
                }

                ++iter;
            }

            if(false == bIsExisted)
            {
                ScheduleJson::PlaySchedule playSchedule;
                playSchedule.id = idValue;
                playSchedule.scheduleList.push_back(scheduleData);
                ELGO_MAIN_LOG("ADD - { id: %s, playData{id: %d, type: %d}, start: %s, end: %s }",
                              idValue.toStdString().c_str(), scheduleData.playDataId, scheduleData.type,
                              scheduleData.startTime.toString().toStdString().c_str(),
                              scheduleData.endTime.toString().toStdString().c_str());

                dest.push_back(playSchedule);
            }
        }
    }
    else
    {
        ELGO_MAIN_LOG("ERROR - Failed query.exec(): %s{%s}",
                      DB_Query::SELECT_ALL_PLAY_SCHEDULE,
                      query.lastError().text().toStdString().c_str());
    }

    db.close();
    m_mutex->unlock();
}

//========================================================
void MainDBCtrl::UpdateNewPowerSchedule(const QVector<ScheduleJson::PowerScheduleData>& src)
//========================================================
{
    m_mutex->lock();

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(SCHEDULE_DB);
    const bool bIsOpened = db.open();
    if(false == bIsOpened)
    {
        ELGO_MAIN_LOG("ERROR - DB Open: %s", SCHEDULE_DB);
        db.close();
        m_mutex->unlock();

        return;
    }

    // Insert or Replace data to DB
    QSqlQuery query(db);
    QVector<ScheduleJson::PowerScheduleData>::const_iterator scheduleIter = src.constBegin();
    while(scheduleIter != src.constEnd())
    {
        query.clear();
        query.prepare(DB_Query::INSERT_POWER_SCHEDULE);
        query.bindValue(":id", (*scheduleIter).id);

        const bool bIsOn = (ScheduleJson::PowerStatus::POWER_ON == (*scheduleIter).status) ? true : false;
        query.bindValue(":isOn", bIsOn);

        query.bindValue(":startDate", (*scheduleIter).startTime);
        query.bindValue(":endDate", (*scheduleIter).endTime);

        QByteArray cronBytes;
        QDataStream cronStream(&cronBytes, QIODevice::WriteOnly);
        cronStream << (*scheduleIter).cron;
        query.bindValue(":cron", cronBytes);
        ELGO_MAIN_LOG("INSERT - {id: %s, startDate: %s, endDate: %s",
                      (*scheduleIter).id.toStdString().c_str(),
                      (*scheduleIter).startTime.toString().toStdString().c_str(),
                      (*scheduleIter).endTime.toString().toStdString().c_str());

        const bool bInserted = query.exec();
        if(false == bInserted)
        {
            ELGO_MAIN_LOG("ERROR - Failed query.exec(): %s{%s}",
                          DB_Query::INSERT_POWER_SCHEDULE,
                          query.lastError().text().toStdString().c_str());
        }

        ++scheduleIter;
    }

    db.close();
    m_mutex->unlock();
}

//========================================================
void MainDBCtrl::DeletePowerScheduleById(const QString& scheduleId)
//========================================================
{
    m_mutex->lock();

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(SCHEDULE_DB);
    const bool bIsOpened = db.open();
    if(false == bIsOpened)
    {
        ELGO_MAIN_LOG("ERROR - DB Open: %s", SCHEDULE_DB);
        db.close();
        m_mutex->unlock();

        return;
    }

    QSqlQuery query(db);
    query.prepare(DB_Query::DELETE_POWER_SCHEDULE_BY_ID);
    query.bindValue(":id", scheduleId);
    const bool bIsDel = query.exec();
    if(false == bIsDel)
    {
        ELGO_MAIN_LOG("ERROR - Failed query.exec(): %s{%s}",
                      DB_Query::DELETE_POWER_SCHEDULE_BY_ID,
                      query.lastError().text().toStdString().c_str());
    }

    db.close();
    m_mutex->unlock();
}

//========================================================
void MainDBCtrl::GetAllPowerScheduleList(ScheduleJson::PowerSchedule& dest)
//========================================================
{
    m_mutex->lock();

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(SCHEDULE_DB);
    const bool bIsOpened = db.open();
    if(false == bIsOpened)
    {
        ELGO_MAIN_LOG("ERROR - DB Open: %s", SCHEDULE_DB);
        db.close();
        m_mutex->unlock();

        return;
    }

    QSqlQuery query(db);
    query.prepare(DB_Query::SELECT_ALL_POWER_SCHEDULE);
    const bool bIsSelected = query.exec();
    if(true == bIsSelected)
    {
        while(query.next())
        {
            ScheduleJson::PowerScheduleData scheduleData;

            const int idIdx = query.record().indexOf("id");
            scheduleData.id = query.value(idIdx).toString();

            const int isOnIdx = query.record().indexOf("isOn");
            const bool bIsOn = query.value(isOnIdx).toBool();
            scheduleData.status = (true == bIsOn) ? ScheduleJson::PowerStatus::POWER_ON
                                                  : ScheduleJson::PowerStatus::POWER_OFF;

            const int startDateIdx = query.record().indexOf("startDate");
            scheduleData.startTime = query.value(startDateIdx).toDateTime();

            const int endDateIdx = query.record().indexOf("endDate");
            scheduleData.endTime = query.value(endDateIdx).toDateTime();

            const int cronIdx = query.record().indexOf("cron");
            QByteArray cronBytes = query.value(cronIdx).toByteArray();
            QDataStream cronStream(&cronBytes, QIODevice::ReadOnly);
            cronStream >> scheduleData.cron;

            dest.scheduleList.push_back(scheduleData);
        }
    }
    else
    {
        ELGO_MAIN_LOG("ERROR - Failed query.exec(): %s{%s}",
                      DB_Query::SELECT_ALL_POWER_SCHEDULE,
                      query.lastError().text().toStdString().c_str());
    }

    db.close();
    m_mutex->unlock();
}

//========================================================
void MainDBCtrl::UpdatePlayingData(const int playDataId, const PlayJson::PlayDataType type)
//========================================================
{
    m_mutex->lock();

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(SCHEDULE_DB);
    const bool bIsOpened = db.open();
    if(false == bIsOpened)
    {
        ELGO_MAIN_LOG("ERROR - DB Open: %s", SCHEDULE_DB);
        db.close();
        m_mutex->unlock();

        return;
    }

    QSqlQuery query(db);
    query.prepare(DB_Query::DELETE_PLAYING_DATA);
    const bool bIsDel = query.exec();
    if(true == bIsDel)
    {
        query.clear();
        query.prepare(DB_Query::INSERT_PLAYING_DATA);
        query.bindValue(":id", playDataId);
        query.bindValue(":type", type);

        const bool bIsInserted = query.exec();
        if(true == bIsInserted)
        {
            ELGO_MAIN_LOG("Inserted - {id: %d, type: %d}",
                          playDataId, type);
        }
        else
        {
            ELGO_MAIN_LOG("ERROR - Failed query.exec(): %s{%s}",
                          DB_Query::INSERT_PLAYING_DATA,
                          query.lastError().text().toStdString().c_str());
        }
    }
    else
    {
        ELGO_MAIN_LOG("ERROR - Failed query.exec(): %s{%s}",
                      DB_Query::DELETE_PLAYING_DATA,
                      query.lastError().text().toStdString().c_str());
    }

    db.close();
    m_mutex->unlock();
}

//========================================================
void MainDBCtrl::GetPlayingData(int& destId, PlayJson::PlayDataType& destType)
//========================================================
{
    m_mutex->lock();

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(SCHEDULE_DB);
    const bool bIsOpened = db.open();
    if(false == bIsOpened)
    {
        ELGO_MAIN_LOG("ERROR - DB Open: %s", SCHEDULE_DB);
        db.close();
        m_mutex->unlock();

        return;
    }

    QSqlQuery query(db);
    query.prepare(DB_Query::SELECT_ALL_PLAYING_DATA);
    const bool bIsSelected = query.exec();
    if(true == bIsSelected)
    {
        while(query.next())
        {
            const int idIdx = query.record().indexOf("id");
            const int typeIdx = query.record().indexOf("type");

            destId = query.value(idIdx).toInt();
            destType = static_cast<PlayJson::PlayDataType>(query.value(typeIdx).toInt());
        }
        ELGO_MAIN_LOG("playing Data - {id: %d, type: %d}", destId, destType);
    }
    else
    {
        ELGO_MAIN_LOG("ERROR - Failed query.exec(): %s{%s}",
                      DB_Query::SELECT_ALL_PLAYING_DATA,
                      query.lastError().text().toStdString().c_str());
    }

    db.close();
    m_mutex->unlock();
}

//========================================================
void MainDBCtrl::AddNewPlayDataToDB(const PlayJson::CustomPlayDataJson& playData)
//========================================================
{
    m_mutex->lock();

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(SCHEDULE_DB);
    const bool bIsOpened = db.open();
    if(false == bIsOpened)
    {
        ELGO_MAIN_LOG("ERROR - DB Open: %s", SCHEDULE_DB);
        db.close();
        m_mutex->unlock();

        return;
    }

    QSqlQuery query(db);
    query.prepare(DB_Query::INSERT_PLAY_DATA);
    query.bindValue(":id", playData.playData.id);
    query.bindValue(":type", playData.playData.playDataType);

    QByteArray bytes;
    QDataStream dataStream(&bytes, QIODevice::WriteOnly);
    dataStream << playData;
    query.bindValue(":data", bytes);
    const bool bIsExec = query.exec();
    if(true == bIsExec)
    {
        ELGO_MAIN_LOG("INSERT - PlayData {id: %d, type: %d}",
                      playData.playData.id, playData.playData.playDataType);
    }
    else
    {
        ELGO_MAIN_LOG("ERROR - INSERT PlayData {id: %d, type: %d}, %s",
                      playData.playData.id, playData.playData.playDataType,
                      query.lastError().text().toStdString().c_str());
    }

    db.close();
    m_mutex->unlock();
}

//========================================================
void MainDBCtrl::AddNewPlayDataToDB(const PlayJson::FixedPlayDataJson& playData)
//========================================================
{
    m_mutex->lock();

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(SCHEDULE_DB);
    const bool bIsOpened = db.open();
    if(false == bIsOpened)
    {
        ELGO_MAIN_LOG("ERROR - DB Open: %s", SCHEDULE_DB);
        db.close();
        m_mutex->unlock();

        return;
    }

    QSqlQuery query(db);
    query.prepare(DB_Query::INSERT_PLAY_DATA);
    query.bindValue(":id", playData.playData.id);
    query.bindValue(":type", playData.playData.playDataType);

    QByteArray bytes;
    QDataStream dataStream(&bytes, QIODevice::WriteOnly);
    dataStream << playData;
    query.bindValue(":data", bytes);
    const bool bIsExec = query.exec();
    if(true == bIsExec)
    {
        ELGO_MAIN_LOG("INSERT - {id: %d, type: %d}",
                      playData.playData.id, playData.playData.playDataType);
    }
    else
    {
        ELGO_MAIN_LOG("ERROR - INSERT PlayData {id: %d, type: %d}, %s",
                      playData.playData.id, playData.playData.playDataType,
                      query.lastError().text().toStdString().c_str());
    }

    db.close();
    m_mutex->unlock();
}

//========================================================
void MainDBCtrl::GetPlayDataFromDB(const int id, const PlayJson::PlayDataType type,
                                   PlayJson::CustomPlayDataJson& dest)
//========================================================
{
    m_mutex->lock();

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(SCHEDULE_DB);
    const bool bIsOpened = db.open();
    if(false == bIsOpened)
    {
        ELGO_MAIN_LOG("ERROR - DB Open: %s", SCHEDULE_DB);
        db.close();
        m_mutex->unlock();

        return;
    }

    QSqlQuery query(db);
    query.prepare(DB_Query::SELECT_PLAY_DATA);
    query.bindValue(":id", id);
    query.bindValue(":type", (int)type);
    const bool bIsExec = query.exec();
    if(true == bIsExec)
    {
        while(query.next())
        {
            const int columIdx = query.record().indexOf("data");
            QByteArray columBytes = query.value(columIdx).toByteArray();
            QDataStream columStream(&columBytes, QIODevice::ReadOnly);
            columStream >> dest;
            ELGO_MAIN_LOG("SELECT Result - {id: %d, type: %d}",
                          dest.playData.id, dest.playData.playDataType);
        }
    }
    else
    {
        ELGO_MAIN_LOG("ERROR - Failed query.exec(): %s{%s}",
                      DB_Query::SELECT_PLAY_DATA,
                      query.lastError().text().toStdString().c_str());
    }

    db.close();
    m_mutex->unlock();
}

//========================================================
void MainDBCtrl::GetPlayDataFromDB(const int id, const PlayJson::PlayDataType type,
                                   PlayJson::FixedPlayDataJson& dest)
//========================================================
{
    m_mutex->lock();

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(SCHEDULE_DB);
    const bool bIsOpened = db.open();
    if(false == bIsOpened)
    {
        ELGO_MAIN_LOG("ERROR - DB Open: %s", SCHEDULE_DB);
        db.close();
        m_mutex->unlock();

        return;
    }

    QSqlQuery query(db);
    query.prepare(DB_Query::SELECT_PLAY_DATA);
    query.bindValue(":id", id);
    query.bindValue(":type", (int)type);
    const bool bIsExec = query.exec();
    if(true == bIsExec)
    {
        while(query.next())
        {
            const int columIdx = query.record().indexOf("data");
            QByteArray columBytes = query.value(columIdx).toByteArray();
            QDataStream columStream(&columBytes, QIODevice::ReadOnly);
            columStream >> dest;
            ELGO_MAIN_LOG("SELECT Result - {id: %d, type: %d}",
                          dest.playData.id, dest.playData.playDataType);
        }
    }
    else
    {
        ELGO_MAIN_LOG("ERROR - Failed query.exec(): %s{%s}",
                      DB_Query::SELECT_PLAY_DATA,
                      query.lastError().text().toStdString().c_str());
    }

    db.close();
    m_mutex->unlock();
}

//========================================================
void MainDBCtrl::GetAllPlayDataFromDB(QVector<PlayJson::CustomPlayDataJson>& customList,
                                      QVector<PlayJson::FixedPlayDataJson>& fixedList)
//========================================================
{
    m_mutex->lock();

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(SCHEDULE_DB);
    const bool bIsOpened = db.open();
    if(false == bIsOpened)
    {
        ELGO_MAIN_LOG("ERROR - DB Open: %s", SCHEDULE_DB);
        db.close();
        m_mutex->unlock();

        return;
    }

    QSqlQuery query(db);
    query.prepare(DB_Query::SELECT_ALL_PLAY_DATA);
    const bool bIsSelected = query.exec();
    if(true == bIsSelected)
    {
        while(query.next())
        {
            const int typeIdx = query.record().indexOf("type");
            const PlayJson::PlayDataType playType = static_cast<PlayJson::PlayDataType>(query.value(typeIdx).toInt());

            if(PlayJson::PlayDataType::CUSTOM == playType)
            {
                PlayJson::CustomPlayDataJson customData;

                const int dataIdx = query.record().indexOf("data");
                QByteArray dataBytes = query.value(dataIdx).toByteArray();
                QDataStream dataStream(&dataBytes, QIODevice::ReadOnly);
                dataStream >> customData;

                customList.push_back(customData);
            }
            else if(PlayJson::PlayDataType::FIXED == playType)
            {
                PlayJson::FixedPlayDataJson fixedData;

                const int dataIdx = query.record().indexOf("data");
                QByteArray dataBytes = query.value(dataIdx).toByteArray();
                QDataStream dataStream(&dataBytes, QIODevice::ReadOnly);
                dataStream >> fixedData;

                fixedList.push_back(fixedData);
            }
            else
            {
                ELGO_MAIN_LOG("ERROR - Unknown Type: %d", playType);
            }
        }
    }
    else
    {
        ELGO_MAIN_LOG("ERROR - Failed query.exec(): %s{%s}",
                      DB_Query::SELECT_ALL_PLAY_DATA,
                      query.lastError().text().toStdString().c_str());
    }

    db.close();
    m_mutex->unlock();
}

//========================================================
void MainDBCtrl::DeletePlayData(const int id, const PlayJson::PlayDataType type)
//========================================================
{
    m_mutex->lock();

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(SCHEDULE_DB);
    const bool bIsOpened = db.open();
    if(false == bIsOpened)
    {
        ELGO_MAIN_LOG("ERROR - DB Open: %s", SCHEDULE_DB);
        db.close();
        m_mutex->unlock();

        return;
    }

    QSqlQuery query(db);
    query.prepare(DB_Query::DELETE_PLAY_DATA);
    query.bindValue(":id", id);
    query.bindValue(":type", (int)type);
    const bool bIsDel = query.exec();
    if(false == bIsDel)
    {
        ELGO_MAIN_LOG("ERROR - Failed query.exec(): %s{%s}",
                      DB_Query::DELETE_PLAY_DATA,
                      query.lastError().text().toStdString().c_str());
    }
    else
    {
        ELGO_MAIN_LOG("Delete - id: %d, type: %d", id, type);
    }

    db.close();
    m_mutex->unlock();
}
