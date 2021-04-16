// QT
#include <QScreen>
#include <QWindow>

// Viewer
#include "ContentsPlayer.h"
#include "ui_ContentsPlayer.h"
#include "Logger/ViewerLogger.h"

// Widget
#include "Widget/Definition/ContentsDef.h"
#include "Widget/Clock/ClockWidget.h"
#include "Widget/Date/DateWidget.h"
#include "Widget/News/NewsFeedWidget.h"
#include "Widget/Weather/WeatherWidget.h"

// Common
#include "Common/CommonDef.h"

ContentsPlayer* ContentsPlayer::pInstance = nullptr;

Q_DECLARE_METATYPE(PlayJson::CustomPlayDataJson);
Q_DECLARE_METATYPE(PlayJson::FixedPlayDataJson);
Q_DECLARE_METATYPE(PlayJson::PlayData);
Q_DECLARE_METATYPE(PlayJson::SubtitleData);
Q_DECLARE_METATYPE(ScheduleJson::PlaySchedule);

Q_DECLARE_METATYPE(ScheduleTimer::PlayDataIndexInfo);
Q_DECLARE_METATYPE(PlayJson::ContentData);
Q_DECLARE_METATYPE(StyleSheet::PosSizeInfo);
Q_DECLARE_METATYPE(StyleSheet::StyleInfo);

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
        m_displaySize = screens[0]->size();
        ELGO_VIEWER_LOG("Screen Size {w : %d, h : %d}", m_displaySize.width(), m_displaySize.height());
    }

    // timer init
    m_schedulerTimer = new SchedulesTimer;
    m_singleTimer = new SinglePlayTimer;

    const QRect screenRect = QRect(QPoint(0.0, 0.0), m_displaySize);
    ui->playerView->setGeometry(screenRect);
    ui->playerView->setStyleSheet("border: 0px");
    ui->playerView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->playerView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->playerView->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    ui->playerView->setRenderHint(QPainter::Antialiasing);

    // connect
    qRegisterMetaType<PlayJson::CustomPlayDataJson>("PlayJson::CustomPlayDataJson");
    qRegisterMetaType<PlayJson::FixedPlayDataJson>("PlayJson::FixedPlayDataJson");
    qRegisterMetaType<PlayJson::PlayData>("PlayJson::PlayData");
    qRegisterMetaType<PlayJson::SubtitleData>("PlayJson::SubtitleData");
    qRegisterMetaType<ScheduleJson::PlaySchedule>("ScheduleJson::PlaySchedules");

    qRegisterMetaType<ScheduleTimer::PlayDataIndexInfo>("ScheduleTimer::PlayDataIndexInfo");
    qRegisterMetaType<PlayJson::ContentData>("PlayJson::ContentData");
    qRegisterMetaType<StyleSheet::PosSizeInfo>("StyleSheet::PosSizeInfo");
    qRegisterMetaType<StyleSheet::StyleInfo>("StyleSheet::StyleInfo");

    connect(this, SIGNAL(AddPlayDataSignal(PlayJson::CustomPlayDataJson)),
            this, SLOT(AddPlayDataSlot(PlayJson::CustomPlayDataJson)));

    connect(this, SIGNAL(AddPlayDataSignal(PlayJson::FixedPlayDataJson)),
            this, SLOT(AddPlayDataSlot(PlayJson::FixedPlayDataJson)));

    connect(this, SIGNAL(AddPlayScheduleListSignal(QVector<ScheduleJson::PlaySchedules>)),
            this, SLOT(AddPlayScheduleListSlot(QVector<ScheduleJson::PlaySchedules>)));

    connect(this, SIGNAL(ExecPlayDataSignal(PlayJson::PlayData)),
            this, SLOT(ExecPlayDataSlot(PlayJson::PlayData)));

    connect(this, SIGNAL(ClearPlayDataSignal()),
            this, SLOT(ClearPlayDataSlot()));

    connect(this, SIGNAL(MakeFileTypeItemSignal(ScheduleTimer::PlayDataIndexInfo,
                                                PlayJson::ContentData,
                                                StyleSheet::PosSizeInfo)),
            this, SLOT(MakeFileTypeItemSlot(ScheduleTimer::PlayDataIndexInfo,
                                            PlayJson::ContentData,
                                            StyleSheet::PosSizeInfo)));

    connect(this, SIGNAL(MakeWidgetTypeItemSingal(ScheduleTimer::PlayDataIndexInfo,
                                                  PlayJson::ContentData,
                                                  StyleSheet::PosSizeInfo)),
            this, SLOT(MakeWidgetTypeItemSlot(ScheduleTimer::PlayDataIndexInfo,
                                              PlayJson::ContentData,
                                              StyleSheet::PosSizeInfo)));

    connect(this, SIGNAL(MakeSubtitleWidgetSignal(ScheduleTimer::PlayDataIndexInfo,
                                                  PlayJson::SubtitleData)),
            this, SLOT(MakeSubtitleWidgetSlot(ScheduleTimer::PlayDataIndexInfo,
                                              PlayJson::SubtitleData)));

    connect(this, SIGNAL(UpdatePlayerNewCustomSceneSignal(ScheduleTimer::PlayDataIndexInfo)),
            this, SLOT(UpdatePlayerNewCustomSceneSlot(ScheduleTimer::PlayDataIndexInfo)));

    connect(this, SIGNAL(PausePrevPlayDataSignal(ScheduleTimer::PlayDataIndexInfo)),
            this, SLOT(PausePrevPlayDataSlot(ScheduleTimer::PlayDataIndexInfo)));

    connect(this, SIGNAL(UpdatePlayerNewFixedSceneSignal(ScheduleTimer::PlayDataIndexInfo, const int)),
            this, SLOT(UpdatePlayerNewFixedSceneSlot(ScheduleTimer::PlayDataIndexInfo, const int)));

    connect(this, SIGNAL(UpdatePlayerFixedLayerContentSignal(ScheduleTimer::PlayDataIndexInfo, ScheduleTimer::PlayDataIndexInfo)),
            this, SLOT(UpdatePlayerFixedLayerContentSlot(ScheduleTimer::PlayDataIndexInfo, ScheduleTimer::PlayDataIndexInfo)));
}

