#ifndef CONTENTWEBSOCKETHANDLER_H
#define CONTENTWEBSOCKETHANDLER_H

#define MAX_THREAD_COUNT    4

// QT
#include <QThreadPool>

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
    void RunEvent(const ContentSchema::Summary& serverJson, QString& clientJson);

    /** @brief */
    void ExecReadyEvent(const ContentSchema::Summary& serverJson, QString& clientJson);

    /** @brief */
    void ExecRenameEvent(const ContentSchema::Summary& serverJson, QString& clientJson);

    /** @brief */
    void ExecSinglePlayEvent(const ContentSchema::Summary& serverJson);

    /** @brief */
    void ExecPlaySchedulesEvent(const ContentSchema::Summary& serverJson);

    /** @brief */
    void ExecPowerSchedulesEvent();

    /** @brief */
    void ExecClearPlaySchdules();

    /** @brief */
    void ExecClearPowerSchedules();

private:
    QThreadPool *m_threadPool;
};

#endif // CONTENTWEBSOCKETHANDLER_H
