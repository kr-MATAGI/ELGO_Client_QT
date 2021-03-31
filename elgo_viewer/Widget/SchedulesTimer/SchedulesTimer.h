#ifndef PLAYTIMER_H
#define PLAYTIMER_H

// QT
#include <QTimer>
#include <QVector>

// Viewer
#include <Logger/ViewerLogger.h>

// Common
#include "Common/Interface/ContentsPlayDataDef.h"
#include "Common/Interface/ScheduleDef.h"

/** @brief  std::pair<PlayDataId, PlayJson::PlayDataType> */
typedef std::pair<int, PlayJson::PlayDataType> PlayDataInfo;

class SchedulesTimer : public QTimer
{
    Q_OBJECT
public:
    SchedulesTimer(QObject *parent = nullptr);
    virtual ~SchedulesTimer();

    /** @brief */
    void AddPlayDataList(const PlayJson::FixedPlayDataJson& src, const bool bIsSinglePlay = false);
    /** @brief */
    void AddPlayDataList(const PlayJson::CustomPlayDataJson& src, const bool bIsSinglePlay = false);
    /** @brief */
    void AddPlaySchedule(const ScheduleJson::PlaySchedules& src);

    /** @brief */
    void ClearAllSchedulesBySinglePlay();

    /** @brief */
    void CheckPlayDateTimeSchedules(const QDateTime& currentDateTime);
    /** @brief */
    bool IsValidCronRuleValue(const QDateTime& currentDateTime, const ScheduleJson::Cron& cron);
    /** @brief */
    void ExecSchedule(const QString& scheduleId, const PlayDataInfo& playDataInfo);
    /** @brief */
    bool ExecPlayData(const PlayDataInfo& playDataInfo);

    /** @brief */
    QString ConvertDateTimeFormat(const QDateTime& src);

private slots:
    /** @brief */
    void CheckContentPlayerSchedules();

private:
    bool m_bIsSinglePlay;
    bool m_bIsExecSchedule;
    QString m_currentScheduleId;
    PlayDataInfo m_currentPlayData;
    PlayDataInfo m_prevPlayData;
    int m_timeCount;

    QVector<PlayJson::FixedPlayDataJson> m_fixedPlayDataList;
    QVector<PlayJson::CustomPlayDataJson> m_customPlayDataList;
    QVector<ScheduleJson::PlaySchedules> m_playScheduleList;
};

#endif // PLAYTIMER_H