//========================================================
ContentsPlayer::~ContentsPlayer()
//========================================================
{
    delete ui;

    delete m_schedulerTimer;
    m_schedulerTimer = NULL;

    delete m_singleTimer;
    m_singleTimer = NULL;
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
bool ContentsPlayer::GetCurrentWidgetCapture()
//========================================================
{
    bool retValue = true;

    QScreen *screen = QGuiApplication::primaryScreen();

    if(const QWindow *window = windowHandle())
    {
        screen = window->screen();
    }
    else if(nullptr == screen)
    {
        retValue = false;
        ELGO_VIEWER_LOG("ERROR - NULL == screen");
    }
    else
    {
        const WId wid = QWidget::winId();
        QString path = CAPATURE_SAVE_PATH;
        path.append("screenCapture.png");
        QPixmap pixmap = screen->grabWindow(wid);
        if(false == pixmap.save(path))
        {
            retValue = false;
            ELGO_VIEWER_LOG("ERROR - Not saved pixmap { path : %s }", path.toStdString().c_str());
        }
        else
        {
            ELGO_VIEWER_LOG("Saved Pixmap { path : %s }", path.toStdString().c_str());
        }
    }

    return retValue;
}

//========================================================
void ContentsPlayer::StartContentsPlayer()
//========================================================
{
    ELGO_VIEWER_LOG("Start Contents Player");
    this->showFullScreen();
}

//========================================================
void ContentsPlayer::AddPlayDataSlot(PlayJson::CustomPlayDataJson src)
//========================================================
{
    m_singleTimer->AddPlayData(src);
}

//========================================================
void ContentsPlayer::AddPlayDataSlot(PlayJson::FixedPlayDataJson src)
//========================================================
{
    m_singleTimer->AddPlayData(src);
}

//========================================================
void ContentsPlayer::AddPlayScheduleListSlot(QVector<ScheduleJson::PlaySchedule> src)
//========================================================
{
    m_schedulerTimer->AddPlaySchedule(src);
}

//========================================================
void ContentsPlayer::ClearPrevPlayDataInfo(const ScheduleTimer::PlayDataInfo& exceptPlayDataInfo)
//========================================================
{
    // image
    const int prevImageListSize = m_imageItemList.size();

    QVector<ImageItemInfo>::iterator imageIter = m_imageItemList.begin();
    for(; imageIter != m_imageItemList.end(); )
    {
        if( (exceptPlayDataInfo.id != imageIter->first.playDataInfo.id) ||
            (exceptPlayDataInfo.type != imageIter->first.playDataInfo.type) )
        {
            imageIter->second->deleteLater();
            imageIter = m_imageItemList.erase(imageIter);
        }
        else
        {
            ++imageIter;
        }
    }

    // video
    const int prevVideoListSize = m_videoItemList.size();

    QVector<VideoItemInfo>::iterator videoIter = m_videoItemList.begin();
    for( ; videoIter != m_videoItemList.end(); )
    {
        if( (exceptPlayDataInfo.id != videoIter->first.playDataInfo.id) ||
            (exceptPlayDataInfo.type != videoIter->first.playDataInfo.type) )
        {
            videoIter->second->StopVideoItem();

            videoIter->second->deleteLater();
            videoIter = m_videoItemList.erase(videoIter);
        }
        else
        {
            ++videoIter;
        }
    }

    // clock
    const int prevClockListSize = m_clockWidgetList.size();

    QVector<ClockWidgetInfo>::iterator clockIter = m_clockWidgetList.begin();
    for( ; clockIter != m_clockWidgetList.end(); )
    {
        if( (exceptPlayDataInfo.id != clockIter->first.playDataInfo.id) ||
            (exceptPlayDataInfo.type != clockIter->first.playDataInfo.type) )
        {
            clockIter->second->StopClock();

            clockIter->second->deleteLater();
            clockIter = m_clockWidgetList.erase(clockIter);
        }
        else
        {
            ++clockIter;
        }
    }

    // date
    const int prevDateListSize = m_dateWidgetList.size();

    QVector<DateWidgetInfo>::iterator dateIter = m_dateWidgetList.begin();
    for( ; dateIter != m_dateWidgetList.end(); )
    {
        if( (exceptPlayDataInfo.id != dateIter->first.playDataInfo.id) ||
            (exceptPlayDataInfo.type != dateIter->first.playDataInfo.type) )
        {
            dateIter->second->StopDateWidget();

            dateIter->second->deleteLater();
            dateIter = m_dateWidgetList.erase(dateIter);
        }
        else
        {
            ++dateIter;
        }
    }

    // news
    const int prevNewsListSize = m_newsFeedWigetList.size();

    QVector<NewsFeedWidgetInfo>::iterator newsIter = m_newsFeedWigetList.begin();
    for( ; newsIter != m_newsFeedWigetList.end(); )
    {
        if( (exceptPlayDataInfo.id != newsIter->first.playDataInfo.id) ||
            (exceptPlayDataInfo.type != newsIter->first.playDataInfo.type) )
        {
            newsIter->second->StopAnimation();

            newsIter->second->deleteLater();
            newsIter  = m_newsFeedWigetList.erase(newsIter);
        }
        else
        {
           ++newsIter;
        }
    }

    // weather
    const int prevWeatherListSize = m_weatherWidgetList.size();

    QVector<WeatherWidgetInfo>::iterator weatherIter = m_weatherWidgetList.begin();
    for( ; weatherIter != m_weatherWidgetList.end(); )
    {
        if( (exceptPlayDataInfo.id != weatherIter->first.playDataInfo.id) ||
            (exceptPlayDataInfo.type != weatherIter->first.playDataInfo.type))
        {
            weatherIter->second->StopDateTimeTimer();

            weatherIter->second->deleteLater();
            weatherIter = m_weatherWidgetList.erase(weatherIter);
        }
        else
        {
            ++weatherIter;
        }
    }

    // subtitle
    const int prevSubtitleListSize = m_subtitleWidgetList.size();

    QVector<SubtitleWidgetInfo>::iterator subIter = m_subtitleWidgetList.begin();
    for( ; subIter != m_subtitleWidgetList.end(); )
    {
        if( (exceptPlayDataInfo.id != subIter->first.playDataInfo.id) ||
            (exceptPlayDataInfo.type != subIter->first.playDataInfo.type) )
        {
            subIter->second->deleteLater();
            subIter = m_subtitleWidgetList.erase(subIter);
        }
        else
        {
            ++subIter;
        }
    }

    // proxy
    const int prevProxyListSize = m_proxyWidgetList.size();

    QVector<ProxyWidgetInfo>::iterator proxyIter = m_proxyWidgetList.begin();
    for( ; proxyIter != m_proxyWidgetList.end(); )
    {
        if( (exceptPlayDataInfo.id != proxyIter->first.second.playDataInfo.id) ||
            (exceptPlayDataInfo.type != proxyIter->first.second.playDataInfo.type) )
        {
            proxyIter->second->deleteLater();
            proxyIter = m_proxyWidgetList.erase(proxyIter);
        }
        else
        {
            ++proxyIter;
        }
    }

    ELGO_VIEWER_LOG("{image - %d : %d, video - %d : %d, clock - %d : %d, date - %d : %d}",
                    prevImageListSize, m_imageItemList.size(),
                    prevVideoListSize, m_videoItemList.size(),
                    prevClockListSize, m_clockWidgetList.size(),
                    prevDateListSize, m_dateWidgetList.size());
    ELGO_VIEWER_LOG("{news - %d : %d, weather - %d : %d, subtitle - %d : %d, proxy - %d : %d}",
                    prevNewsListSize, m_newsFeedWigetList.size(),
                    prevWeatherListSize, m_weatherWidgetList.size(),
                    prevSubtitleListSize, m_subtitleWidgetList.size(),
                    prevProxyListSize, m_proxyWidgetList.size());
}

//========================================================
void ContentsPlayer::ClearPrevSceneList(const ScheduleTimer::PlayDataInfo& exceptPlayDataInfo)
//========================================================
{
    ClearPrevPlayDataInfo(exceptPlayDataInfo);

    const int prevSceneListSize = m_sceneList.size();

    QVector<SceneInfo>::iterator sceneIter = m_sceneList.begin();
    for(; sceneIter != m_sceneList.end(); )
    {
        if( (exceptPlayDataInfo.id != sceneIter->first.playDataInfo.id) ||
            (exceptPlayDataInfo.type != sceneIter->first.playDataInfo.type) )
        {
            ELGO_VIEWER_LOG("Erase Scene : {id: %d, type: %d}",
                             sceneIter->first.playDataInfo.id,
                             sceneIter->first.playDataInfo.type);

            sceneIter->second->deleteLater();
            sceneIter = m_sceneList.erase(sceneIter);
        }
        else
        {
            ++sceneIter;
        }
    }

    ELGO_VIEWER_LOG("Clear Scene {%d -> %d}", prevSceneListSize, m_sceneList.size());
}

//========================================================
void ContentsPlayer::ExecPlayDataSlot(PlayJson::PlayData src)
//========================================================
{
    m_singleTimer->stop();

    ScheduleTimer::PlayDataInfo playDataInfo;
    playDataInfo.id = src.id;
    playDataInfo.type = src.playDataType;

    m_singleTimer->ExecPlayData(playDataInfo);

    // Prev Scene Delete
    ClearPrevSceneList(playDataInfo);

    m_singleTimer->start(990);
}

//========================================================
void ContentsPlayer::ClearPlayDataSlot()
//========================================================
{
    m_singleTimer->ClearPlayData();
}

//========================================================
void ContentsPlayer::UpdatePlayerNewCustomSceneSlot(ScheduleTimer::PlayDataIndexInfo playDataIdxInfo)
//========================================================
{
    if(PlayJson::PlayDataType::CUSTOM == playDataIdxInfo.playDataInfo.type)
    {
        bool bIsFindScene = false;

        // Searching Scene
        QVector<SceneInfo>::iterator sceneIter = m_sceneList.begin();
        for(; sceneIter != m_sceneList.end(); ++sceneIter)
        {
            if(sceneIter->first == playDataIdxInfo)
            {
                bIsFindScene = true;
                ELGO_VIEWER_LOG("Success Find Scene {id: %d, pageIdx: %d, itemSize: %d}",
                                sceneIter->first.playDataInfo.id, sceneIter->first.pageIdx,
                                sceneIter->second->items().size());

                ui->playerView->setScene(sceneIter->second);
                ExecPlayDataItemList(playDataIdxInfo);

                break;
            }
        }

        if(false == bIsFindScene)
        {
            QGraphicsScene *newScene = new QGraphicsScene(this);
            newScene->setSceneRect(ui->playerView->rect());

            SearchItemAndAddToScene(playDataIdxInfo, newScene);
            ELGO_VIEWER_LOG("Made New Custom Scene {id: %d, pageIdx: %d, itemSize: %d}",
                            playDataIdxInfo.playDataInfo.id, playDataIdxInfo.pageIdx, newScene->items().size());

            ui->playerView->setScene(newScene);
            ExecPlayDataItemList(playDataIdxInfo);

            SceneInfo newSceneInfo;
            newSceneInfo.first = playDataIdxInfo;
            newSceneInfo.second = newScene;

            m_sceneList.push_back(newSceneInfo);
            ELGO_VIEWER_LOG("SceneList Size : %d", m_sceneList.size());
        }

        m_singleTimer->UpdateCustomPlayDataIndexInfo(playDataIdxInfo);
    }
    else
    {
        ELGO_VIEWER_LOG("ERROR - Not Custom Play Data Type");
    }
}

//========================================================
void ContentsPlayer::UpdatePlayerNewFixedSceneSlot(ScheduleTimer::PlayDataIndexInfo playDataIdxInfo, const int layerCount)
//========================================================
{
    if(PlayJson::PlayDataType::FIXED == playDataIdxInfo.playDataInfo.type)
    {
        QGraphicsScene *newScene = new QGraphicsScene;
        newScene->setSceneRect(ui->playerView->rect());

        for(int idx =0; idx < layerCount; idx++)
        {
            SearchItemAndAddToScene(playDataIdxInfo, newScene);
            ExecPlayDataItemList(playDataIdxInfo);
            playDataIdxInfo.layerIdx++;
        }
        ui->playerView->setScene(newScene);

        SceneInfo newSceneInfo;
        newSceneInfo.first = playDataIdxInfo;
        newSceneInfo.second = newScene;

        m_sceneList.push_back(newSceneInfo);
        ELGO_VIEWER_LOG("SceneList Size : %d", m_sceneList.size());
    }
    else
    {
        ELGO_VIEWER_LOG("ERROR - Not Fixed Play Data type");
    }
}

//========================================================
void ContentsPlayer::UpdatePlayerFixedLayerContentSlot(ScheduleTimer::PlayDataIndexInfo prevDataIdxInfo,
                                                       ScheduleTimer::PlayDataIndexInfo newDataIdxInfo)
//========================================================
{
    // Find Scene
    QVector<SceneInfo>::iterator sceneIter = m_sceneList.begin();
    for(; sceneIter != m_sceneList.end(); ++sceneIter)
    {
        if( (sceneIter->first.playDataInfo.id == newDataIdxInfo.playDataInfo.id) &&
            (sceneIter->first.playDataInfo.type == newDataIdxInfo.playDataInfo.type) )
        {
            // Add / Remove Image
            QVector<ImageItemInfo>::iterator imageIter = m_imageItemList.begin();
            for(; imageIter != m_imageItemList.end(); ++imageIter)
            {
                if(newDataIdxInfo == imageIter->first)
                {
                    ui->playerView->scene()->addItem(imageIter->second);

                    ELGO_VIEWER_LOG("ADD Image Item to Scene : %s, Index {layer: %d, content: %d}",
                                    imageIter->second->GetImageFileName().toUtf8().constData(),
                                    newDataIdxInfo.layerIdx, newDataIdxInfo.contentIdx);
                }

                if(prevDataIdxInfo == imageIter->first)
                {
                    ui->playerView->scene()->removeItem(imageIter->second);

                    ELGO_VIEWER_LOG("Remove Image Item to Scene : %s, Index {layer: %d, content: %d}",
                                    imageIter->second->GetImageFileName().toUtf8().constData(),
                                    prevDataIdxInfo.layerIdx, prevDataIdxInfo.contentIdx);
                }
            }

            // ADD / Remove Video
            QVector<VideoItemInfo>::iterator videoIter = m_videoItemList.begin();
            for(; videoIter != m_videoItemList.end(); ++videoIter)
            {
                if(newDataIdxInfo == videoIter->first)
                {
                    ui->playerView->scene()->addItem(videoIter->second);
                    ELGO_VIEWER_LOG("ADD Video Item to Scene : %s, Index {layer: %d, content: %d}",
                                    videoIter->second->GetVideoFileName().toUtf8().constData(),
                                    newDataIdxInfo.layerIdx, newDataIdxInfo.contentIdx);
                }

                if(prevDataIdxInfo == videoIter->first)
                {
                    ui->playerView->scene()->removeItem(videoIter->second);
                    ELGO_VIEWER_LOG("Remove Video Item to Scene : %s, Index {layer: %d, content: %d}",
                                    videoIter->second->GetVideoFileName().toUtf8().constData(),
                                    prevDataIdxInfo.layerIdx, prevDataIdxInfo.contentIdx);
                }
            }

            // Add / Remove Proxy Widget
            QVector<ProxyWidgetInfo>::iterator proxyIter = m_proxyWidgetList.begin();
            for(; proxyIter != m_proxyWidgetList.end(); ++proxyIter)
            {
                if(PlayJson::MediaType::SUBTITLE == proxyIter->first.first)
                {
                    continue;
                }

                if(newDataIdxInfo == proxyIter->first.second)
                {
                    ui->playerView->scene()->addItem(proxyIter->second);
                }

                if(prevDataIdxInfo == proxyIter->first.second)
                {
                    ui->playerView->scene()->removeItem(proxyIter->second);
                }
            }

            ExecPlayDataItemList(newDataIdxInfo);
            PausePrevPlayDataSlot(prevDataIdxInfo);

            break;
        }
    }
}

//========================================================
void ContentsPlayer::PausePrevPlayDataSlot(ScheduleTimer::PlayDataIndexInfo prevPlayDataIdxInfo)
//========================================================
{
    // Video
    QVector<VideoItemInfo>::iterator videoIter = m_videoItemList.begin();
    for(; videoIter != m_videoItemList.end(); ++videoIter)
    {
        if( (prevPlayDataIdxInfo == videoIter->first) &&
            (true == videoIter->second->IsPlayingVideo()))
        {
            videoIter->second->StopVideoItem();
        }
    }

    // Clock
    QVector<ClockWidgetInfo>::iterator clockIter = m_clockWidgetList.begin();
    for(; clockIter != m_clockWidgetList.end(); ++clockIter)
    {
        if( (prevPlayDataIdxInfo == clockIter->first) &&
            (true == clockIter->second->IsStartedClock()) )
        {
            clockIter->second->StopClock();
        }
    }

    // Date
    QVector<DateWidgetInfo>::iterator dateIter = m_dateWidgetList.begin();
    for(; dateIter != m_dateWidgetList.end(); ++dateIter)
    {
        if( (prevPlayDataIdxInfo == dateIter->first) &&
            (true == dateIter->second->IsStartedDateTimer()) )
        {
            dateIter->second->StopDateWidget();
        }
    }

    // News
    QVector<NewsFeedWidgetInfo>::iterator newsIter = m_newsFeedWigetList.begin();
    for(; newsIter != m_newsFeedWigetList.end(); ++newsIter)
    {
        if( (prevPlayDataIdxInfo == newsIter->first) &&
            (true == newsIter->second->IsStartedAnimation()) )
        {
            newsIter->second->StopAnimation();
        }
    }

    // Weather
    QVector<WeatherWidgetInfo>::iterator weatherIter = m_weatherWidgetList.begin();
    for(; weatherIter != m_weatherWidgetList.end(); ++weatherIter)
    {
        if( (prevPlayDataIdxInfo == weatherIter->first) &&
            (true == weatherIter->second->IsStartedDateTimeTimer()) )
        {
            weatherIter->second->StopDateTimeTimer();
        }
    }

    // Subtitle
    QVector<SubtitleWidgetInfo>::iterator subIter = m_subtitleWidgetList.begin();
    for(; subIter != m_subtitleWidgetList.end(); ++subIter)
    {
        if( (prevPlayDataIdxInfo == subIter->first) &&
            (PlayJson::PlayDataType::CUSTOM == subIter->first.playDataInfo.type) &&
            (true == subIter->second->IsStartedAnimation()) )
        {
            subIter->second->StopAnimation();
        }
    }
}

//========================================================
void ContentsPlayer::MakeFileTypeItemSlot(ScheduleTimer::PlayDataIndexInfo contentIndxInfo,
                                          PlayJson::ContentData contentData,
                                          StyleSheet::PosSizeInfo posSizeInfo)
//========================================================
{
    QString fullPath = RESOURCE_SAVE_PATH;
    const QString& mediaTypeStr = ConvertMediaTypeEnumToString(contentData.contentInfo.mediaType);
    if(0 >= mediaTypeStr.length())
    {
        ELGO_VIEWER_LOG("Error - MediaTypeStr.length() == 0");

        return;
    }
    fullPath += mediaTypeStr;
    fullPath += contentData.name;

    if(PlayJson::MediaType::IMAGE == contentData.contentInfo.mediaType)
    {
        ImageItem *imageItem = new ImageItem;
        imageItem->setZValue(contentData.zIndex);

        const bool bIsSetItem = imageItem->SetImageItem(fullPath, posSizeInfo);
        if(true == bIsSetItem)
        {
            ImageItemInfo newImageItem;            
            newImageItem.first = contentIndxInfo;
            newImageItem.second = imageItem;
            m_imageItemList.push_back(newImageItem);
        }
        else
        {
            ELGO_VIEWER_LOG("Error - Not Add Image Item : %s",
                            contentData.name.toUtf8().constData());
        }
    }
    else
    {
        VideoItem *videoItem = new VideoItem;

        VideoInfo::MetaData videoInfo;
        videoInfo.fileName = contentData.name;
        videoInfo.duration.user = contentData.userDuration;
        videoInfo.duration.file = contentData.fileDuration;

        videoItem->SetVideoFile(fullPath, videoInfo);
        videoItem->SetVideoPosAndSize(posSizeInfo);
        videoItem->setZValue(contentData.zIndex);

        VideoItemInfo newVideoItem;
        newVideoItem.first = contentIndxInfo;
        newVideoItem.second = videoItem;
        m_videoItemList.push_back(newVideoItem);
    }
}

//========================================================
void ContentsPlayer::MakeWidgetTypeItemSlot(ScheduleTimer::PlayDataIndexInfo contentIndexInfo,
                              PlayJson::ContentData contentData,
                              StyleSheet::PosSizeInfo posSizeinfo)
//================================ ========================
{
    StyleSheet::StyleInfo styleInfo;
    styleInfo.bTransparency = contentData.bBackgroundOpacity;
    styleInfo.backgroundColor = contentData.backgroundColor;
    styleInfo.fontColor = contentData.fontColor;

    QGraphicsProxyWidget *proxyWidget = new QGraphicsProxyWidget;
    if(PlayJson::MediaType::CLOCK == contentData.contentInfo.mediaType)
    {
        ClockWidget *newClcokWidget = new ClockWidget;
        newClcokWidget->MakeClockTimeString(contentData.hourType);
        newClcokWidget->SetStyleSheet(styleInfo);
        newClcokWidget->SetPosSizeInfo(posSizeinfo);

        proxyWidget->setWidget(newClcokWidget);
        proxyWidget->setZValue(contentData.zIndex);

        ClockWidgetInfo newClockWidgetInfo(contentIndexInfo, newClcokWidget);
        m_clockWidgetList.push_back(newClockWidgetInfo);

        ProxyDataInfo proxyDataInfo(PlayJson::MediaType::CLOCK, contentIndexInfo);
        ProxyWidgetInfo proxyWidgetInfo(proxyDataInfo, proxyWidget);
        m_proxyWidgetList.push_back(proxyWidgetInfo);
    }
    else if(PlayJson::MediaType::DATE == contentData.contentInfo.mediaType)
    {
        DateWidget *newDateWidget = new DateWidget;
        newDateWidget->MakeDateLabelString();
        newDateWidget->SetStyleSheet(styleInfo);
        newDateWidget->SetPosSizeinfo(posSizeinfo);

        proxyWidget->setWidget(newDateWidget);
        proxyWidget->setZValue(contentData.zIndex);

        DateWidgetInfo newDateWidgetInfo(contentIndexInfo, newDateWidget);
        m_dateWidgetList.push_back(newDateWidgetInfo);

        ProxyDataInfo proxyDataInfo(PlayJson::MediaType::DATE, contentIndexInfo);
        ProxyWidgetInfo proxyWidgetInfo(proxyDataInfo, proxyWidget);
        m_proxyWidgetList.push_back(proxyWidgetInfo);
    }
    else if(PlayJson::MediaType::WEATHER == contentData.contentInfo.mediaType)
    {
        WeatherWidget *newWeatherWidget = new WeatherWidget;
        WeatherInfo::DisplayValue displayValue;
        displayValue.metroCity = contentData.metropolCityName;
        displayValue.city = contentData.cityName;
        displayValue.temperature = contentData.T1H;
        displayValue.pty = contentData.PTY;
        displayValue.sky = contentData.SKY;
        displayValue.lgt = contentData.LGT;

        newWeatherWidget->SetStyleSheet(styleInfo);
        newWeatherWidget->SetPosSizeInfo(posSizeinfo);
        newWeatherWidget->MakeWeatherWidget(displayValue);

        proxyWidget->setWidget(newWeatherWidget);
        proxyWidget->setZValue(contentData.zIndex);

        WeatherWidgetInfo weatherWidgetInfo(contentIndexInfo, newWeatherWidget);
        m_weatherWidgetList.push_back(weatherWidgetInfo);

        ProxyDataInfo proxyDataInfo(PlayJson::MediaType::WEATHER, contentIndexInfo);
        ProxyWidgetInfo proxyWidgetInfo(proxyDataInfo, proxyWidget);
        m_proxyWidgetList.push_back(proxyWidgetInfo);
    }
    else if(PlayJson::MediaType::NEWS == contentData.contentInfo.mediaType)
    {
        NewsFeedWidget *newNewsWidget = new NewsFeedWidget;
        StyleSheet::StyleInfo feedStyle;
        feedStyle.backgroundColor = contentData.newsBoxColor;
        feedStyle.fontColor = contentData.fontColor;
        feedStyle.fontSize = contentData.newsfontSize;
        feedStyle.bTransparency = contentData.bNewsBoxOpacity;

        newNewsWidget->SetNewsFeedList(contentData.newsCategory, contentData.newsFeedList);
        newNewsWidget->SetWidgetStyleSheet(styleInfo);
        newNewsWidget->SetFeedLabelStyleSheet(feedStyle);
        newNewsWidget->SetPosSizeInfo(posSizeinfo);
        newNewsWidget->MakeNewsFeedWidget();

        proxyWidget->setWidget(newNewsWidget);
        proxyWidget->setZValue(contentData.zIndex);

        NewsFeedWidgetInfo newsWidgetInfo(contentIndexInfo, newNewsWidget);
        m_newsFeedWigetList.push_back(newsWidgetInfo);

        ProxyDataInfo proxyDataInfo(PlayJson::MediaType::NEWS, contentIndexInfo);
        ProxyWidgetInfo proxyWidgetInfo(proxyDataInfo, proxyWidget);
        m_proxyWidgetList.push_back(proxyWidgetInfo);
    }
}

//========================================================
void ContentsPlayer::MakeSubtitleWidgetSlot(ScheduleTimer::PlayDataIndexInfo contentIndexInfo,
                            PlayJson::SubtitleData subtitleData)
//========================================================
{
    QGraphicsProxyWidget *proxyWidget = new QGraphicsProxyWidget;
    SubtitleWidget *newSubtitle = new SubtitleWidget;

    newSubtitle->MakeSubTitleWidget(subtitleData);
    proxyWidget->setWidget(newSubtitle);
    proxyWidget->setZValue(100); // top level

    SubtitleWidgetInfo subtitleWidgetInfo(contentIndexInfo, newSubtitle);
    m_subtitleWidgetList.push_back(subtitleWidgetInfo);

    ProxyDataInfo proxyDataInfo(PlayJson::MediaType::SUBTITLE, contentIndexInfo);
    ProxyWidgetInfo proxyWidgetInfo(proxyDataInfo, proxyWidget);
    m_proxyWidgetList.push_back(proxyWidgetInfo);
}

//========================================================
void ContentsPlayer::SearchItemAndAddToScene(const ScheduleTimer::PlayDataIndexInfo& playDataIdxInfo,
                                              QGraphicsScene* scene)
//========================================================
{
    if(NULL != scene)
    {
        // Find Image Item
        QVector<ImageItemInfo>::iterator imageIter = m_imageItemList.begin();
        for(; imageIter != m_imageItemList.end(); ++imageIter)
        {
            if(playDataIdxInfo == imageIter->first)
            {
                scene->addItem(imageIter->second);
                ELGO_VIEWER_LOG("ADD Image Item to Scene : %s",
                                imageIter->second->GetImageFileName().toUtf8().constData());
            }
        }

        // Find Widget Type Item
        QVector<ProxyWidgetInfo>::iterator proxyIter = m_proxyWidgetList.begin();
        for(; proxyIter != m_proxyWidgetList.end(); ++proxyIter)
        {
            if( (PlayJson::PlayDataType::FIXED == playDataIdxInfo.playDataInfo.type) &&
                (playDataIdxInfo.playDataInfo == proxyIter->first.second.playDataInfo) &&
                (PlayJson::MediaType::SUBTITLE == proxyIter->first.first))
            {
                scene->addItem(proxyIter->second);
                ELGO_VIEWER_LOG("ADD %s",
                                proxyIter->second->widget()->objectName().toStdString().c_str());
            }
            else if(playDataIdxInfo == proxyIter->first.second)
            {
                scene->addItem(proxyIter->second);
                ELGO_VIEWER_LOG("ADD %s",
                                proxyIter->second->widget()->objectName().toStdString().c_str());
            }
        }

        // Find Video Item
        QVector<VideoItemInfo>::iterator videoIter = m_videoItemList.begin();
        for(; videoIter != m_videoItemList.end(); ++videoIter)
        {
            if(playDataIdxInfo == videoIter->first)
            {
                scene->addItem(videoIter->second);
                ELGO_VIEWER_LOG("ADD Video Item to Scene : %s",
                                videoIter->second->GetVideoFileName().toUtf8().constData());
            }
        }
    }
    else
    {
        ELGO_VIEWER_LOG("Error - Scene is NULL");
    }
}

//========================================================
void ContentsPlayer::ExecPlayDataItemList(const ScheduleTimer::PlayDataIndexInfo& playDataIdxInfo)
//========================================================
{
    // Video
    QVector<VideoItemInfo>::iterator videoIter = m_videoItemList.begin();
    for(; videoIter != m_videoItemList.end(); ++videoIter)
    {
        if(playDataIdxInfo == videoIter->first)
        {
            videoIter->second->PlayVideoItem();
        }
    }

    // Clock
    QVector<ClockWidgetInfo>::iterator clockIter = m_clockWidgetList.begin();
    for(; clockIter != m_clockWidgetList.end(); ++clockIter)
    {
        if(playDataIdxInfo == clockIter->first)
        {
            clockIter->second->StartClock();
        }
    }

    // Date
    QVector<DateWidgetInfo>::iterator dateIter = m_dateWidgetList.begin();
    for(; dateIter != m_dateWidgetList.end(); ++dateIter)
    {
        if(playDataIdxInfo == dateIter->first)
        {
            dateIter->second->StartDateWidget();
        }
    }

    // News
    QVector<NewsFeedWidgetInfo>::iterator newsIter = m_newsFeedWigetList.begin();
    for(; newsIter != m_newsFeedWigetList.end(); ++newsIter)
    {
        if(playDataIdxInfo == newsIter->first)
        {
            newsIter->second->StartAnimation();
        }
    }

    // Weather
    QVector<WeatherWidgetInfo>::iterator weatherIter = m_weatherWidgetList.begin();
    for(; weatherIter != m_weatherWidgetList.end(); ++weatherIter)
    {
        if(playDataIdxInfo == weatherIter->first)
        {
            weatherIter->second->StartDateTimeTimer();
        }
    }

    // Subtitle
    QVector<SubtitleWidgetInfo>::iterator subIter = m_subtitleWidgetList.begin();
    for(; subIter != m_subtitleWidgetList.end(); ++subIter)
    {
        if( (PlayJson::PlayDataType::FIXED == playDataIdxInfo.playDataInfo.type) &&
            (playDataIdxInfo.playDataInfo == subIter->first.playDataInfo) )
        {
            subIter->second->StartAnimation();
        }
        else if(playDataIdxInfo == subIter->first)
        {
            subIter->second->StartAnimation();
        }
    }
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
