#include "MainDBCtrl.h"

#include <QSqlQuery>

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
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(DEVICE_DB);

    bool bIsOpenedDB = db.open();
    if(false == bIsOpenedDB)
    {
        qDebug() << DEVICE_DB << " Open Error" ;
        db.close();

        return;
    }

    CheckingDefaultTables();

    m_mutex->unlock();
}

//========================================================
void MainDBCtrl::CheckingDefaultTables()
//========================================================
{
    QSqlQuery query(QSqlDatabase::database(DEVICE_DB));
    query.prepare(DB_Query::SELECT_ALL_INFO_DEVICE);
    query.exec();

    // device table
    if(-1 == query.numRowsAffected())
    {
        query.prepare(DB_Query::CREATE_TABLE_DEVICE);
        query.exec();
    }

    // account table
    query.prepare(DB_Query::SELECT_ALL_INFO_ACCOUNT);
    query.exec();
    if(-1 == query.numRowsAffected())
    {
        query.prepare(DB_Query::CREATE_TABLE_ACCOUNT);
        query.exec();
    }

    // schedule table
    query.prepare(DB_Query::SELECT_ALL_SCHEDULE);
    query.exec();
    if(-1 == query.numRowsAffected())
    {
        query.prepare(DB_Query::CREATE_TABLE_SCHEDULE);
        query.exec();
    }
}
