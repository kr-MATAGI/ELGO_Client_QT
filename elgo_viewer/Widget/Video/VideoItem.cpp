// Viewer
#include "VideoItem.h"
#include "Logger/ViewerLogger.h"

//========================================================
VideoItem::VideoItem(QGraphicsItem *parent)
    : QGraphicsVideoItem(parent)
//========================================================
{
    // init
    m_player = new QMediaPlayer(this);
    m_player->setVideoOutput(this);
    m_player->setNotifyInterval(100);

    // connect
    connect(m_player, SIGNAL(error(QMediaPlayer::Error)),
           this, SLOT(CheckMediaPlayerError(QMediaPlayer::Error)));
    connect(m_player, SIGNAL(mediaStatusChanged(QMediaPlayer::MediaStatus)),
            this, SLOT(CheckMediaStatus(QMediaPlayer::MediaStatus)));
    connect(m_player, SIGNAL(stateChanged(QMediaPlayer::State)),
            this, SLOT(CheckStateChanged(QMediaPlayer::State)));
    connect(m_player, SIGNAL(positionChanged(qint64)),
            this, SLOT(CheckPositionChanged(qint64)));
}

//========================================================
VideoItem::~VideoItem()
//========================================================
{
    delete m_player;
    m_player = NULL;
}

//========================================================
void VideoItem::SetVideoFile(const QString& path, const VideoInfo::MetaData& metaData)
//========================================================
{
    m_videoInfo = metaData;
    m_player->setMedia(QUrl::fromLocalFile(path));
    ELGO_VIEWER_LOG("Set Video File : %s", path.toUtf8().constData());
}

//========================================================
void VideoItem::SetVideoPosAndSize(const StyleSheet::PosSizeInfo& posSizeInfo)
//========================================================
{
    m_posSizeInfo = posSizeInfo;

    this->setPos(m_posSizeInfo.pos);
    this->setSize(m_posSizeInfo.size);
    ELGO_VIEWER_LOG("name : %s { pos : %f,%f, size : %d x %d }",
                    m_videoInfo.fileName.toUtf8().constData(),
                    m_posSizeInfo.pos.x(), m_posSizeInfo.pos.y(),
                    m_posSizeInfo.size.width(), m_posSizeInfo.size.height());
}

//========================================================
void VideoItem::PlayVideoItem()
//========================================================
{
    ELGO_VIEWER_LOG("Play Video : %s, duration {file : %lld, user : %lld }",
                    m_videoInfo.fileName.toUtf8().constData(),
                    m_videoInfo.duration.file, m_videoInfo.duration.user);
    m_player->play();
}

//========================================================
void VideoItem::StopVideoItem()
//========================================================
{
    m_player->stop();
}

//========================================================
QString VideoItem::GetVideoFileName()
//========================================================
{
    return m_videoInfo.fileName;
}

//========================================================
qint64 VideoItem::GetDuration()
//========================================================
{
    return m_player->duration();
}

//========================================================
void VideoItem::CheckMediaPlayerError(QMediaPlayer::Error error)
//========================================================
{
    ELGO_VIEWER_LOG("Media player ERROR - %d", error);
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
    if(pos >= m_videoInfo.duration.file)
    {
        m_player->setPosition(0);
    }
}
