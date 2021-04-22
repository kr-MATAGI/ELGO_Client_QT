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
    void RunAction(Remote::Action action, const QString& src, Remote::Result::Contents& results);

    /** @brief */
    Remote::Result::Status GetDeviceLoginInfoValidation(const QString& src);

    /** @brief */
    Remote::Result::Status GetAvailableWifiList(const QString& src);

    /** @brief */
    Remote::Result::Status ManageDeviceInfo(const QString& src);

    /** @brief */
    Remote::Result::Status RotateDeviceDisplay(const QString& src);

    /** @brief */
    Remote::Result::Status UpdateDeviceOptions(const QString& src);

    /** @brief */
    Remote::Result::Status ConnectNewWifi(const QString& src);
};

#endif // REMOTECONTROLHANDLER_H
