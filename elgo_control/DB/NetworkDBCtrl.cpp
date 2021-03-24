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
    const bool bIsDbOpen = db.open();
    if(false == bIsDbOpen)
    {
        const QSqlError error = db.lastError();
        ELGO_CONTROL_LOG("Error - db open : %s", error.text().toUtf8().constData());
    }
    else
    {
        QSqlQuery query;
        query.prepare(DB_Query::SELECT_ALL_INFO_DEVICE);
        const bool bIsOkExec = query.exec();

        if(false == bIsOkExec)
        {
            ELGO_CONTROL_LOG("Error - query exec : %s",
                             DB_Query::SELECT_ALL_INFO_DEVICE.toUtf8().constData());
        }
        else
        {
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
        }
    }

    db.close();
    m_mutex->unlock();

    return retValue;
}

//========================================================
bool NetworkDBCtrl::ChangeDevicePassword(Remote::ManageDevice& manageDeviceInfo)
//========================================================
{
    bool retValue = false;

    // compare db password & oldPw
    m_mutex->lock();
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(DEVICE_DB);
    const bool bIsDbOpen = db.open();
    if(false == bIsDbOpen)
    {
        const QSqlError error = db.lastError();
        ELGO_CONTROL_LOG("Error - db open : %s", error.text().toUtf8().constData());
    }
    else
    {
        QSqlQuery query;
        query.prepare(DB_Query::SELECT_ALL_INFO_DEVICE);
        const bool bIsOkExec = query.exec();

        if(false == bIsOkExec)
        {
            ELGO_CONTROL_LOG("Error - query exec : %s",
                             DB_Query::SELECT_ALL_INFO_DEVICE.toUtf8().constData());
        }
        else
        {
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
        }
    }


    db.close();
    m_mutex->unlock();

    return retValue;
}

//========================================================
void NetworkDBCtrl::UpdateDeviceNameFromServer(const QString& src)
//========================================================
{
    m_mutex->lock();

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(DEVICE_DB);
    const bool bIsDbOpen = db.open();
    if(false == bIsDbOpen)
    {
        const QSqlError error = db.lastError();
        ELGO_CONTROL_LOG("Error - db open : %s", error.text().toUtf8().constData());
    }
    else
    {
        QSqlQuery query;
        query.prepare(DB_Query::SELECT_DEVICE_NAME);
        const bool bIsOkExec = query.exec();

        if(false == bIsOkExec)
        {
            ELGO_CONTROL_LOG("Error - query exec : %s",
                             DB_Query::SELECT_DEVICE_NAME.toUtf8().constData());
        }
        else
        {
            QString dbName;
            const int nameFieldidx = query.record().indexOf("name");
            while(query.next())
            {
                dbName = query.value(nameFieldidx).toString();
            }

            if(0 == std::strcmp(dbName.toStdString().c_str(), src.toStdString().c_str()))
            {
                ELGO_CONTROL_LOG("Already updated device name");
            }
            else
            {
                // update
                query.prepare(DB_Query::UPDATE_DEVICE_NAME);
                query.bindValue(":name", src);
                const bool bIsUpdated = query.exec();
                if(false == bIsUpdated)
                {
                    ELGO_CONTROL_LOG("Error - Not updated device name");
                }
                else
                {
                    ELGO_CONTROL_LOG("Updated device name %s -> %s",
                                     dbName.toUtf8().constData(), src.toUtf8().constData());
                }
            }
        }
    }

    db.close();
    m_mutex->unlock();
}
