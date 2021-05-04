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

    /** @brief */
    void AddPowerScheduleList(const QVector<ScheduleJson::PowerScheduleData>& src);

    /** @brief */
    void StartPowerTimer();
    /** @brief */
    void StopPowerTimer();
    /** @brief */
    bool IsActivePowerTimer();

private slots:
    /** @brief */
    void PowerScheduleTimeout();

private:
    bool m_bIsActive;

    QVector<ScheduleJson::PowerScheduleData> m_scheduleList;
};

#endif // POWERSCHEDULETIMER_H
