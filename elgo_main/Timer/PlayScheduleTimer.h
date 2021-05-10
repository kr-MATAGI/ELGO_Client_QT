#ifndef PLAYSCHEDULETIMER_H
#define PLAYSCHEDULETIMER_H

// QT
#include <QTimer>

// Common
#include "Common/Interface/ScheduleJsonDef.h"
#include "Common/Interface/ContentsPlayDataDef.h"

// Main
#include "MainCtrl/MainController.h"

class MainController;

class PlayScheduleTimer : public QTimer
{
    Q_OBJECT
public:
    /** @brief */
    explicit PlayScheduleTimer(QObject *parent = nullptr);
    /** @brief */
    virtual ~PlayScheduleTimer();

    /** @brief */
    void AddPlayScheduleList(const QVector<ScheduleJson::PlaySchedule>& src);

    /** @brief */
    void ClearAllPlayScheduleList();
    /** @brief */
    void DeletePlayScheduleById(const QString& id);

    /** @brief */
    void UpdatePlayingData(const int id, const PlayJson::PlayDataType type);

    /** @brief */
    void StartPlayTimer();
    /** @brief */
    void StopPlayTimer();
    /** @brief */
    bool IsActivePlayTimer();

signals:
    /** @brief */
    void AddPlayScheduleListSignal(const QVector<ScheduleJson::PlaySchedule>& src);

    /** @brief */
    void PlayScheduleTimerStartSignal();
    /** @brief */
    void PlayScheduleTimerStopSignal();

private slots:
    /** @brief */
    void AddPlayScheduleListSlot(const QVector<ScheduleJson::PlaySchedule>& src);

    /** @brief */
    void PlayScheduleTimerStartSlot();
    /** @brief */
    void PlayScheduleTimerStopSlot();

    /** @brief */
    void PlayScheduleTimeout();

private:
    /** @brief
     *          return pos
     *          -1 == pos, Not Existed
     */
    int CheckDuplicatedScheduleId(const QString& id);

    /** @brief */
    bool CheckValidPlaySchedule(const QDateTime& currentDateTime,
                                const ScheduleJson::Cron& cron);


    /** @brief */
    QString ConvertDateTimeToString(const QDateTime& src);

private:
    bool m_bIsActive;

    std::pair<int, PlayJson::PlayDataType> m_playingData;
    std::pair<int, PlayJson::PlayDataType> m_prevSinglePlay;

    QString m_currScheduleId;
    QVector<ScheduleJson::PlaySchedule> m_playScheduleList;
};

#endif // PLAYSCHEDULETIMER_H
