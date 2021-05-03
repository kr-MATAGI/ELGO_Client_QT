#ifndef SCHEDULETIMER_H
#define SCHEDULETIMER_H

// QT
#include <QTimer>

// Common
#include "Common/Interface/ScheduleJsonDef.h"

// Main
#include "MainCtrl/MainController.h"

class MainController;

class ScheduleTimer : public QTimer
{
    Q_OBJECT
public:
    /** @brief*/
    explicit ScheduleTimer(QObject *parent = nullptr);
    /** @brief*/
    virtual ~ScheduleTimer();

    /** @brief*/
    void AddPlayScheduleList(const QVector<ScheduleJson::PlaySchedule>& src);

private slots:
    /** @brief */
    void ScheduleTimeout();

private:
    /** @brief */
    bool CheckValidPlayScheduleId(const QString& id);

private:
    QVector<ScheduleJson::PlaySchedule> m_playScheduleList;
    QVector<ScheduleJson::PowerSchedule> m_powerScheduleList;
};

#endif // SCHEDULETIMER_H
