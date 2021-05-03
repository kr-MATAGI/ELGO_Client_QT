#ifndef PLAYSCHEDULETIMER_H
#define PLAYSCHEDULETIMER_H

// QT
#include <QTimer>

// Common
#include "Common/Interface/ScheduleJsonDef.h"

// Main
#include "MainCtrl/MainController.h"

class MainController;

class PlayScheduleTimer : public QTimer
{
    Q_OBJECT
public:
    /** @brief */
    explicit PlayScheduleTimer(QObject *parent=nullptr);
    /** @brief */
    virtual ~PlayScheduleTimer();

    /** @brief */
    void AddPlayScheduleList(const QVector<ScheduleJson::PlaySchedule>& src);

    /** @brief */
    void ClearAllPlayScheduleList();
    /** @brief */
    void ClearPlayScheduleById(const QString& id);

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
    /** @brief */
    bool CheckValidPlayScheduleId(const QString& id);

    /** @brief */
    bool CheckValidDateTimeCron(const QDateTime& currentDateTime,
                                const ScheduleJson::Cron& cron);


    /** @brief */
    QString ConvertDateTimeToString(const QDateTime& src);

private:
    bool m_bIsActive;

    QString m_currScheduleId;
    QVector<ScheduleJson::PlaySchedule> m_playScheduleList;
};

#endif // PLAYSCHEDULETIMER_H
