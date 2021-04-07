#ifndef SINGLEPLAYTIMER_H
#define SINGLEPLAYTIMER_H

// QT
#include <QTimer>

// Viewer
#include "ScheduleTimerDef.h"
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
    void ExecPlayData(SchedulerDef::PlayDataInfo& playDataInfo);

private:
    /** @brief */
    void MakeCustomPlayDataContents(SchedulerDef::PlayDataIndexInfo& playDataIdxInfo);
    /** @brief */
    void MakeFixedPlayDataContents(SchedulerDef::PlayDataIndexInfo& playDataIdxInfo);

    /** @brief */
    void UpdatePlayerNewScene(SchedulerDef::PlayDataIndexInfo& playDataIdxInfo);

private slots:
    /** @brief */
    void SinglePlayTimeout();

private:
    SchedulerDef::PlayDataIndexInfo m_customPlayDataInfo;
    SchedulerDef::CurrFixedPlayInfo m_fixedPlayDataInfo;
    SchedulerDef::CountdownInfo m_countdownInfo;

    QVector<PlayJson::CustomPlayDataJson> m_customPlayDataList;
    QVector<PlayJson::FixedPlayDataJson> m_fixedPlayDataList;
};

#endif // SINGLEPLAYTIMER_H
