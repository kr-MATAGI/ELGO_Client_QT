#ifndef REMOTECONTROLACTIONHANDLER_H
#define REMOTECONTROLACTIONHANDLER_H


// Control
#include "JSON/Schema/RemoteSchema.h"
#include "NetworkCtrl/NetworkController.h"

class NetworkController;

class RemoteControlActionHandler
{
public:
    /** @brief */
    RemoteControlActionHandler();
    ~RemoteControlActionHandler();

public:
    /** @brief  RunAction() will be return inner method result */
    void RunAction(Remote::Action action, const QString& src, Remote::Result::Contents& results);

    /** @brief */
    bool GetDeviceLoginInfoValidation(const QString& src);

    /** @brief */
    bool GetAvailableWifiList(const QString& src);

    /** @brief */
    bool ManageDeviceInfo(const QString& src);

};

#endif // REMOTECONTROLACTIONHANDLER_H
