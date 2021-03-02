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

    // Checking device.db's tables
    CheckingDefaultTables();

    m_mutex->unlock();
}

//========================================================
void MainDBCtrl::CheckingDefaultTables()
//========================================================
{
    const int ZERO_RESULT = -1;

    QSqlQuery query(QSqlDatabase::database(DEVICE_DB));
    query.prepare(DB_Query::SELECT_ALL_INFO_DEVICE);
    query.exec();

    // device table
    if(ZERO_RESULT == query.numRowsAffected())
    {
        query.prepare(DB_Query::CREATE_TABLE_DEVICE);
        query.exec();
    }

    // account table
    query.prepare(DB_Query::SELECT_ALL_INFO_ACCOUNT);
    query.exec();
    if(ZERO_RESULT == query.numRowsAffected())
    {
        query.prepare(DB_Query::CREATE_TABLE_ACCOUNT);
        query.exec();
    }

    // schedule table
    query.prepare(DB_Query::SELECT_ALL_SCHEDULE);
    query.exec();
    if(ZERO_RESULT == query.numRowsAffected())
    {
        query.prepare(DB_Query::CREATE_TABLE_SCHEDULE);
        query.exec();
    }

    // network table
    // not using, below code will be replaced XML
#if 0
    query.prepare(DB_Query::SELECT_ALL_NETWORK_TABLE);
    query.exec();
    if(ZERO_RESULT == query.numRowsAffected())
    {
        query.prepare(DB_Query::CREATE_NETWORK_TABLE);
        query.exec();
    }
#endif
}
