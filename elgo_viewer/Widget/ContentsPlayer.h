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

public:
    /** @note   Related to Scheduler */
    /** @brief */
    void AddPlayDataList(const PlayJson::CustomPlayDataJson& src);
    /** @brief */
    void AddPlayDataList(const PlayJson::FixedPlayDataJson& src);
    /** @brief */
    void ExecSinglePlayData(const PlayJson::PlayData& src);

    /** @brief */
    void UpdatePlayerNewCustomScene(const SchedulerDef::PlayDataIndexInfo& playDataIdxInfo);
    /** @brief */
    void UpdatePlayerNewFixedScene(SchedulerDef::PlayDataIndexInfo& playDataIdxInfo, const int layerCount);
    /** @brief */
    void UpdatePlayerFixedLayerContent(const SchedulerDef::PlayDataIndexInfo& prevDataIdxInfo,
                                       const SchedulerDef::PlayDataIndexInfo& newDataIdxInfo);

    /** @brief */
    void MakeFileTypeItem(const SchedulerDef::PlayDataIndexInfo& contentIndxInfo,
                          const PlayJson::ContentData& contentData,
                          const StyleSheet::PosSizeInfo& posSizeInfo);


    /** @brief */
    void SearchItemAndAddToScene(const SchedulerDef::PlayDataIndexInfo& playDataIdxInfo,
                                                  QGraphicsScene* scene);

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
    // QVector<Widget> m_widgetItemList;
    // QVector<Subtitle> m_subtitleItemList;
};

#endif // CONTENTSPLAYER_H
