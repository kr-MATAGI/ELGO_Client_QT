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

public:
    /** @brief */
    bool CheckDeviceLogingInfo(Remote::DeviceLogin& deviceInfo);

    /** @brief */
    bool ChangeDevicePassword(Remote::ManageDevice& manageDeviceInfo);

    /** @brief */
    void UpdateDeviceNameFromServer(const QString& src);

private:
    QMutex *m_mutex;
};

#endif // NETWORKDBCTRL_H
