#ifndef REMOTECONTROLHANDLER_H
#define REMOTECONTROLHANDLER_H

// Control
#include "JSON/Schema/RemoteSchema.h"
#include "NetworkCtrl/NetworkController.h"

class NetworkController;

class RemoteControlHandler
{
public:
    /** @brief */
    RemoteControlHandler();
    ~RemoteControlHandler();

public:
    /** @brief  RunAction() will be return inner method result */
    void RunAction(Remote::Action action, const QString& src, Remote::Result::Contents& dest);

    /** @brief */
    Remote::Result::Status RemoteDeviceLogin(const QString& src);

    /** @brief */
    Remote::Result::Status RemoteGetAvailableWifiList(const QString& src);

    /** @brief */
    Remote::Result::Status RemoteManageDevice(const QString& src);

    /** @brief */
    Remote::Result::Status RemoteRotateDeviceScreen(const QString& src);

    /** @brief */
    Remote::Result::Status RemoteUpdateDeviceOptions(const QString& src);

    /** @brief */
    Remote::Result::Status RemoteConnectNewWifi(const QString& src);

    /** @brief */
    Remote::Result::Status RemoteUserLogin(const QString& src, Remote::Result::Contents& dest);
};

#endif // REMOTECONTROLHANDLER_H
