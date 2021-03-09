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
bool NetworkDBCtrl::CheckDeviceLogingInfo(Remote::DeviceJson& deviceInfo)
//========================================================
{
    bool retValue = false;

    // get id, pw from db
    QSqlQuery query(QSqlDatabase::database(DEVICE_DB));
    query.prepare(DB_Query::SELECT_ALL_INFO_DEVICE);
    query.exec();

    QString id;
    QString pw;
    const int idFieldIdx = query.record().indexOf("id");
    const int pwFieldIdx = query.record().indexOf("pw");
    while(query.next()){
        id = query.value(idFieldIdx).toString();
        pw = query.value(pwFieldIdx).toString();
    }

    // compare db, json
    const int idCmpResult = std::strcmp(deviceInfo.id.toUtf8().constData(), pw.toUtf8().constData());
    const int pwCmpResult = std::strcmp(deviceInfo.pw.toUtf8().constData(), pw.toUtf8().constData());

    if(0 == idCmpResult && 0 == pwCmpResult)
    {
        retValue = true;
    }
    else
    {
        ELGO_CONTROL_LOG("{id : %s, pw : %s} is NOT Valid",
                         deviceInfo.id.toUtf8().constData(), deviceInfo.pw.toUtf8().constData());
    }

    return retValue;
}
