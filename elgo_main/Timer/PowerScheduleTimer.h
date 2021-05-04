#ifndef POWERSCHEDULETIMER_H
#define POWERSCHEDULETIMER_H

// QT
#include <QTimer>

// Common
#include "Common/Interface/ScheduleJsonDef.h"

// Main
#include "MainCtrl/MainController.h"

class MainController;

class PowerScheduleTimer : public QTimer
{
    Q_OBJECT

public:
    /** @brief */
    explicit PowerScheduleTimer(QObject *parent = nullptr);
    /** @brief */
    virtual ~PowerScheduleTimer();

private slots:
    /** @brief */
    void PowerScheduleTimeout();

private:
    bool m_bIsActive;
};

#endif // POWERSCHEDULETIMER_H
