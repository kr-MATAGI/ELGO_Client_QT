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
    void StartPlayTimer();
    /** @brief */
    void StopPlayTimer();
    /** @brief */
    bool IsActivePlayTimer();

private slots:
    /** @brief */
    void PlayScheduleTimeout();

private:
    /** @brief
     *          return pos
     *          -1 == pos, Not Existed
     */
    int CheckDuplicatedScheduleId(const QString& id);

    /** @brief */
    bool CheckValidDateTimeCron(const QDateTime& currentDateTime,
                                const ScheduleJson::Cron& cron);


    /** @brief */
    QString ConvertDateTimeToString(const QDateTime& src);

private:
    bool m_bIsActive;

    int m_prevPlayDataId;
    PlayJson::PlayDataType m_prevPlayDataType;
    QString m_currScheduleId;
    QVector<ScheduleJson::PlaySchedule> m_playScheduleList;
};

#endif // PLAYSCHEDULETIMER_H
