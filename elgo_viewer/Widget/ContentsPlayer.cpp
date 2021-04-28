// QT
#include <QScreen>
#include <QWindow>

// Viewer
#include "ContentsPlayer.h"
#include "ui_ContentsPlayer.h"
#include "Logger/ViewerLogger.h"

// Common
#include "Common/CommonDef.h"

ContentsPlayer* ContentsPlayer::pInstance = nullptr;

//========================================================
ContentsPlayer::ContentsPlayer(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ContentsPlayer)
//========================================================
{
    // init
    ui->setupUi(this);
    QList<QScreen *> screens = QApplication::screens();
    if(0 < screens.size())
    {
        QSize screenSize = screens[0]->size();
        const QRect screenRect = QRect(QPoint(0.0, 0.0), screenSize);

        m_screenRect = screenRect;
        ELGO_VIEWER_LOG("screenRect - {x: %d, y: %d}, {w: %d, y: %d}",
                        screenRect.x(), screenRect.y(),
                        screenRect.width(), screenRect.height());
    }

    ui->playerView->setGeometry(m_screenRect);
    ui->playerView->setStyleSheet("border: 0px");
    ui->playerView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->playerView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->playerView->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    ui->playerView->setRenderHint(QPainter::Antialiasing);

    // Register meta type
    qRegisterMetaType<ScheduleJson::PlaySchedule>("ScheduleJson::PlaySchedule");
    qRegisterMetaType<QVector<ScheduleJson::PlaySchedule>>("QVector<ScheduleJson::PlaySchedule>");

    qRegisterMetaType<PlayJson::CustomPlayDataJson>("PlayJson::CustomPlayDataJson");
    qRegisterMetaType<PlayJson::FixedPlayDataJson>("PlayJson::FixedPlayDataJson");
    qRegisterMetaType<PlayJson::PlayData>("PlayJson::PlayData");

    // Connect Singal and Slot
    connect(this, &ContentsPlayer::StartContentsPlayerSignal,
            this, &ContentsPlayer::StartContentsPlayerSlot);

    connect(this, &ContentsPlayer::AddPlayScheduleListSignal,
            this, &ContentsPlayer::AddPlayScheduleListSlot);

    connect(this, SIGNAL(AddPlayDataSignal(const PlayJson::CustomPlayDataJson&)),
            this, SLOT(AddPlayDataSlot(const PlayJson::CustomPlayDataJson&)));
    connect(this, SIGNAL(AddPlayDataSignal(const PlayJson::FixedPlayDataJson&)),
            this, SLOT(AddPlayDataSlot(const PlayJson::FixedPlayDataJson&)));

    connect(this, &ContentsPlayer::ExecPlayDataSignal,
            this, &ContentsPlayer::ExecPlayDataSlot);


}

//========================================================
ContentsPlayer::~ContentsPlayer()
//========================================================
{
    delete ui;
}

//========================================================
ContentsPlayer* ContentsPlayer::GetInstance()
//========================================================
{
    if(nullptr == pInstance)
    {
        pInstance = new ContentsPlayer();
    }
    return pInstance;
}

//========================================================
void ContentsPlayer::DestoryInstance()
//========================================================
{
    if(nullptr != pInstance)
    {
        delete pInstance;
        pInstance = NULL;
    }
}

//========================================================
void ContentsPlayer::StartContentsPlayerSlot()
//========================================================
{
    ELGO_VIEWER_LOG("Start Contents Player");
    this->showFullScreen();
}

//========================================================
void ContentsPlayer::AddPlayScheduleListSlot(const QVector<ScheduleJson::PlaySchedule>& src)
//========================================================
{
    const int srcSize = src.size();
    for(int idx = 0; idx < srcSize; idx++)
    {
        m_playScheduleList.push_back(src[idx]);
        ELGO_VIEWER_LOG("ADD PlaySchedule - { id: %d }", src[idx].id.toStdString().c_str());
    }
}

//========================================================
void ContentsPlayer::AddPlayDataSlot(const PlayJson::CustomPlayDataJson& src)
//========================================================
{
    if( m_customPlayDataList.end() == std::find(m_customPlayDataList.begin(),
                                                m_customPlayDataList.end(),
                                                src) )
    {
        m_customPlayDataList.push_back(src);
        ELGO_VIEWER_LOG("ADD PlayData - {id: %d, type: %d}",
                        src.playData.id, src.playData.playDataType);
    }
    else
    {
        ELGO_VIEWER_LOG("Existed Data - {id: %d, type: %d}",
                        src.playData.id, src.playData.playDataType);
    }
}

//========================================================
void ContentsPlayer::AddPlayDataSlot(const PlayJson::FixedPlayDataJson& src)
//========================================================
{
    if( m_fixedPlayDataList.end() == std::find(m_fixedPlayDataList.begin(),
                                               m_fixedPlayDataList.end(),
                                               src) )
    {
        m_fixedPlayDataList.push_back(src);
        ELGO_VIEWER_LOG("ADD PlayData - {id: %d, type: %d}",
                        src.playData.id, src.playData.playDataType);
    }
    else
    {
        ELGO_VIEWER_LOG("Existed Data - {id: %d, type: %d}",
                        src.playData.id, src.playData.playDataType);
    }
}

//========================================================
void ContentsPlayer::ExecPlayDataSlot(const PlayJson::PlayData& playData)
//========================================================
{
    m_scheduleTimer.stop();

    if( (playData.id != m_playingIndex.playDataInfo.id) &&
        (playData.playDataType != m_playingIndex.playDataInfo.type) )
    {
        ELGO_VIEWER_LOG("Read to Play - {id: %d, type: %d}",
                        playData.id, playData.playDataType);

        // Make Custom or Fixed Contents
        if(PlayJson::PlayDataType::CUSTOM == playData.playDataType)
        {

        }
        else
        {

        }
    }
    else
    {
        ELGO_VIEWER_LOG("Already Playing - {id: %d, type: %d}",
                        playData.id, playData.playDataType);
    }

    m_scheduleTimer.start();
}

//========================================================
QString ContentsPlayer::ConvertMediaTypeEnumToString(const PlayJson::MediaType type)
//========================================================
{
    QString retValue;

    if(PlayJson::MediaType::VIDEO == type)
    {
        retValue = "video/";
    }
    else if(PlayJson::MediaType::IMAGE == type)
    {
        retValue = "image/";
    }
    else if(PlayJson::MediaType::WEATHER == type)
    {
        retValue = "icon/";
    }
    else
    {
        ELGO_VIEWER_LOG("Error - Unkwon MediaType : %d", type);
    }

    return retValue;
}
