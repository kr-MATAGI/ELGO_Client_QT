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
    bool SetVideoContentToBuffer(const QString& path, const Video::Duration& duration);

    /** @brief */
    void ShowVideoItemToScreen();

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
    Video::ContentInfo m_videoInfo;
};

#endif // VIDEOITEM_H
