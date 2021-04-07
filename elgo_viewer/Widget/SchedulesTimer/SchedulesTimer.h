#ifndef PLAYTIMER_H
#define PLAYTIMER_H

// QT
#include <QTimer>

// Viewer
#include "ScheduleTimerDef.h"
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
    void AddPlaySchedule(const ScheduleJson::PlaySchedules& src);

    /** @brief */
    void ExecSinglePlayEvent(const PlayJson::PlayData& src);

private:
    /** @brief */
    void ExecSchedule(const QString& scheduleId);

    /** @brief */
    bool IsValidCronRuleValue(const QDateTime& currentDateTime, const ScheduleJson::Cron& cron);


    /** @brief */
    QString ConvertDateTimeFormat(const QDateTime& src);

private slots:
    /** @brief */
    void SchedulerTimeout();

private:
    QString m_currScheduleId;
    SchedulerDef::PlayDataInfo m_currPlayDataInfo;
    SchedulerDef::PlayDataInfo m_prevSinglePlayDataInfo;

    QVector<ScheduleJson::PlaySchedules> m_playScheduleList;
    QVector<SchedulerDef::PlayDataInfo> m_playDataInfoList;
};

#endif // PLAYTIMER_H
