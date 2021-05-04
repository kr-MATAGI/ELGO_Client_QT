#ifndef MAINEVENTSTATE_H
#define MAINEVENTSTATE_H

#define MAX_THREAD_COUNT 4

// QT
#include <QByteArray>
#include <QThreadPool>

// EFC
#include "Common/EventList.h"
#include "LocalSocketEvent/EventState.h"

// Main
#include "MainCtrl/MainController.h"

class MainController;

class MainEventState
{
public:
    MainEventState();
    ~MainEventState();

public:
    /** @brief */
    void ExecState(const quint16 event, const QByteArray &src);

    /** @brief */
    void RecvProcecssReady(const QByteArray& src);

    /** @brief */
    void RecvUpdateDeviceOptions(const QByteArray& src);

    /** @brief */
    void RecvUpdateDisplaySleep(const QByteArray& src);

    /** @brief */
    void RecvSystemReboot(const QByteArray& src);

    /** @brief */
    void RecvSearchingWifiList(const QByteArray& src);
    /** @brief */
    void RecvConnectNewWifi(const QByteArray& src);

    /** @brief */
    void RecvUpdatePlaySchedule(const QByteArray& src);
    /** @brief */
    void RecvClearAllPlaySchedule(const QByteArray& src);
    /** @brief */
    void RecvClearPlayScheduleById(const QByteArray& src);

    /** @brief */
    void RecvAddPlayDataToDB(const QByteArray& src);

private:
    QThreadPool *m_threadPool;
    EventState<MainEventState> m_state;
};

#endif // MAINEVENTSTATE_H
