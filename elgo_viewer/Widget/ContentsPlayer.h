#ifndef CONTENTSPLAYER_H
#define CONTENTSPLAYER_H

// QT
#include <QWidget>
#include <QTimer>

// Viewer
#include "Definition/ContentsDef.h"
#include "Common/Interface/ScheduleJsonDef.h"
#include "Widget/Definition/ScheduleTimerDef.h"

namespace Ui {
class ContentsPlayer;
}

class ContentsPlayer : public QWidget
{
    Q_OBJECT

public:
    /** @brief */
    static ContentsPlayer *pInstance;

    /** @brief */
    explicit ContentsPlayer(QWidget *parent = nullptr);
    /** @brief */
    ~ContentsPlayer();

    /** @brief */
    static ContentsPlayer* GetInstance();
    /** @brief */
    void DestoryInstance();

signals:
    /** @brief  Show Contents Player (Signal) */
    void StartContentsPlayerSignal();


    /** @brief  Add Play Schedule (Signal) */
    void AddPlayScheduleListSignal(const QVector<ScheduleJson::PlaySchedule>& src);

    /** @brief  Add Custom PlayData Info (Signal) */
    void AddPlayDataSignal(const PlayJson::CustomPlayDataJson& src);
    /** @brief  Add Fxied PlayData Info (Signal) */
    void AddPlayDataSignal(const PlayJson::FixedPlayDataJson& src);

    /** @brief  Exec PlayData (Signal) */
    void ExecPlayDataSignal(const PlayJson::PlayData& playData);

private slots:
    /** @brief  Show Contents Player (Slot) */
    void StartContentsPlayerSlot();


    /** @brief  Add Play Schedule (Slot) */
    void AddPlayScheduleListSlot(const QVector<ScheduleJson::PlaySchedule>& src);

    /** @brief  Add Custom PlayData Info (Slot) */
    void AddPlayDataSlot(const PlayJson::CustomPlayDataJson& src);
    /** @brief  Add Fxied PlayData Info (Slot) */
    void AddPlayDataSlot(const PlayJson::FixedPlayDataJson& src);

    /** @brief  Exec PlayData (Slot) */
    void ExecPlayDataSlot(const PlayJson::PlayData& playData);

private:
    /** @@brief   Simple Utils */
    QString ConvertMediaTypeEnumToString(const PlayJson::MediaType type);

private:
    Ui::ContentsPlayer *ui;
    QRect m_screenRect;

    // Schedule timer
    QTimer m_scheduleTimer;

    // PlaySchedule
    QVector<ScheduleJson::PlaySchedule> m_playScheduleList;

    // For playing countdown
    ScheduleTimer::PlayingIndex m_playingIndex;
    ScheduleTimer::CountdownInfo m_playCountdown;

    // PlayData List
    QVector<PlayJson::CustomPlayDataJson> m_customPlayDataList;
    QVector<PlayJson::FixedPlayDataJson> m_fixedPlayDataList;

    // Saved play data item list
    QVector<SceneInfo> m_sceneList;
    QVector<ImageItemInfo> m_imageItemList;
    QVector<VideoItemInfo> m_videoItemList;
    QVector<ClockWidgetInfo> m_clockWidgetList;
    QVector<DateWidgetInfo> m_dateWidgetList;
    QVector<NewsFeedWidgetInfo> m_newsFeedWigetList;
    QVector<WeatherWidgetInfo> m_weatherWidgetList;
    QVector<SubtitleWidgetInfo> m_subtitleWidgetList;
    QVector<ProxyWidgetInfo> m_proxyWidgetList;
};

#endif // CONTENTSPLAYER_H
