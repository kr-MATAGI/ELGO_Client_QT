#ifndef PLAYTIMER_H
#define PLAYTIMER_H

// QT
#include <QTimer>

// Viewer
#include "Widget/Definition/ScheduleTimerDef.h"
#include "Logger/ViewerLogger.h"
#include "Widget/ContentsPlayer.h"

class ContentsPlayer;

class SchedulesTimer : public QTimer
{
    Q_OBJECT
public:

    /** @brief */
    SchedulesTimer(QObject *parent = nullptr);
    /** @brief */
    virtual ~SchedulesTimer();

public:
    /** @brief */
    void AddPlaySchedule(const QVector<ScheduleJson::PlaySchedule>& src);

private:
    /** @brief */
    void ExecSchedule(const QString& scheduleId, const ScheduleTimer::PlayDataInfo& playData);

    /** @brief */
    bool IsValidCronRuleValue(const QDateTime& currentDateTime, const ScheduleJson::Cron& cron);


    /** @brief */
    QString ConvertDateTimeFormat(const QDateTime& src);

private slots:
    /** @brief */
    void SchedulerTimeout();

private:
    QString m_currScheduleId;
    ScheduleTimer::PlayDataInfo m_currPlayDataInfo;
    ScheduleTimer::PlayDataInfo m_prevSinglePlayDataInfo;

    QVector<ScheduleJson::PlaySchedule> m_playScheduleList;
    QVector<ScheduleTimer::PlayDataInfo> m_playDataInfoList;
};

#endif // PLAYTIMER_H
