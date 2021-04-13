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
Q_DECLARE_METATYPE(ScheduleJson::PlaySchedules);
Q_DECLARE_METATYPE(PlayJson::PlayData);

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
    qRegisterMetaType<ScheduleJson::PlaySchedules>("ScheduleJson::PlaySchedules");
    qRegisterMetaType<PlayJson::PlayData>("PlayJson::PlayData");

    qRegisterMetaType<ScheduleTimer::PlayDataIndexInfo>("ScheduleTimer::PlayDataIndexInfo");
    qRegisterMetaType<PlayJson::ContentData>("PlayJson::ContentData");
    qRegisterMetaType<StyleSheet::PosSizeInfo>("StyleSheet::PosSizeInfo");
    qRegisterMetaType<StyleSheet::StyleInfo>("StyleSheet::StyleInfo");

    connect(this, SIGNAL(AddPlayDataSignal(PlayJson::CustomPlayDataJson)),
            this, SLOT(AddPlayDataSlot(PlayJson::CustomPlayDataJson)));

    connect(this, SIGNAL(AddPlayDataSignal(PlayJson::FixedPlayDataJson)),
            this, SLOT(AddPlayDataSlot(PlayJson::FixedPlayDataJson)));

    connect(this, SIGNAL(AddPlayScheduleListSignal(ScheduleJson::PlaySchedules)),
            this, SLOT(AddPlayScheduleListSlot(ScheduleJson::PlaySchedules)));

    connect(this, SIGNAL(ExecPlayDataSingal(PlayJson::PlayData)),
            this, SLOT(ExecPlayDataSlot(PlayJson::PlayData)));

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
void ContentsPlayer::StartScheduleTimer()
//========================================================
{
    ELGO_VIEWER_LOG("Start Schedule / Signle Timer !");

    m_schedulerTimer = new SchedulesTimer;
    m_singleTimer = new SinglePlayTimer;

    m_schedulerTimer->start(900);
    m_singleTimer->start(990);
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
void ContentsPlayer::AddPlayScheduleListSlot(ScheduleJson::PlaySchedules src)
//========================================================
{
    m_schedulerTimer->AddPlaySchedule(src);
}

//========================================================
void ContentsPlayer::ExecPlayDataSlot(PlayJson::PlayData src)
//========================================================
{
    ScheduleTimer::PlayDataInfo playDataInfo;
    playDataInfo.id = src.id;
    playDataInfo.type = src.playDataType;

    m_singleTimer->ExecPlayData(playDataInfo);
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
                if(newDataIdxInfo == proxyIter->first)
                {
                    ui->playerView->scene()->addItem(proxyIter->second);
                }

                if(prevDataIdxInfo == proxyIter->first)
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

        ProxyWidgetInfo proxyWidgetInfo(contentIndexInfo, proxyWidget);
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

        ProxyWidgetInfo proxyWidgetInfo(contentIndexInfo, proxyWidget);
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

        ProxyWidgetInfo proxyWidgetInfo(contentIndexInfo, proxyWidget);
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

        ProxyWidgetInfo proxyWidgetInfo(contentIndexInfo, proxyWidget);
        m_proxyWidgetList.push_back(proxyWidgetInfo);
    }
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
            if(playDataIdxInfo == proxyIter->first)
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
