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

    /** @brief */
    void RunAction(Remote::Action action, const QString& src);

    /** @brief */
    bool GetDeviceLoginInfoValidation(const QString& src);

};

#endif // REMOTECONTROLACTIONHANDLER_H
