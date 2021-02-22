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
    db.setDatabaseName(DEVICE_DB_PATH);

    bool bIsOpenedDB = db.open();
    if(false == bIsOpenedDB)
    {
        qDebug() << "DB Open ERROR : " << DEVICE_DB_PATH;
    }

    m_mutex->unlock();
}

//========================================================
void MainDBCtrl::CreateDeviceDBTable()
//========================================================
{
    m_mutex->lock();

    QSqlQuery query;
    bool bIsOK = query.exec(DB_Query::CREATE_TABLE_DEVICE);
    if(false == bIsOK)
    {
        qDebug() << "query error - " << DB_Query::CREATE_TABLE_DEVICE;
    }

    m_mutex->unlock();
}

//========================================================
void MainDBCtrl::CreateAccountDBTable()
//========================================================
{
    m_mutex->lock();

    QSqlQuery query;
    bool bIsOk = query.exec(DB_Query::CREATE_TABLE_ACCOUNT);
    if(false == bIsOk)
    {
        qDebug() << "querry error - " << DB_Query::CREATE_TABLE_ACCOUNT;
    }

    m_mutex->unlock();
}

//========================================================
void MainDBCtrl::GetAllInfoFromDeviceTable()
//========================================================
{
    m_mutex->lock();



    m_mutex->unlock();
}

//========================================================
void MainDBCtrl::GetAllInfoFromAccountTable()
//========================================================
{
    m_mutex->lock();



    m_mutex->unlock();
}
