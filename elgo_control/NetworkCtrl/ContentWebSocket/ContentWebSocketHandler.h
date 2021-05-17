#ifndef CONTENTWEBSOCKETHANDLER_H
#define CONTENTWEBSOCKETHANDLER_H

#define MAX_THREAD_COUNT    4

// QT
#include <QThreadPool>
#include <QTimer>

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
    void ExecPowerSchedulesEvent(const ContentSchema::Summary& serverJson);

    /** @brief */
    void ExecDisplayOnOffEvent(const ContentSchema::Summary& serverJson, QString& clientJson);

    /** @brief */
    void ExecScreenCaptureEvent(const ContentSchema::Summary& serverJson);

    /** @brief */
    void ExecSystemRebootEvent(const ContentSchema::Summary& serverJson, QString& clientJson);

    /** @brief */
    void ExecClearPlaySchdules(const ContentSchema::Summary& serverJson, QString& clientJson);

    /** @brief */
    void ExecClearPowerSchedules(const ContentSchema::Summary& serverJson, QString& clientJson);

private:
    QThreadPool *m_threadPool;
};

#endif // CONTENTWEBSOCKETHANDLER_H
