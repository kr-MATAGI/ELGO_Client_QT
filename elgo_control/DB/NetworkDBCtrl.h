#ifndef NETWORKDBCTRL_H
#define NETWORKDBCTRL_H

// QT
#include <QObject>
#include <QtSql>
#include <QMutex>

// Common
#include "Common/DBDefinition.h"

// Control
#include "JSON/Schema/RemoteSchema.h"

class NetworkDBCtrl
{
public:
    /** @brief */
    NetworkDBCtrl();
    /** @brief */
    ~NetworkDBCtrl();

    /** @brief*/
    bool CheckDeviceLogingInfo(Remote::DeviceJson& deviceInfo);

private:
    QMutex *m_mutex;
};

#endif // NETWORKDBCTRL_H
