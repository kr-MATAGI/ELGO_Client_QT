#ifndef MAINTHREAD_H
#define MAINTHREAD_H

//QT
#include <QRunnable>
#include <QByteArray>

// EFC
#include "Common/EventList.h"

// Main
#include "DB/MainDBCtrl.h"
#include "MainCtrl/MainController.h"

class MainController;

class MainThread : public QRunnable
{
public:
    MainThread();
    ~MainThread();

public:
    /** @brief QRunnable run() override */
    void run() override;

    /** @brief */
    void ExecRecvProcecssReady();

    /** @brief */
    void ExecSearchingWifiList();
    /** @brief */
    void ExecConnectNewWifi();

public:
    /** @brief */
    void SetMainEvent(const MAIN_EVENT::Event event);

    /** @brief */
    void SetRecvBytes(const QByteArray &src);

private:
    /** @brief */
    void CheckValidResourceFile(const QVector<PlayJson::CustomPlayDataJson>& customSrc,
                                const QVector<PlayJson::FixedPlayDataJson>& fixedSrc);

private:
    MAIN_EVENT::Event m_event;
    QByteArray m_bytes;
};

#endif // MAINTHREAD_H
