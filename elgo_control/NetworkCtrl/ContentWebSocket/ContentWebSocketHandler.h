#ifndef CONTENTWEBSOCKETHANDLER_H
#define CONTENTWEBSOCKETHANDLER_H

// Control
#include "JSON/Schema/ContentServerSchema.h"
#include "NetworkCtrl/NetworkController.h"

class NetworkController;

class ContentWebSocketHandler
{
public:
    /** @brief */
    ContentWebSocketHandler();
    /** @brief */
    ~ContentWebSocketHandler();

    /** @brief */
    void RunEvent(const ContentSchema::Summary& response, QString& request);
};

#endif // CONTENTWEBSOCKETHANDLER_H