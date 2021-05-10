// QT
#include <QFile>

// Viewer
#include "VideoItem.h"
#include "Logger/ViewerLogger.h"

//========================================================
VideoItem::VideoItem(QGraphicsItem *parent)
    : QGraphicsVideoItem(parent)
    , m_bIsPlaying(false)
//========================================================
{
    // init
    m_player = new QMediaPlayer(this, QMediaPlayer::StreamPlayback);
    m_player->setVideoOutput(this);
    m_player->setNotifyInterval(100);

    this->setAspectRatioMode(Qt::IgnoreAspectRatio);

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

    m_mediaBuffer->deleteLater();

    delete m_mediaBytes;
    m_mediaBytes = NULL;
}

//========================================================
void VideoItem::SetVideoFile(const QString& path, const VideoInfo::MetaData& metaData)
//========================================================
{
    m_videoInfo = metaData;

    QFile mediafile(path);
    mediafile.open(QIODevice::ReadOnly);

    m_mediaBytes = new QByteArray;
    m_mediaBytes->append(mediafile.readAll());

    m_mediaBuffer = new QBuffer(m_mediaBytes);
    m_mediaBuffer->open(QIODevice::ReadOnly);
    m_mediaBuffer->reset();

    m_player->setMedia(QMediaContent(), m_mediaBuffer);

    ELGO_VIEWER_LOG("Set Video File : %s", path.toUtf8().constData());
}

//========================================================
void VideoItem::SetVideoPosAndSize(const StyleSheet::PosSizeInfo& posSizeInfo)
//========================================================
{
    m_posSizeInfo = posSizeInfo;

    this->setPos(m_posSizeInfo.pos);
    this->setSize(m_posSizeInfo.size);
    ELGO_VIEWER_LOG("name : %s, posSize{ x: %d, y:%d, w: %d, h: %d }",
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
    m_bIsPlaying = true;
}

//========================================================
void VideoItem::StopVideoItem()
//========================================================
{
    ELGO_VIEWER_LOG("Stop Video : %s, duration {file : %lld, user : %lld }",
                    m_videoInfo.fileName.toUtf8().constData(),
                    m_videoInfo.duration.file, m_videoInfo.duration.user);
    m_player->stop();
    m_bIsPlaying = false;
}

//========================================================
void VideoItem::PauseVideoItem()
//========================================================
{
    ELGO_VIEWER_LOG("Pause Video : %s, duration {file : %lld, user : %lld }",
                    m_videoInfo.fileName.toUtf8().constData(),
                    m_videoInfo.duration.file, m_videoInfo.duration.user);
    m_player->pause();
    m_bIsPlaying = false;
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
bool VideoItem::IsPlayingVideo()
//========================================================
{
    return m_bIsPlaying;
}

//========================================================
void VideoItem::CheckMediaPlayerError(QMediaPlayer::Error error)
//========================================================
{
    /**
     *  NoError = 0,
     *  ResourceError = 1,
     *  FormatError = 2,
     *  NetworkError = 3,
     *  AccessDeniedError = 4,
     *  ServiceMissingError = 5,
     *  MediaIsPlaylist = 6
     */
    QString errorStr;
    if(QMediaPlayer::Error::NoError == error)
    {
        errorStr = "No Error";
    }
    else if(QMediaPlayer::Error::ResourceError == error)
    {
        errorStr = "Resource Error";
    }
    else if(QMediaPlayer::Error::FormatError == error)
    {
        errorStr = "FormatError Error";
    }
    else if(QMediaPlayer::Error::NetworkError == error)
    {
        errorStr = "NetworkError Error";
    }
    else if(QMediaPlayer::Error::AccessDeniedError == error)
    {
        errorStr = "AccessDeniedError Error";
    }
    else if(QMediaPlayer::Error::ServiceMissingError == error)
    {
        errorStr = "ServiceMissingError Error";
    }
    else if(QMediaPlayer::Error::MediaIsPlaylist == error)
    {
        errorStr = "MediaIsPlaylist Error";
    }

    ELGO_VIEWER_LOG("Media player ERROR %s - %s",
                    m_videoInfo.fileName.toStdString().c_str(),
                    error);
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
    QString statusStr;
    if(QMediaPlayer::MediaStatus::UnknownMediaStatus == status)
    {
        statusStr = "Unknown Status";
    }
    else if(QMediaPlayer::MediaStatus::NoMedia == status)
    {
        statusStr = "No Media Status";
    }
    else if(QMediaPlayer::MediaStatus::LoadingMedia == status)
    {
        statusStr = "Loading Media Status";
    }
    else if(QMediaPlayer::MediaStatus::LoadedMedia == status)
    {
        statusStr = "Loaded Media Status";
    }
    else if(QMediaPlayer::MediaStatus::StalledMedia == status)
    {
        statusStr = "Stalled Media Status";
    }
    else if(QMediaPlayer::MediaStatus::BufferingMedia == status)
    {
        statusStr = "Buffering Media Status";
    }
    else if(QMediaPlayer::MediaStatus::BufferedMedia == status)
    {
        statusStr = "Buffered Media Status";
    }
    else if(QMediaPlayer::MediaStatus::InvalidMedia == status)
    {
        statusStr = "Invalid Media Status";
    }

    ELGO_VIEWER_LOG("%s - %s",
                    m_videoInfo.fileName.toStdString().c_str(),
                    statusStr.toStdString().c_str());
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
    QString stateStr;
    if(QMediaPlayer::State::StoppedState == state)
    {
        stateStr = "Stop State";
    }
    else if(QMediaPlayer::State::PlayingState == state)
    {
        stateStr = "Playing State";
    }
    else if(QMediaPlayer::State::PausedState == state)
    {
        stateStr = "Paused State";
    }
    else
    {
        ELGO_VIEWER_LOG("Unknwon State: %d", state);
    }
    ELGO_VIEWER_LOG("%s - %s",
                    m_videoInfo.fileName.toStdString().c_str(),
                    stateStr.toStdString().c_str());
}

//========================================================
void VideoItem::CheckPositionChanged(qint64 pos)
//========================================================
{
    const qint64 secUnit = 1000 * (m_videoInfo.duration.file / 1000);
    if(pos >= secUnit)
    {
        ELGO_VIEWER_LOG("Replay - %s (0:%lld) ",
                        m_videoInfo.fileName.toStdString().c_str(),
                        secUnit)
        m_player->setPosition(0);
    }
}
