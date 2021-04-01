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
    m_player->setNotifyInterval(100);

    // connect
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
    delete m_bytes;
    m_bytes = NULL;

    delete m_buffer;
    m_buffer = NULL;

    delete m_player;
    m_player = NULL;
}

//========================================================
bool VideoItem::SetVideoFileToBuffer(const QString& path, const VideoInfo::MetaData& metaData)
//========================================================
{
    m_videoInfo = metaData;

    bool retValue = true;
    QFile mediaFile(path);

    if(false == mediaFile.open(QIODevice::ReadOnly))
    {
        retValue = false;
        ELGO_VIEWER_LOG("Error - Not opend video item : %s", path.toStdString().c_str());
    }
    else
    {
        m_bytes = new QByteArray;
        m_bytes->append(mediaFile.readAll());

        m_buffer = new QBuffer(m_bytes);
        m_buffer->open(QIODevice::ReadOnly);

        m_player->setMedia(QMediaContent(), m_buffer);
        mediaFile.close();
    }    

    return retValue;
}

//========================================================
void VideoItem::SetVideoPosAndSize(const StyleSheet::WidgetInfo& widgetInfo)
//========================================================
{
    m_widgetInfo = widgetInfo;

    this->setPos(m_widgetInfo.pos);
    this->setSize(m_widgetInfo.size);
    ELGO_VIEWER_LOG("name : %s { pos : %f,%f, size : %d x %d }",
                    widgetInfo.fileName.toUtf8().constData(),
                    widgetInfo.pos.x(), widgetInfo.pos.y(),
                    widgetInfo.size.width(), widgetInfo.size.height());
}

//========================================================
void VideoItem::PlayVideoItem()
//========================================================
{
    m_player->play();
}

//========================================================
void VideoItem::StopVideoItem()
//========================================================
{
    m_player->stop();
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
