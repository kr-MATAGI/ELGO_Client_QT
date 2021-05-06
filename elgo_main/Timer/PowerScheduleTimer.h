#ifndef POWERSCHEDULETIMER_H
#define POWERSCHEDULETIMER_H

// QT
#include <QTimer>

// Common
#include "Common/Interface/ScheduleJsonDef.h"

// Main
#include "MainCtrl/MainController.h"

// <id, PowerStatus>
typedef std::pair<QString, ScheduleJson::PowerStatus> POWER_INFO;

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
    void DeletePowerScheduleById(const QString& id);

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
    /** @brief */
    QString ConvertDateTimeToString(const QDateTime& src);

    /** @brief */
    bool CheckValidPowerSchedule(const QDateTime& currDateTime,
                                 const ScheduleJson::Cron& cron);

private:
    bool m_bIsActive;

    QVector<ScheduleJson::PowerScheduleData> m_scheduleList;
    POWER_INFO m_powerInfo;
};

#endif // POWERSCHEDULETIMER_H
