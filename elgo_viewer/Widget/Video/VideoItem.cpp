// QT
#include <QFile>

// Viewer
#include "VideoItem.h"
#include "Logger/ViewerLogger.h"

//========================================================
VideoItem::VideoItem(QGraphicsItem *parent)
    : QGraphicsVideoItem(parent)
//========================================================
{
    // init
    m_player = new QMediaPlayer(this, QMediaPlayer::StreamPlayback);
    m_player->setVideoOutput(this);

    // connect
    connect(m_player, SIGNAL(mediaStatusChanged(QMediaPlayer::MediaStatus)),
            this, SLOT(CheckMediaStatus(QMediaPlayer::MediaStatus)));
    connect(m_player, SIGNAL(stateChanged(QMediaPlayer::State)),
            this, SLOT(CheckStateChanged(QMeidaPlayer::State)));
    connect(m_player, SIGNAL(positionChanged(qint64)),
            this, SLOT(CheckPositionChanged(qint64)));
}

//========================================================
VideoItem::~VideoItem()
//========================================================
{
    delete m_bytes;
    m_bytes = NULL;

    delete m_buffer;
    m_buffer = NULL;

    delete m_player;
    m_player = NULL;
}

//========================================================
bool VideoItem::SetVideoContentToBuffer(const QString& path, const Video::Duration& duration)
//========================================================
{
    bool retValue = true;
    QFile mediaFile(path);

    if(false == mediaFile.open(QIODevice::ReadOnly))
    {
        retValue = false;
        ELGO_VIEWER_LOG("Error - Not opend video item : %s", path.toStdString().c_str());
    }
    else
    {
        QString fileName = QString(path).split("/", Qt::SkipEmptyParts).back();
        m_videoInfo.fileName = fileName;
        m_videoInfo.duration = duration;

        m_bytes = new QByteArray;
        m_bytes->append(mediaFile.readAll());

        m_buffer = new QBuffer(m_bytes);
        m_buffer->open(QIODevice::ReadOnly);

        m_player->setMedia(QMediaContent(), m_buffer);
    }

    mediaFile.close();

    return retValue;
}

//========================================================
void VideoItem::ShowVideoItemToScreen()
//========================================================
{
    m_player->play();
}

//========================================================
void VideoItem::CheckMediaStatus(QMediaPlayer::MediaStatus status)
//========================================================
{
    /**
     *  @note
     *          UnknownMediaStatus = 0
     *          NoMedia = 1
     *          LoadingMedia = 2
     *          LoadedMedia = 3
     *          StalledMedia = 4
     *          BufferingMedia = 5
     *          BufferedMedia = 6
     *          EndOfMedia = 7
     *          InvalidMedia = 8
     */
    ELGO_VIEWER_LOG("%s - %d", m_videoInfo.fileName.toStdString().c_str(), status)
}

//========================================================
void VideoItem::CheckStateChanged(QMediaPlayer::State state)
//========================================================
{
    /**
      * @note
      *         StoppedState = 0
      *         PlayingState = 1
      *         PausedState = 2
      */
    ELGO_VIEWER_LOG("%s - %d", m_videoInfo.fileName.toStdString().c_str(), state);
}

//========================================================
void VideoItem::CheckPositionChanged(qint64 pos)
//========================================================
{

}
