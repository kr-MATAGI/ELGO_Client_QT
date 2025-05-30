#ifndef DOWNLOADTHREAD_H
#define DOWNLOADTHREAD_H

// QT
#include <QString>
#include <QRunnable>

// Control
#include "DownloadDef.h"
#include "Common/Interface/ContentsPlayDataDef.h"
#include "NetworkCtrl/NetworkController.h"

class NetworkController;

class DownloadThread : public QRunnable
{
public:
    DownloadThread();
    ~DownloadThread();

    /** @brief  QRunnable run() override */
    void run() override;

    /** @brief */
    void ExecDownloadSinglePlayData();
    /** @brief */
    void ExecDownloadPlaySchedule();
    /** @brief */
    void ExecDownloadPowerSchedule();

    /** @brief */
    bool SearchCustomDataWidgetType(const int playDataId, const PlayJson::PlayDataType playType,
                                    QVector<PlayJson::PageData>& pageDataList,
                                    QVector<PlayJson::UpdateWidgetInfo>& updateWidgetList);
    /** @brief */
    bool SearchFixedDataWidgetType(const int playDataId, const PlayJson::PlayDataType playType,
                                   QVector<PlayJson::FixedLayerData>& layerDataList,
                                   QVector<PlayJson::UpdateWidgetInfo>& updateWidgetList);
    /** @brief */
    bool DownloadAdditionalWidgetInfo(PlayJson::ContentData& contentData);


    /** @brief */
    void SetContentSchema(const ContentSchema::Summary& src);
    /** @brief */
    void SetDownloadAction(const DownloadDef::Action value);
    /** @brief */
    void SetDownloadBytes(const QByteArray& src);

private:
    ContentSchema::Summary m_serverJson;
    DownloadDef::Action m_action;
    QByteArray m_bytes;
};

#endif // DOWNLOADTHREAD_H
