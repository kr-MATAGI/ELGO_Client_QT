#ifndef MAINTHREAD_H
#define MAINTHREAD_H

//QT
#include <QRunnable>
#include <QByteArray>

// EFC
#include "Common/EventList.h"

// Main
#include "DB/MainDBCtrl.h"

class MainThread : public QRunnable
{
public:
    MainThread();
    ~MainThread();

public:
    /** @brief QRunnable run() overrid */
    void run() override;

    /** @brief */
    void ExecRecvProcecssReady();

public:
    /** @brief */
    void SetMainEvent(MAIN_EVENT::Event event);

    /** @brief */
    void SetRecvBytes(const QByteArray &src);

private:
    MAIN_EVENT::Event m_event;
    QByteArray m_bytes;

    MainDBCtrl *m_dbCtrl;
};

#endif // MAINTHREAD_H
