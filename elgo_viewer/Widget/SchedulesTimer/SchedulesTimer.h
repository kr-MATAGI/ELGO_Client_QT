#ifndef PLAYTIMER_H
#define PLAYTIMER_H

// QT
#include <QTimer>

// Viewer
#include "SinglePlayTimer.h"
#include "Logger/ViewerLogger.h"

class SchedulesTimer : public QTimer
{
    Q_OBJECT
public:
    /** @brief */
    static SchedulesTimer *pInstance;

    /** @brief */
    SchedulesTimer(QObject *parent = nullptr);
    /** @brief */
    virtual ~SchedulesTimer();

    /** @brief */
    static SchedulesTimer* GetInstance();
    /** @brief */
    void DestoryInstance();

public:
    /** @brief */
    void AddPlayDataList(const PlayJson::FixedPlayDataJson& src);
    /** @brief */
    void AddPlayDataList(const PlayJson::CustomPlayDataJson& src);
    /** @brief */
    void AddPlaySchedule(const ScheduleJson::PlaySchedules& src);

private:
    /** @brief */
    void ExecSchedule(const QString& scheduleId);
    /** @brief */
//    void UpdatePlayerScene(const PlayDataInfo& playDataInfom, const bool bIsSchedule = false);


    /** @brief */
    bool IsValidCronRuleValue(const QDateTime& currentDateTime, const ScheduleJson::Cron& cron);


    /** @brief */
    QString ConvertDateTimeFormat(const QDateTime& src);

private slots:
    /** @brief */
    void SchedulerTimeout();

private:
    SinglePlayTimer m_singleTimer;

    QString m_currScheduleId;
    SchedulerDef::PlayDataInfo m_currPlayDataInfo;
    SchedulerDef::PlayDataInfo m_prevSinglePlayDataInfo;

    QVector<ScheduleJson::PlaySchedules> m_playScheduleList;
    QVector<SchedulerDef::PlayDataInfo> m_playDataInfoList;
};

#endif // PLAYTIMER_H
