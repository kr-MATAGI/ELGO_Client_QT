#ifndef PLAYTIMER_H
#define PLAYTIMER_H

// QT
#include <QTimer>
#include <QVector>
#include <QGraphicsScene>

// Viewer
#include "Logger/ViewerLogger.h"
#include "Widget/ContentsPlayer.h"

#include "Widget/Definition/ContentsDef.h"
#include "Widget/Image/ImageItem.h"
#include "Widget/Video/VideoItem.h"

// Common
#include "Common/Interface/ContentsPlayDataDef.h"
#include "Common/Interface/ScheduleDef.h"

/** @brief  std::pair<PlayDataId, PlayJson::PlayDataType> */
typedef std::pair<int, PlayJson::PlayDataType> PlayDataInfo;
typedef std::pair<PlayDataInfo, QGraphicsScene *> SceneInfo;
typedef std::pair<PlayDataInfo, VideoItem *> VideoItemInfo;
typedef std::pair<PlayDataInfo, ImageItem *> ImageItemInfo;

class ContentsPlayer;

class SchedulesTimer : public QTimer
{
    Q_OBJECT
public:
    /** @brief */
    static SchedulesTimer *pInstance;

    /** @brief */
    SchedulesTimer(QObject *parent = nullptr);
    /** @brief */
    virtual ~SchedulesTimer();

    /** @brief */
    static SchedulesTimer* GetInstance();
    /** @brief */
    void DestoryInstance();

public:
    /** @brief */
    void AddPlayDataList(const PlayJson::FixedPlayDataJson& src, const bool bIsSiglePlay = false);
    /** @brief */
    void AddPlayDataList(const PlayJson::CustomPlayDataJson& src, const bool bIsSiglePlay = false);
    /** @brief */
    void AddPlaySchedule(const ScheduleJson::PlaySchedules& src);

private:
    /** @brief */
    void ExecSchedule(const QString& scheduleId, const PlayDataInfo& playDataInfo, QGraphicsScene& scene);
    /** @brief */
    void ExecSinglePlay();
    /** @brief */
    void SetPlayDataContentToScene(const PlayDataInfo& playDataInfo, QGraphicsScene& scene);
    /** @brief */
    void UpdatePlayerScene(const PlayDataInfo& playDataInfom, const bool bIsSchedule = false);

    /** @brief */
    void AddFileContentData(const PlayDataInfo& playDataInfo, const PlayJson::ContentData& contentData,
                            const StyleSheet::WidgetInfo& widgetInfo, QGraphicsScene& scene);
    /** @brief */
    void AddWidgetContentData(const PlayDataInfo& playDataInfo, const PlayJson::ContentData& contentData,
                              const StyleSheet::WidgetInfo& widgetInfo, QGraphicsScene& scene);
    /** @brief */
    void AddSubtitleData(const PlayDataInfo& playDataInfo, const PlayJson::SubtitleData& subtitleData, QGraphicsScene& scene);


    /** @brief */
    void ClearAllSchedulesBySinglePlay();

    /** @brief */
    void CheckPlayDateTimeSchedules(const QDateTime& currentDateTime);
    /** @brief */
    bool IsValidCronRuleValue(const QDateTime& currentDateTime, const ScheduleJson::Cron& cron);


    /** @brief */
    QString ConvertDateTimeFormat(const QDateTime& src);
    /** @brief */
    QString ConvertMediaTypeEnumToString(const PlayJson::MediaType type);

private slots:
    /** @brief */
    void SchedulerTimeoutSlot();

private:
    bool m_bIsSchedule;
    bool m_bRecvSingePlay;
    QString m_currentScheduleId;
    PlayDataInfo m_currentPlayData;
    PlayDataInfo m_prevPlayData;
    PlayDataInfo m_prevSinglePlayData;
    int m_timeCount;

    QVector<PlayJson::FixedPlayDataJson> m_fixedPlayDataList;
    QVector<PlayJson::CustomPlayDataJson> m_customPlayDataList;
    QVector<ScheduleJson::PlaySchedules> m_playScheduleList;

    QVector<SceneInfo> m_sceneList;
    QVector<ImageItemInfo> m_imageItemList;
    QVector<VideoItemInfo> m_videoItemList;
};

#endif // PLAYTIMER_H
