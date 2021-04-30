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
    void ExecPlayDataSignal(const PlayJson::PlayData& playData,
                            const bool bDelPrevData = false);

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
    void ExecPlayDataSlot(const PlayJson::PlayData& playData,
                          const bool bDelPrevData = false);

    /** @brief  Timer's timeout slot */
    void PlayerTimeout();

private:
    /** @brief  Simple Utils */
    void ConvertMediaTypeEnumToString(const PlayJson::MediaType src, QString& dest);
    /** @brief
     *          Compare Content Playing Index Info
     *          ture == same, false == diff
     */
    bool ComparePlayingIndex(const ScheduleTimer::PlayingIndex& lhs,
                             const ScheduleTimer::PlayingIndex& rhs);


    /** @brief  Update Display Scene */
    void UpdatePlayerScene(const ScheduleTimer::PlayingIndex& playingIndex,
                           const bool bDelPrevData = false);
    /** @brief  Update Next Fixed Content */
    void UpdateNextFixedLayerContent(const ScheduleTimer::PlayingIndex& prevPlayingIndex,
                                     const ScheduleTimer::PlayingIndex& nextPlayingIndex);

    /** @brief  Add Content to Scene */
    void SearchContentAndAddToScene(const ScheduleTimer::PlayingIndex& playingIndex,
                                    QGraphicsScene* dest);

    /** @brief  Make Countdown Info */
    void MakePlayDataCountdownInfo(const ScheduleTimer::PlayingIndex& playingIndex);
    /** @brief  */
    void GetSuitablePlayDataIndex(const PlayJson::PlayData& playData);


    /** @brief  Make Custom PlayData Item/Widget */
    void MakeCustomPlayDataContents(const ScheduleTimer::PlayingIndex& playingIndex,
                                    QGraphicsScene* dest);
    /** @brief  Make Fixed PlayData Item/Widget */
    void MakeFixedPlayDataContents(const ScheduleTimer::PlayingIndex& playingIndex,
                                   QGraphicsScene* dest);

    /** @brief  Make Image or Video Content */
    void MakeFileTypeContent(const ScheduleTimer::PlayingIndex& playingIndex,
                             const PlayJson::ContentData& contentData,
                             const StyleSheet::PosSizeInfo& posSize,
                             QGraphicsScene *dest);

    /** @brief  Make Widget Content */
    void MakeWidgetTypeContent(const ScheduleTimer::PlayingIndex& playingIndex,
                               const PlayJson::ContentData& contentData,
                               const StyleSheet::PosSizeInfo& posSize,
                               QGraphicsScene *dest);

    /** @brief  Make Subtitle Content */
    void MakeSubtitleTypeContent(const ScheduleTimer::PlayingIndex& playingIndex,
                                 const PlayJson::SubtitleData& subtitleData,
                                 QGraphicsScene *dest);


    /** @brief  Play Item and Widget */
    void PlayItemAndWidgetContents(const ScheduleTimer::PlayingIndex& playingIndex);
    /** @brief  Pause Item and Widget */
    void PauseItemAndWidgetContents(const ScheduleTimer::PlayingIndex& playingIndex);
    /** @brief  Remove Item and Widget from Scene */
    void RemoveItemAndWidgetFromScene(const ScheduleTimer::PlayingIndex& playingIndex);

    /** @brief  Clear Prev PlayData Json */
    void ClearOtherPlayDataJsonInfo(const ScheduleTimer::PlayingIndex& playingIndex);
    /** @brief  Clear Prev PlayData Item */
    void ClearOtherPlayDataItem(const ScheduleTimer::PlayingIndex& playingIndex);
    /** @brief  Clear Prev Playing Data */
    void ClearPrevPlayingData(const ScheduleTimer::PlayingIndex& playingIndex);

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
    int m_playDataIndex;
    QVector<PlayJson::CustomPlayDataJson> m_customPlayDataList;
    QVector<PlayJson::FixedPlayDataJson> m_fixedPlayDataList;

    // Saved play data item list
    SceneInfo m_currScene;
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
