#ifndef SINGLEPLAYTIMER_H
#define SINGLEPLAYTIMER_H

// QT
#include <QTimer>

// Viewer
#include "Widget/Definition/ScheduleTimerDef.h"
#include "Widget/ContentsPlayer.h"

class ContentsPlayer;

class SinglePlayTimer : public QTimer
{
    Q_OBJECT
public:
    explicit SinglePlayTimer(QObject *parent = nullptr);
    ~SinglePlayTimer();

public:
    /** @brief */
    void AddPlayData(const PlayJson::CustomPlayDataJson& src);
    /** @brief */
    void AddPlayData(const PlayJson::FixedPlayDataJson& src);

    /** @brief */
    void ExecPlayData(ScheduleTimer::PlayDataInfo& playDataInfo);
    /** @brief */
    void UpdateCustomPlayDataIndexInfo(const ScheduleTimer::PlayDataIndexInfo& playDataIndexInfo);

private:
    /** @brief */
    void MakeCustomPlayDataContents(ScheduleTimer::PlayDataIndexInfo& playDataIdxInfo);
    /** @brief */
    void MakeFixedPlayDataContents(ScheduleTimer::PlayDataIndexInfo& playDataIdxInfo);

    /** @brief */
    void UpdatePlayerNewScene(ScheduleTimer::PlayDataIndexInfo& playDataIdxInfo);

private slots:
    /** @brief */
    void SinglePlayTimeout();

private:
    PlayJson::PlayDataType m_currPlayDataType;
    ScheduleTimer::PlayDataIndexInfo m_customPlayDataInfo;
    ScheduleTimer::FixedPlayIndexInfo m_fixedPlayDataInfo;
    ScheduleTimer::CountdownInfo m_countdownInfo;

    QVector<PlayJson::CustomPlayDataJson> m_customPlayDataList;
    QVector<PlayJson::FixedPlayDataJson> m_fixedPlayDataList;
};

#endif // SINGLEPLAYTIMER_H
