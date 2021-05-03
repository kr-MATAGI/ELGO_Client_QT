// QT
#include <QSqlQuery>

// Common
#include "Common/Interface/ScheduleImpl.h"

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
void MainDBCtrl::ConnectionDB()
//========================================================
{
    m_mutex->lock();

    // Checking device.db's tables
    CheckingDefaultTables(DEVICE_DB);
    CheckingDefaultTables(SCHEDULE_DB);

    m_mutex->unlock();
}

//========================================================
void MainDBCtrl::CheckingDefaultTables(const char* dbPath)
//========================================================
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");

    db.setDatabaseName(dbPath);
    const bool bIsOpened = db.open();
    if(false == bIsOpened)
    {
        ELGO_MAIN_LOG("ERROR - DB Open: %s", dbPath);
        db.close();

        return;
    }

    if(0 == strcmp(dbPath, DEVICE_DB))
    {
        QSqlQuery query(db);
        query.prepare(DB_Query::CHECK_DEVICE_TABLE);
        query.bindValue(":table", "device");

        const bool bIsChecked = query.exec();
        if(true == bIsChecked)
        {
            // device table
            int countIdx = 0;
            int countValue = 0;
            while(query.next())
            {
                countIdx = query.record().indexOf("COUNT");
                countValue = query.value(countIdx).toInt();
                ELGO_MAIN_LOG("device table, count - {idx: %d, value: %d}",
                              countIdx, countValue);
            }

            if(ZERO_RESULT == countValue)
            {
                query.clear();
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
                ELGO_MAIN_LOG("Already Made device table - countIdx: %d", countIdx);
            }
        }
        else
        {
            ELGO_MAIN_LOG("ERROR - Failed query.exec(): %s", DB_Query::CHECK_DEVICE_TABLE);
        }
    }
    else if(0 == strcmp(dbPath, SCHEDULE_DB))
    {
        QSqlQuery query(db);

        // playSchedule table
        query.prepare(DB_Query::CHECK_DEVICE_TABLE);
        query.bindValue(":table", "playSchedule");

        const bool bIsPlayChecked = query.exec();
        if(true == bIsPlayChecked)
        {
            int countIdx = 0;
            int countValue = 0;
            while(query.next())
            {
                countIdx = query.record().indexOf("COUNT");
                countValue = query.value(countIdx).toInt();
                ELGO_MAIN_LOG("playSchedule table, count - {idx: %d, value: %d}",
                              countIdx, countValue);
            }

            if(ZERO_RESULT == countValue)
            {
                // Play Schedule
                query.clear();
                query.prepare(DB_Query::CREATE_TABLE_PLAY_SCHEDULE);
                const bool bIsPlayCreated = query.exec();
                if(false == bIsPlayCreated)
                {
                    ELGO_MAIN_LOG("ERROR - Failed query.exec(): %s{%s}",
                                  DB_Query::CREATE_TABLE_PLAY_SCHEDULE,
                                  query.lastError().text().toStdString().c_str());
                }
            }
        }
        else
        {
            ELGO_MAIN_LOG("ERROR - Failed query.exec(): %s{%s}",
                          DB_Query::CHECK_DEVICE_TABLE,
                          query.lastError().text().toStdString().c_str());
        }

        query.clear();
        query.prepare(DB_Query::CHECK_DEVICE_TABLE);
        query.bindValue(":table", "powerSchedule");

        const bool bIsPowerChecked = query.exec();
        if(true == bIsPowerChecked)
        {
            int countIdx = 0;
            int countValue = 0;
            while(query.next())
            {
                countIdx = query.record().indexOf("COUNT");
                countValue = query.value(countIdx).toInt();
                ELGO_MAIN_LOG("powerSchedule table, count - {idx: %d, value: %d}",
                              countIdx, countValue);
            }

            if(ZERO_RESULT == countValue)
            {

                // Power Schedule
                query.clear();
                query.prepare(DB_Query::CREATE_TABLE_POWER_SCHEDULE);
                const bool bIsPowerCreated = query.exec();
                if(false == bIsPowerCreated)
                {
                    ELGO_MAIN_LOG("ERROR - Failed query.exec(): %s{%s}",
                                  DB_Query::CREATE_TABLE_POWER_SCHEDULE,
                                  query.lastError().text().toStdString().c_str());
                }
            }
        }
        else
        {
            ELGO_MAIN_LOG("ERROR - Failed query.exec(): %s{%s}",
                          DB_Query::CHECK_DEVICE_TABLE,
                          query.lastError().text().toStdString().c_str());
        }
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

    QSqlQuery query(db);
    query.prepare(DB_Query::SELECT_ID_PLAY_SCHEDULE);
    const bool bIdExec =  query.exec();
    if(true == bIdExec)
    {
        // Get Insterted ID
        QVector<QString> idList;
        while(query.next())
        {
            const int idIdx = query.record().indexOf("id");
            const QString& id = query.value(idIdx).toString();
            idList.push_back(id);
        }

        // Check Duplicated
        QVector<ScheduleJson::PlaySchedule>::const_iterator iter = playScheduleList.constBegin();
        while(iter != playScheduleList.end())
        {
            const bool bValidId = CheckDuplicatedId(idList, (*iter).id);
            if(true == bValidId)
            {
                const QString& id = (*iter).id;
                QVector<ScheduleJson::PlayScheduleData>::const_iterator dataIter = (*iter).scheduleList.constBegin();
                while(dataIter != (*iter).scheduleList.constEnd())
                {
                    query.clear();
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
            }
            else
            {
                ELGO_MAIN_LOG("Already Inserted PlaySchedule ID: %s",
                              (*iter).id.toStdString().c_str());
            }
            ++iter;
        }
    }
    else
    {
        ELGO_MAIN_LOG("ERROR - Failed query.exec(): %s{%s}",
                      DB_Query::SELECT_ID_PLAY_SCHEDULE,
                      query.lastError().text().toStdString().c_str());
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

    m_mutex->unlock();
}

//========================================================
bool MainDBCtrl::CheckDuplicatedId(const QVector<QString>& dbIdList, const QString& id)
//========================================================
{
    QVector<QString>::const_iterator iter = dbIdList.constBegin();
    while(iter != dbIdList.constEnd())
    {
        if(0 == strcmp((*iter).toStdString().c_str(),
                       id.toStdString().c_str()))
        {
            return false;
        }
        ++iter;
    }

    return true;
}
