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
    void ExecPlayData(const SchedulerDef::PlayDataInfo& playDataInfo);

private:
    /** @brief */
    void MakeCustomPlayDataContents(const SchedulerDef::PlayDataInfo& playDataInfo, QGraphicsScene& scene);
    /** @brief */
    void MakeFixedPlayDataContents(const SchedulerDef::PlayDataInfo& playDataInfo, QGraphicsScene& scene);

    /** @brief */
    void MakeFileTypeContent(const SchedulerDef::ContentIndxInfo& contentIndxInfo,
                             const PlayJson::ContentData& contentData,
                             const StyleSheet::PosSizeInfo& posSizeInfo,
                             QGraphicsScene& scene);
    /** @brief */
    void MakeWidgetTypeContent(const SchedulerDef::ContentIndxInfo& contentIndxInfo,
                               const PlayJson::ContentData& contentData,
                               const StyleSheet::PosSizeInfo& posSizeInfo,
                               QGraphicsScene& scene);

    /** @brief */
    void ManageContentItemList();


    /** @brief */
    QString ConvertMediaTypeEnumToString(const PlayJson::MediaType type);

private slots:
    /** @brief */
    void SinglePlayTimeout();

private:
    SchedulerDef::PlayDataInfo m_currPlayInfo;
    SchedulerDef::ContentIndxInfo m_currCountDownInfo;

    QVector<PlayJson::CustomPlayDataJson> m_customPlayDataList;
    QVector<PlayJson::FixedPlayDataJson> m_fixedPlayDataList;

    QVector<SceneInfo> m_sceneList;
    QVector<ImageItemInfo> m_imageItemList;
    QVector<VideoItemInfo> m_videoItemList;
};

#endif // SINGLEPLAYTIMER_H
