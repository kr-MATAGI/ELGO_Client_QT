// QT
#include <QSqlQuery>

// Main
#include "MainDBCtrl.h"

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
    db.close();

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

        // id: root, pw : root, name : elgo
        const int randMax = 1000000;
        const int randMin = 1;
        int randomValue = (double)std::rand() / RAND_MAX * (randMax - randMin) + randMin;
        query.prepare(DB_Query::INSERT_VALUES_DEVICE);

        query.bindValue(":id", "root");
        query.bindValue(":pw", "root");
        query.bindValue(":name", QString("elgo_d_" + QString::number(randomValue)));
        query.exec();
    }

    // account table - NOT USING
#if 0
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
#endif

    // network table
    // NOT USING, below code will be replaced XML
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
