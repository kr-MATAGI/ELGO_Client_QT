#ifndef SINGLEPLAYTIMER_H
#define SINGLEPLAYTIMER_H

// QT
#include <QTimer>

// Viewer
#include "SchedulerDef.h"
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
    void MakeFileTypeContent(const SchedulerDef::PlayDataIndexInfo& contentIndxInfo,
                             const PlayJson::ContentData& contentData,
                             const StyleSheet::PosSizeInfo& posSizeInfo);
    /** @brief */
    void MakeWidgetTypeContent(const SchedulerDef::PlayDataIndexInfo& contentIndxInfo,
                               const PlayJson::ContentData& contentData,
                               const StyleSheet::PosSizeInfo& posSizeInfo);

    /** @brief */
    void UpdatePlayerNewScene(SchedulerDef::PlayDataIndexInfo& playDataIdxInfo);
    /** @brief */
    void UpdateFixedContentToScene(const SchedulerDef::PlayDataIndexInfo& prevDataIdxInfo,
                                   const SchedulerDef::PlayDataIndexInfo& newDataIdxInfo);
    /** @brief */
    void SearchItemAndAddToScene(const SchedulerDef::PlayDataIndexInfo& playDataIdxInfo, QGraphicsScene* scene);


    /** @brief */
    QString ConvertMediaTypeEnumToString(const PlayJson::MediaType type);

private slots:
    /** @brief */
    void SinglePlayTimeout();

private:
    SchedulerDef::PlayDataIndexInfo m_customPlayDataInfo;
    SchedulerDef::CurrFixedPlayInfo m_fixedPlayDataInfo;
    SchedulerDef::CountdownInfo m_countdownInfo;

    QVector<PlayJson::CustomPlayDataJson> m_customPlayDataList;
    QVector<PlayJson::FixedPlayDataJson> m_fixedPlayDataList;

    QVector<SceneInfo> m_sceneList;
    QVector<ImageItemInfo> m_imageItemList;
    QVector<VideoItemInfo> m_videoItemList;
    // QVector<Widget> m_widgetItemList;
    // QVector<Subtitle> m_subtitleItemList;
};

#endif // SINGLEPLAYTIMER_H
