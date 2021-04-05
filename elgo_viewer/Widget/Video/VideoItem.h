#ifndef VIDEOITEM_H
#define VIDEOITEM_H

// QT
#include <QObject>
#include <QGraphicsVideoItem>
#include <QMediaPlayer>
#include <QBuffer>
#include <QByteArray>

// Viewer
#include "Widget/Definition/ContentsDef.h"

class VideoItem : public QGraphicsVideoItem
{
    Q_OBJECT
public:
    explicit VideoItem(QGraphicsItem *parent = nullptr);
    ~VideoItem();

    /** @brief */
    bool SetVideoFileToBuffer(const QString& path, const VideoInfo::MetaData& metaData);
    /** @brief */
    void SetVideoPosAndSize(const StyleSheet::PosSizeInfo& posSizeInfo);

    /** @brief */
    void PlayVideoItem();
    /** @brief */
    void StopVideoItem();

    /** @brief */
    QString GetVideoFileName();

    /** @brief */
    qint64 GetDuration();

private slots:
    /** @brief */
    void CheckMediaStatus(QMediaPlayer::MediaStatus status);

    /** @brief */
    void CheckStateChanged(QMediaPlayer::State state);

    /** @brief */
    void CheckPositionChanged(qint64 pos);

private:
    QMediaPlayer *m_player;

    QByteArray *m_bytes;
    QBuffer *m_buffer;

    VideoInfo::MetaData m_videoInfo;
    StyleSheet::PosSizeInfo m_posSizeInfo;
};

#endif // VIDEOITEM_H
