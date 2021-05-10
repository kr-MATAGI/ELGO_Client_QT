#ifndef CONTENTSPLAYER_H
#define CONTENTSPLAYER_H

// QT
#include <QWidget>
#include <QTimer>

// Viewer
#include "Definition/ContentsDef.h"
#include "Definition/ViewerDefinition.h"
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

    /** @brief  Add Custom PlayData Info (Signal) */
    void AddPlayDataSignal(const PlayJson::CustomPlayDataJson& src);
    /** @brief  Add Fxied PlayData Info (Signal) */
    void AddPlayDataSignal(const PlayJson::FixedPlayDataJson& src);

    /** @brief  Exec PlayData (Signal) */
    void ExecPlayDataSignal(const PlayJson::PlayData& playData,
                            const bool bDelPrevData = false);

    /** @brief */
    void RotateScreenSignal(const VIEWER_DEF::HEADING heading);

    /** @brief */
    void UpdatePlayerPauseSignal(const bool bIsPause);

private slots:
    /** @brief  Show Contents Player (Slot) */
    void StartContentsPlayerSlot();

    /** @brief  Add Custom PlayData Info (Slot) */
    void AddPlayDataSlot(const PlayJson::CustomPlayDataJson& src);
    /** @brief  Add Fxied PlayData Info (Slot) */
    void AddPlayDataSlot(const PlayJson::FixedPlayDataJson& src);


    /** @brief  Exec PlayData (Slot) */
    void ExecPlayDataSlot(const PlayJson::PlayData& playData,
                          const bool bDelPrevData = false);

    /** @brief */
    void RotateScreenSlot(const VIEWER_DEF::HEADING heading);

    /** @brief */
    void UpdatePlayerPauseSlot(const bool bIsPause);

    /** @brief  Timer's timeout slot */
    void PlayerTimeout();

private:
    /** @brief  Simple Utils */
    void ConvertMediaTypeEnumToString(const PlayJson::MediaType src, QString& dest);

    /**
     *  @note
     *          Compare Content Playing Index Info
     *          ture == same, false == diff
     */
    bool ComparePlayingIndex(const PlayScheduleTimer::PlayingIndex& lhs,
                             const PlayScheduleTimer::PlayingIndex& rhs);


    /** @brief  Update Display Scene */
    void UpdatePlayerScene(const PlayScheduleTimer::PlayingIndex& playingIndex,
                           const bool bDelPrevData = false);
    /** @brief  Update Next Fixed Content */
    void UpdateNextFixedLayerContent(const PlayScheduleTimer::PlayingIndex& prevPlayingIndex,
                                     const PlayScheduleTimer::PlayingIndex& nextPlayingIndex);

    /** @brief  Add Content to Scene */
    void SearchContentAndAddToScene(const PlayScheduleTimer::PlayingIndex& playingIndex,
                                    QGraphicsScene* dest);

    /** @brief  Make Countdown Info */
    void MakePlayDataCountdownInfo(const PlayScheduleTimer::PlayingIndex& playingIndex);
    /** @brief  */
    void GetSuitablePlayDataIndex(const PlayJson::PlayData& playData);


    /** @brief  Make Custom PlayData Item/Widget */
    void MakeCustomPlayDataContents(const PlayScheduleTimer::PlayingIndex& playingIndex,
                                    QGraphicsScene* dest);
    /** @brief  Make Fixed PlayData Item/Widget */
    void MakeFixedPlayDataContents(const PlayScheduleTimer::PlayingIndex& playingIndex,
                                   QGraphicsScene* dest);

    /** @brief  Make Image or Video Content */
    void MakeFileTypeContent(const PlayScheduleTimer::PlayingIndex& playingIndex,
                             const PlayJson::ContentData& contentData,
                             const StyleSheet::PosSizeInfo& posSize,
                             QGraphicsScene *dest);

    /** @brief  Make Widget Content */
    void MakeWidgetTypeContent(const PlayScheduleTimer::PlayingIndex& playingIndex,
                               const PlayJson::ContentData& contentData,
                               const StyleSheet::PosSizeInfo& posSize,
                               QGraphicsScene *dest);

    /** @brief  Make Subtitle Content */
    void MakeSubtitleTypeContent(const PlayScheduleTimer::PlayingIndex& playingIndex,
                                 const PlayJson::SubtitleData& subtitleData,
                                 QGraphicsScene *dest);


    /** @brief  Play Item and Widget */
    void PlayItemAndWidgetContents(const PlayScheduleTimer::PlayingIndex& playingIndex);
    /** @brief  Pause Item and Widget */
    void PauseItemAndWidgetContents(const PlayScheduleTimer::PlayingIndex& playingIndex,
                                    const bool bIsFixedTimeout = false);
    /** @brief  Remove Item and Widget from Scene */
    void RemoveItemAndWidgetFromScene(const PlayScheduleTimer::PlayingIndex& playingIndex,
                                      const bool bIsFixedTimeout = false);

    /** @brief  Clear Prev PlayData Json */
    void ClearOtherPlayDataJsonInfo(const PlayScheduleTimer::PlayingIndex& playingIndex);
    /** @brief  Clear Prev PlayData Item */
    void ClearOtherPlayDataItem(const PlayScheduleTimer::PlayingIndex& playingIndex);
    /** @brief  Clear Prev Playing Data */
    void ClearPrevPlayingData(const PlayScheduleTimer::PlayingIndex& playingIndex,
                              const bool bIsFixedTimeout = false);

private:
    Ui::ContentsPlayer *ui;
    QRect m_screenRect;
    VIEWER_DEF::HEADING m_heading;

    // Schedule timer
    QTimer m_playerTimer;

    // For playing countdown
    PlayScheduleTimer::PlayingIndex m_playingIndex;
    PlayScheduleTimer::CountdownInfo m_playCountdown;

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
