// std
#include <cstring>

// Control
#include "Logger/ControlLogger.h"
#include "NetworkDBCtrl.h"

//========================================================
NetworkDBCtrl::NetworkDBCtrl()
//========================================================
{
    m_mutex = new QMutex;
}

//========================================================
NetworkDBCtrl::~NetworkDBCtrl()
//========================================================
{
    delete m_mutex;
    m_mutex = NULL;
}

//========================================================
bool NetworkDBCtrl::CheckDeviceLogingInfo(Remote::DeviceLogin& deviceInfo)
//========================================================
{
    bool retValue = false;

    // get id, pw from db
    m_mutex->lock();

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(DEVICE_DB);
    db.open();

    QSqlQuery query;
    query.prepare(DB_Query::SELECT_ALL_INFO_DEVICE);
    query.exec();

    QString id;
    QString pw;
    const int idFieldIdx = query.record().indexOf("id");
    const int pwFieldIdx = query.record().indexOf("pw");
    while(query.next())
    {
        id = query.value(idFieldIdx).toString();
        pw = query.value(pwFieldIdx).toString();
    }

    // compare db, json
    const int idCmpResult = std::strcmp(deviceInfo.id.toUtf8().constData(), id.toUtf8().constData());
    const int pwCmpResult = std::strcmp(deviceInfo.pw.toUtf8().constData(), pw.toUtf8().constData());

    if(0 == idCmpResult && 0 == pwCmpResult)
    {
        retValue = true;
    }
    else
    {
        ELGO_CONTROL_LOG("remote {id : %s, pw : %s} db {id : %s, pw : %s} is NOT Valid",
                         deviceInfo.id.toUtf8().constData(), deviceInfo.pw.toUtf8().constData(),
                         id.toUtf8().constData(), pw.toUtf8().constData());
    }
    db.close();
    m_mutex->unlock();

    return retValue;
}

//========================================================
bool NetworkDBCtrl::ChangeDevicePassword(Remote::MangeDevice& manageDeviceInfo)
//========================================================
{
    bool retValue = false;

    // compare db password & oldPw
    m_mutex->lock();
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(DEVICE_DB);
    db.open();

    QSqlQuery query;
    query.prepare(DB_Query::SELECT_ALL_INFO_DEVICE);
    query.exec();

    QString dbPw;
    const int pwFieldidx = query.record().indexOf("pw");
    while(query.next())
    {
        dbPw = query.value(pwFieldidx).toString();
    }

    const int pwCmpResult = std::strcmp(manageDeviceInfo.oldPw.toUtf8().constData(), dbPw.toUtf8().constData());
    if(0 == pwCmpResult)
    {
        // update
        query.prepare(DB_Query::UPDATE_PW_INFO_DEVICE);
        query.bindValue(":pw", manageDeviceInfo.newPw);
        retValue = query.exec();
        if(false == retValue)
        {
            ELGO_CONTROL_LOG("Error - Not updated new password");
        }
    }
    else
    {
        ELGO_CONTROL_LOG("oldPassword and dbPassword is not same");
    }

    db.close();
    m_mutex->unlock();

    return retValue;
}
