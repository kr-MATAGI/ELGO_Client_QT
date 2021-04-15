#ifndef CONTENTSPLAYER_H
#define CONTENTSPLAYER_H

// QT
#include <QWidget>

// Viewer
#include "Definition/ContentsDef.h"
#include "ViewerCtrl/ViewerController.h"
#include "Common/Interface/ScheduleDef.h"
#include "SchedulesTimer/SinglePlayTimer.h"
#include "SchedulesTimer/SchedulesTimer.h"

class ViewerController;
class SchedulesTimer;
class SinglePlayTimer;

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

public:
    /** @brief */
    bool GetCurrentWidgetCapture();

    /** @brief */
    void StartContentsPlayer();
    /** @brief */
    void StartScheduleTimer();

signals:
    /** @note   Related to Schedule Timer */
    /** @brief */
    void AddPlayDataSignal(PlayJson::CustomPlayDataJson src);
    /** @brief */
    void AddPlayDataSignal(PlayJson::FixedPlayDataJson src);

    /** @brief */
    void AddPlayScheduleListSignal(ScheduleJson::PlaySchedules src);

    /** @brief */
    void ExecPlayDataSingal(PlayJson::PlayData playData);

    /** @brief */
    void MakeFileTypeItemSignal(ScheduleTimer::PlayDataIndexInfo contentIndexInfo,
                                PlayJson::ContentData contentData,
                                StyleSheet::PosSizeInfo posSizeInfo);
    /** @brief */
    void MakeWidgetTypeItemSingal(ScheduleTimer::PlayDataIndexInfo contentIndexInfo,
                                  PlayJson::ContentData contentData,
                                  StyleSheet::PosSizeInfo posSizeinfo);

    /** @brief */
    void MakeSubtitleWidgetSignal(ScheduleTimer::PlayDataIndexInfo contentIndexInfo,
                                  PlayJson::SubtitleData subtitleData);

    /** @brief */
    void UpdatePlayerNewCustomSceneSignal(ScheduleTimer::PlayDataIndexInfo playDataIdxInfo);
    /** @brief */
    void PausePrevPlayDataSignal(ScheduleTimer::PlayDataIndexInfo prevPlayDataIdxInfo);

    /** @brief */
    void UpdatePlayerNewFixedSceneSignal(ScheduleTimer::PlayDataIndexInfo playDataIdxInfo, const int layerCount);
    /** @brief */
    void UpdatePlayerFixedLayerContentSignal(ScheduleTimer::PlayDataIndexInfo prevDataIdxInfo,
                                       ScheduleTimer::PlayDataIndexInfo newDataIdxInfo);

private slots:
    /** @note   Related to Scheduler */
    /** @brief */
    void AddPlayDataSlot(PlayJson::CustomPlayDataJson src);
    /** @brief */
    void AddPlayDataSlot(PlayJson::FixedPlayDataJson src);

    /** @brief */
    void AddPlayScheduleListSlot(ScheduleJson::PlaySchedules src);

    /** @brief */
    void ExecPlayDataSlot(PlayJson::PlayData playData);

    /** @brief */
    void MakeFileTypeItemSlot(ScheduleTimer::PlayDataIndexInfo contentIndxInfo,
                              PlayJson::ContentData contentData,
                              StyleSheet::PosSizeInfo posSizeInfo);
    /** @brief */
    void MakeWidgetTypeItemSlot(ScheduleTimer::PlayDataIndexInfo contentIndexInfo,
                                  PlayJson::ContentData contentData,
                                  StyleSheet::PosSizeInfo posSizeinfo);

    /** @brief */
    void MakeSubtitleWidgetSlot(ScheduleTimer::PlayDataIndexInfo contentIndexInfo,
                                PlayJson::SubtitleData subtitleData);

    /** @brief */
    void UpdatePlayerNewCustomSceneSlot(ScheduleTimer::PlayDataIndexInfo playDataIdxInfo);
    /** @brief */
    void PausePrevPlayDataSlot(ScheduleTimer::PlayDataIndexInfo prevPlayDataIdxInfo);

    /** @brief */
    void UpdatePlayerNewFixedSceneSlot(ScheduleTimer::PlayDataIndexInfo playDataIdxInfo, const int layerCount);
    /** @brief */
    void UpdatePlayerFixedLayerContentSlot(ScheduleTimer::PlayDataIndexInfo prevDataIdxInfo,
                                       ScheduleTimer::PlayDataIndexInfo newDataIdxInfo);

private:
    /** @brief */
    void SearchItemAndAddToScene(const ScheduleTimer::PlayDataIndexInfo& playDataIdxInfo,
                                                  QGraphicsScene* scene);

    /** @brief */
    void ExecPlayDataItemList(const ScheduleTimer::PlayDataIndexInfo& playDataIdxInfo);

    /** @brief */
    void ClearPrevPlayDataInfo(const ScheduleTimer::PlayDataInfo& exceptPlayDataInfo);
    /** @brief */
    void ClearPrevSceneList(const ScheduleTimer::PlayDataInfo& exceptPlayDataInfo);

    /** @note   Simple Utils */
    QString ConvertMediaTypeEnumToString(const PlayJson::MediaType type);

private:
    Ui::ContentsPlayer *ui;
    QSize m_displaySize;

    SchedulesTimer *m_schedulerTimer;
    SinglePlayTimer *m_singleTimer;

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
