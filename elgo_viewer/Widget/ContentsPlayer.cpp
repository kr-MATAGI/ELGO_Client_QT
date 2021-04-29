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

    // timer
    connect(&m_scheduleTimer, &QTimer::timeout,
            this, &ContentsPlayer::PlayerTimeout);
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
void ContentsPlayer::ExecPlayDataSlot(const PlayJson::PlayData& playData,
                                      const bool bDelPrevData)
//========================================================
{
    m_scheduleTimer.stop();

    if( (playData.id != m_playingIndex.playData.id) ||
        (playData.playDataType != m_playingIndex.playData.playDataType) )
    {
        ELGO_VIEWER_LOG("Read to Play - {id: %d, type: %d}",
                        playData.id, playData.playDataType);

        // Make Custom or Fixed Contents
        if(PlayJson::PlayDataType::CUSTOM == playData.playDataType)
        {
            MakeCustomPlayDataContents(playData);
        }
        else if(PlayJson::PlayDataType::FIXED == playData.playDataType)
        {
            MakeFixedPlayDataContents(playData);
        }
        else
        {
            ELGO_VIEWER_LOG("Error - Unkwon PlayData {id: %d, type: %d}",
                            playData.id,
                            playData.playDataType);
        }

        // Update Display Scene
        ScheduleTimer::PlayingIndex playingIndex;
        playingIndex.playData = playData;
        UpdatePlayerScene(playingIndex, bDelPrevData);
    }
    else
    {
        ELGO_VIEWER_LOG("Already Playing - {id: %d, type: %d}",
                        playData.id, playData.playDataType);
    }

    m_scheduleTimer.start(990);
}

//========================================================
void ContentsPlayer::UpdatePlayerScene(const ScheduleTimer::PlayingIndex& playingIndex,
                                       const bool bDelPrevData)
//========================================================
{
    QGraphicsScene *newScene = new QGraphicsScene(this);
    newScene->setSceneRect(m_screenRect);

    if(PlayJson::PlayDataType::CUSTOM == playingIndex.playData.playDataType)
    {
        SearchContentAndAddToScene(playingIndex, newScene);
        ELGO_VIEWER_LOG("Made New Scene { id: %d, pageIdx: %d, sceneItemSize: %d }",
                        playingIndex.playData.id, playingIndex.pageIdx, newScene->items().size());

        PlayItemAndWidgetContents(playingIndex);
    }
    else if(PlayJson::PlayDataType::FIXED == playingIndex.playData.playDataType)
    {
        ScheduleTimer::PlayingIndex fixedPlayingIndex = playingIndex;
        for(int layIdx = 0; layIdx < m_playCountdown.maxLayer; layIdx++)
        {
            SearchContentAndAddToScene(fixedPlayingIndex, newScene);
            PlayItemAndWidgetContents(fixedPlayingIndex);
            fixedPlayingIndex.layerIdx++;
        }
    }
    else
    {
        newScene->deleteLater();
        ELGO_VIEWER_LOG("Error Unkown PlayData {id: %d, type: %d}",
                        playingIndex.playData.id,
                        playingIndex.playData.playDataType);

        return;
    }

    ui->playerView->setScene(newScene);

    PauseItemAndWidgetContents(m_playingIndex);
    m_playingIndex = playingIndex;

    if(NULL != m_currScene.second)
    {
        QList<QGraphicsItem *> items = m_currScene.second->items();
        foreach(auto item, items)
        {
            m_currScene.second->removeItem(item);
        }
        m_currScene.second->deleteLater();
    }

    if(true == bDelPrevData)
    {
        ClearOtherPlayDataJsonInfo(m_playingIndex);
    }

    SceneInfo newSceneInfo(playingIndex, newScene);
    m_currScene = newSceneInfo;
}

//========================================================
void ContentsPlayer::SearchContentAndAddToScene(const ScheduleTimer::PlayingIndex& playingIndex,
                                                QGraphicsScene* dest)
//========================================================
{
    if(NULL != dest)
    {
        // Find Image Item
        QVector<ImageItemInfo>::iterator imageIter = m_imageItemList.begin();
        for(; imageIter != m_imageItemList.end(); ++imageIter)
        {
            const bool bIsSameIndex = ComparePlayingIndex(playingIndex, imageIter->first);
            if(true == bIsSameIndex)
            {
                dest->addItem(imageIter->second);
                ELGO_VIEWER_LOG("ADD Image item to scene - %s",
                                imageIter->second->GetImageFileName().toStdString().c_str());
            }
        }

        // Find Video Item
        QVector<VideoItemInfo>::iterator videoIter = m_videoItemList.begin();
        for(; videoIter != m_videoItemList.end(); ++videoIter)
        {
            const bool bIsSameIndex = ComparePlayingIndex(playingIndex, videoIter->first);
            if(true == bIsSameIndex)
            {
                dest->addItem(videoIter->second);
                ELGO_VIEWER_LOG("ADD Video item to scene - %s",
                                videoIter->second->GetVideoFileName().toStdString().c_str());
            }
        }

        // Find Widget Item
        QVector<ProxyWidgetInfo>::iterator proxyIter = m_proxyWidgetList.begin();
        for(; proxyIter != m_proxyWidgetList.end(); ++proxyIter)
        {
            const bool bIsSameIndex = ComparePlayingIndex(playingIndex, proxyIter->first);
            if(true == bIsSameIndex)
            {
                dest->addItem(proxyIter->second);
                ELGO_VIEWER_LOG("ADD Widget Type Item - %s",
                                proxyIter->second->widget()->objectName().toStdString().c_str());
            }
        }
    }
    else
    {
        ELGO_VIEWER_LOG("Error -Scene is NULL");
    }
}

//========================================================
void ContentsPlayer::MakeCustomPlayDataContents(const PlayJson::PlayData& playData)
//========================================================
{
    // Find play data in saved list
    int dataIdx = NOT_EXISTED_DATA;
    const int playDataListSize = m_customPlayDataList.size();
    for(int idx = 0; idx < playDataListSize; idx++)
    {
        if(playData.id == m_customPlayDataList[idx].playData.id)
        {
            dataIdx = idx;
            break;
        }
    }

    if(NOT_EXISTED_DATA == dataIdx)
    {
        ELGO_VIEWER_LOG("Not existed custom play data - id: %d", playData.id);
        return;
    }

    // Set play data
    const PlayJson::CustomPlayDataJson& customPlayData = m_customPlayDataList[dataIdx];

    // Make countdown information
    ScheduleTimer::CountdownInfo countdownInfo;
    countdownInfo.id = customPlayData.playData.id;
    countdownInfo.type = customPlayData.playData.playDataType;
    countdownInfo.maxPage = customPlayData.pageDataList.size();
    countdownInfo.pageTimecount.resize(countdownInfo.maxPage);

    // Set Countdown info
    m_playCountdown = countdownInfo;

    // Make item or widget contents
    const int pageListSize = customPlayData.pageDataList.size();
    for(int pageIdx = 0; pageIdx < pageListSize; pageIdx++)
    {
        const PlayJson::PageData& pageData = customPlayData.pageDataList[pageIdx];

        // Set page index info
        ScheduleTimer::PlayingIndex playingIndex;
        playingIndex.playData = playData;
        playingIndex.pageIdx = pageIdx;

        const int layerListSize = pageData.layerDataList.size();
        for(int layIdx = 0; layIdx < layerListSize; layIdx++)
        {
            const PlayJson::CustomLayerData& layerData = pageData.layerDataList[layIdx];

            // Set pos and size
            StyleSheet::PosSizeInfo posSize;
            posSize.pos = QPoint(layerData.left, layerData.top);
            posSize.size = QSize(layerData.width, layerData.height);

            // Set media type
            playingIndex.mediaType = layerData.layerContent.contentInfo.mediaType;

            // Make Content
            if(PlayJson::ContentType::FILE == layerData.layerContent.contentInfo.contentType)
            {
                MakeFileTypeContent(playingIndex, layerData.layerContent, posSize);
            }
            else if(PlayJson::ContentType::WIDGET == layerData.layerContent.contentInfo.contentType)
            {
                MakeWidgetTypeContent(playingIndex, layerData.layerContent, posSize);
            }
            else
            {
                ELGO_VIEWER_LOG("Error - Unkwon Content Type: %d", layerData.layerContent.contentInfo.contentType);
            }
        }

        // Make Subtitle
        const int subtitleListSize = pageData.subtitleDataList.size();
        for(int subIdx = 0; subIdx < subtitleListSize; subIdx++)
        {
            ScheduleTimer::PlayingIndex subtitlePlayingIndex;
            subtitlePlayingIndex.playData = playData;
            subtitlePlayingIndex.pageIdx = pageIdx;
            subtitlePlayingIndex.mediaType = PlayJson::MediaType::SUBTITLE;

            MakeSubtitleTypeContent(subtitlePlayingIndex, pageData.subtitleDataList[subIdx]);
        }
    }
}

//========================================================
void ContentsPlayer::MakeFixedPlayDataContents(const PlayJson::PlayData& playData)
//========================================================
{
    // Find play data in saved list
    int dataIdx = NOT_EXISTED_DATA;
    const int playDataListSize = m_fixedPlayDataList.size();
    for(int idx = 0; idx < playDataListSize; idx++)
    {
        if(playData.id == m_fixedPlayDataList[idx].playData.id)
        {
            dataIdx = idx;
            break;
        }
    }

    if(NOT_EXISTED_DATA == dataIdx)
    {
        ELGO_VIEWER_LOG("Not existed custom play data - id: %d", playData.id);
        return;
    }

    // Set play data
    const PlayJson::FixedPlayDataJson& fixedPlayData = m_fixedPlayDataList[dataIdx];

    // Make countdown information
    ScheduleTimer::CountdownInfo countdown;
    countdown.id = playData.id;
    countdown.type = playData.playDataType;
    countdown.maxLayer = fixedPlayData.layerDataList.size();
    countdown.layerTimecountList.resize(countdown.maxLayer);

    ScheduleTimer::PlayingIndex playingIndex;
    playingIndex.playData.id = playingIndex.playData.id;
    playingIndex.playData.playDataType = playingIndex.playData.playDataType;

    // Make item or widget contents
    const int layerListSize = fixedPlayData.layerDataList.size();
    for(int layIdx = 0; layIdx < layerListSize; layIdx++)
    {
        const PlayJson::FixedLayerData& layerData = fixedPlayData.layerDataList[layIdx];

        // Set layer timeout info
        const int maxContentSize = layerData.contentDataList.size();
        countdown.layerTimecountList[layIdx].maxContent = maxContentSize;
        countdown.layerTimecountList[layIdx].contentTimeout.resize(maxContentSize);

        // Content index info
        ScheduleTimer::PlayingIndex playingIndex;
        playingIndex.playData = playData;
        playingIndex.layerIdx = layIdx;

        // Set pos and size
        StyleSheet::PosSizeInfo posSize;
        posSize.pos = QPoint(layerData.left, layerData.top);
        posSize.size = QSize(layerData.width, layerData.height);

        for(int cdIdx = 0; cdIdx < maxContentSize; cdIdx++)
        {
            // Make Content
            const PlayJson::ContentData& contentData = layerData.contentDataList[cdIdx];

            playingIndex.contentIdx = cdIdx;
            playingIndex.mediaType = contentData.contentInfo.mediaType;

            if(PlayJson::ContentType::FILE == contentData.contentInfo.contentType)
            {
                MakeFileTypeContent(playingIndex, contentData, posSize);
            }
            else if(PlayJson::ContentType::WIDGET == contentData.contentInfo.contentType)
            {
                MakeWidgetTypeContent(playingIndex, contentData, posSize);
            }
            else
            {
                ELGO_VIEWER_LOG("Error - Unkwon Content Type: %d", contentData.contentInfo.contentType);
            }
        }
    }

    // Make Subtitle
    const int subtitleListSize = fixedPlayData.subtitleDataList.size();
    for(int subIdx = 0; subIdx < subtitleListSize; subIdx++)
    {
        ScheduleTimer::PlayingIndex subtitlePlayingIndex;
        subtitlePlayingIndex.playData = playData;

        MakeSubtitleTypeContent(subtitlePlayingIndex, fixedPlayData.subtitleDataList[subIdx]);
    }

    // Set Countdown info
    m_playCountdown = countdown;
}

//========================================================
void ContentsPlayer::MakeFileTypeContent(const ScheduleTimer::PlayingIndex& playingIndex,
                                         const PlayJson::ContentData& contentData,
                                         const StyleSheet::PosSizeInfo& posSize)
//========================================================
{
    // Make resource path
    QString fullPath = RESOURCE_SAVE_PATH;
    QString mediaTypeStr;
    ConvertMediaTypeEnumToString(contentData.contentInfo.mediaType, mediaTypeStr);
    if(0 >= mediaTypeStr.length())
    {
        ELGO_VIEWER_LOG("Error - MediaType.length() == %d", mediaTypeStr.length());
        return;
    }
    fullPath.append(mediaTypeStr);
    fullPath.append(contentData.name);

    if(PlayJson::MediaType::IMAGE == contentData.contentInfo.mediaType)
    {
        ImageItem *newImageItem = new ImageItem;
        newImageItem->setZValue(contentData.zIndex);

        const bool bIsSetItem = newImageItem->SetImageItem(fullPath, posSize);
        if(true == bIsSetItem)
        {
            ImageItemInfo imageItemInfo(playingIndex, newImageItem);
            m_imageItemList.push_back(imageItemInfo);
        }
        else
        {
            ELGO_VIEWER_LOG("Error - Not Added Image Item: %s",
                            contentData.name.toStdString().c_str());
        }
    }
    else if(PlayJson::MediaType::VIDEO == contentData.contentInfo.mediaType)
    {
        VideoItem *newVideoItem = new VideoItem;

        VideoInfo::MetaData metaData;
        metaData.fileName = contentData.name;
        metaData.duration.user = contentData.userDuration;
        metaData.duration.file = contentData.fileDuration;

        newVideoItem->SetVideoFile(fullPath, metaData);
        newVideoItem->SetVideoPosAndSize(posSize);
        newVideoItem->setZValue(contentData.zIndex);

        VideoItemInfo videoItemInfo(playingIndex, newVideoItem);
        m_videoItemList.push_back(videoItemInfo);
    }
    else
    {
        ELGO_VIEWER_LOG("Error - Unkwon Media Type: %d", contentData.contentInfo.mediaType);
    }
}

//========================================================
void ContentsPlayer::MakeWidgetTypeContent(const ScheduleTimer::PlayingIndex& playingIndex,
                                           const PlayJson::ContentData& contentData,
                                           const StyleSheet::PosSizeInfo& posSize)
//========================================================
{
    // Set style info
    StyleSheet::StyleInfo styleInfo;
    styleInfo.bTransparency = contentData.bBackgroundOpacity;
    styleInfo.backgroundColor = contentData.backgroundColor;
    styleInfo.fontColor = contentData.fontColor;

    // Make widget and insert proxy widget
    QGraphicsProxyWidget *newProxyWidget = new QGraphicsProxyWidget;
    if(PlayJson::MediaType::CLOCK == contentData.contentInfo.mediaType)
    {
        ClockWidget *newClockWidget = new ClockWidget;
        newClockWidget->MakeClockTimeString(contentData.hourType);
        newClockWidget->SetStyleSheet(styleInfo);
        newClockWidget->SetPosSizeInfo(posSize);

        newProxyWidget->setWidget(newClockWidget);
        newProxyWidget->setZValue(contentData.zIndex);

        ClockWidgetInfo clockWidgetInfo(playingIndex, newClockWidget);
        m_clockWidgetList.push_back(clockWidgetInfo);

        ProxyWidgetInfo proxyWidgetInfo(playingIndex, newProxyWidget);
        m_proxyWidgetList.push_back(proxyWidgetInfo);
    }
    else if(PlayJson::MediaType::DATE == contentData.contentInfo.mediaType)
    {
        DateWidget *newDateWidget = new DateWidget;
        newDateWidget->MakeDateLabelString();
        newDateWidget->SetStyleSheet(styleInfo);
        newDateWidget->SetPosSizeinfo(posSize);

        newProxyWidget->setWidget(newDateWidget);
        newProxyWidget->setZValue(contentData.zIndex);

        DateWidgetInfo dateWidgetInfo(playingIndex, newDateWidget);
        m_dateWidgetList.push_back(dateWidgetInfo);

        ProxyWidgetInfo proxyWidgetInfo(playingIndex, newProxyWidget);
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
        newWeatherWidget->SetPosSizeInfo(posSize);
        newWeatherWidget->SetDisplayValue(displayValue);

        newProxyWidget->setWidget(newWeatherWidget);
        newProxyWidget->setZValue(contentData.zIndex);

        WeatherWidgetInfo weatherWidgetInfo(playingIndex, newWeatherWidget);
        m_weatherWidgetList.push_back(weatherWidgetInfo);

        ProxyWidgetInfo proxyWidgetInfo(playingIndex, newProxyWidget);
        m_proxyWidgetList.push_back(proxyWidgetInfo);
    }
    else if(PlayJson::MediaType::NEWS == contentData.contentInfo.mediaType)
    {
        NewsFeedWidget * newNewsWidget = new NewsFeedWidget;

        StyleSheet::StyleInfo feedStyle;
        feedStyle.backgroundColor = contentData.newsBoxColor;
        feedStyle.fontColor = contentData.fontColor;
        feedStyle.fontSize = contentData.newsfontSize;
        feedStyle.bTransparency = contentData.bNewsBoxOpacity;

        newNewsWidget->SetNewsFeedList(contentData.newsCategory, contentData.newsFeedList);
        newNewsWidget->SetWidgetStyleSheet(styleInfo);
        newNewsWidget->SetFeedLabelStyleSheet(feedStyle);
        newNewsWidget->SetPosSizeInfo(posSize);
        newNewsWidget->MakeNewsFeedWidget();

        newProxyWidget->setWidget(newNewsWidget);
        newProxyWidget->setZValue(contentData.zIndex);

        NewsFeedWidgetInfo newsWidgetInfo(playingIndex, newNewsWidget);
        m_newsFeedWigetList.push_back(newsWidgetInfo);

        ProxyWidgetInfo proxyWidgetInfo(playingIndex, newProxyWidget);
        m_proxyWidgetList.push_back(proxyWidgetInfo);
    }
    else
    {
        ELGO_VIEWER_LOG("Error - Unkwon Media Type: %d", contentData.contentInfo.mediaType);
    }
}

//========================================================
void ContentsPlayer::MakeSubtitleTypeContent(const ScheduleTimer::PlayingIndex& playingIndex,
                                             const PlayJson::SubtitleData& subtitleData)
//========================================================
{
    QGraphicsProxyWidget *newProxyWidget = new QGraphicsProxyWidget;
    SubtitleWidget *newSubtitle = new SubtitleWidget;

    newSubtitle->MakeSubTitleWidget(subtitleData);
    newProxyWidget->setWidget(newSubtitle);
    newProxyWidget->setZValue(100); // top level

    SubtitleWidgetInfo subtitleWidgetInfo(playingIndex, newSubtitle);
    m_subtitleWidgetList.push_back(subtitleWidgetInfo);

    ProxyWidgetInfo proxyWidgetInfo(playingIndex, newProxyWidget);
    m_proxyWidgetList.push_back(proxyWidgetInfo);
}

//========================================================
void ContentsPlayer::PlayItemAndWidgetContents(const ScheduleTimer::PlayingIndex& playingIndex)
//========================================================
{
    // Video
    QVector<VideoItemInfo>::iterator videoIter = m_videoItemList.begin();
    for(; videoIter != m_videoItemList.end(); ++videoIter)
    {
        const bool bIsSameIndex = ComparePlayingIndex(playingIndex, videoIter->first);
        if(true == bIsSameIndex)
        {
            videoIter->second->PlayVideoItem();
        }
    }

    // Clock
    QVector<ClockWidgetInfo>::iterator clockIter = m_clockWidgetList.begin();
    for(; clockIter != m_clockWidgetList.end(); ++clockIter)
    {
        const bool bIsSameIndex = ComparePlayingIndex(playingIndex, clockIter->first);
        if(true == bIsSameIndex)
        {
            clockIter->second->StartClock();
        }
    }

    // Date
    QVector<DateWidgetInfo>::iterator dateIter = m_dateWidgetList.begin();
    for(; dateIter != m_dateWidgetList.end(); ++dateIter)
    {
        const bool bIsSameIndex = ComparePlayingIndex(playingIndex, dateIter->first);
        if(true == bIsSameIndex)
        {
            dateIter->second->StartDateWidget();
        }
    }

    // News
    QVector<NewsFeedWidgetInfo>::iterator newsIter = m_newsFeedWigetList.begin();
    for(; newsIter != m_newsFeedWigetList.end(); ++newsIter)
    {
        const bool bIsSameIndex = ComparePlayingIndex(playingIndex, newsIter->first);
        if(true == bIsSameIndex)
        {
            newsIter->second->StartAnimation();
        }
    }

    // Weather
    QVector<WeatherWidgetInfo>::iterator weatherIter = m_weatherWidgetList.begin();
    for(; weatherIter != m_weatherWidgetList.end(); ++weatherIter)
    {
        const bool bIsSameIndex = ComparePlayingIndex(playingIndex, weatherIter->first);
        if(true == bIsSameIndex)
        {
            weatherIter->second->StartDateTimeTimer();
        }
    }

    // Subtitle
    QVector<SubtitleWidgetInfo>::iterator subIter = m_subtitleWidgetList.begin();
    for(; subIter != m_subtitleWidgetList.end(); ++subIter)
    {
        const bool bIsSameIndex = ComparePlayingIndex(playingIndex, subIter->first);
        if(true == bIsSameIndex)
        {
            subIter->second->StartAnimation();
        }
    }
}

//========================================================
void ContentsPlayer::PauseItemAndWidgetContents(const ScheduleTimer::PlayingIndex& playingIndex)
//========================================================
{
    if( (0 == playingIndex.playData.id) &&
        (PlayJson::PlayDataType::NONE_PLAY_DATA_TYPE == playingIndex.playData.playDataType) )
    {
        ELGO_VIEWER_LOG("Not Existed Prev Data - {id: %d, type: %d}",
                        playingIndex.playData.id, playingIndex.playData.playDataType);

        return;
    }

    // Video
    QVector<VideoItemInfo>::iterator videoIter = m_videoItemList.begin();
    for(; videoIter != m_videoItemList.end(); ++videoIter)
    {
        const bool bIsSameIndex = ComparePlayingIndex(playingIndex, videoIter->first);
        if( (true == bIsSameIndex) &&
            (true == videoIter->second->IsPlayingVideo()) )
        {
            videoIter->second->StopVideoItem();
        }
    }

    // Clock
    QVector<ClockWidgetInfo>::iterator clockIter = m_clockWidgetList.begin();
    for(; clockIter != m_clockWidgetList.end(); ++clockIter)
    {
        const bool bIsSameIndex = ComparePlayingIndex(playingIndex, clockIter->first);
        if( (true == bIsSameIndex) &&
            (true == clockIter->second->IsStartedClock()) )
        {
            clockIter->second->StopClock();
        }
    }

    // Date
    QVector<DateWidgetInfo>::iterator dateIter = m_dateWidgetList.begin();
    for(; dateIter != m_dateWidgetList.end(); ++dateIter)
    {
        const bool bIsSameIndex = ComparePlayingIndex(playingIndex, dateIter->first);
        if( (true == bIsSameIndex) &&
            (true == dateIter->second->IsStartedDateTimer()) )

        {
            dateIter->second->StopDateWidget();
        }
    }

    // News
    QVector<NewsFeedWidgetInfo>::iterator newsIter = m_newsFeedWigetList.begin();
    for(; newsIter != m_newsFeedWigetList.end(); ++newsIter)
    {
        const bool bIsSameIndex = ComparePlayingIndex(playingIndex, newsIter->first);
        if( (true == bIsSameIndex) &&
            (true == newsIter->second->IsStartedAnimation()) )
        {
            newsIter->second->StopAnimation();
        }
    }

    // Weather
    QVector<WeatherWidgetInfo>::iterator weatherIter = m_weatherWidgetList.begin();
    for(; weatherIter != m_weatherWidgetList.end(); ++weatherIter)
    {
        const bool bIsSameIndex = ComparePlayingIndex(playingIndex, weatherIter->first);
        if( (true == bIsSameIndex) &&
            (true == weatherIter->second->IsStartedDateTimeTimer()) )

        {
            weatherIter->second->StopDateTimeTimer();
        }
    }

    // Subtitle
    QVector<SubtitleWidgetInfo>::iterator subIter = m_subtitleWidgetList.begin();
    for(; subIter != m_subtitleWidgetList.end(); ++subIter)
    {
        const bool bIsSameIndex = ComparePlayingIndex(playingIndex, subIter->first);
        if( (true == bIsSameIndex) &&
            (true == subIter->second->IsStartedAnimation()) )
        {
            subIter->second->StopAnimation();
        }
    }
}

//========================================================
void ContentsPlayer::ClearOtherPlayDataJsonInfo(const ScheduleTimer::PlayingIndex& playingIndex)
//========================================================
{
    const int prevCustomPlayDataListSize = m_customPlayDataList.size();
    const int prevFixedPlayDataListSize = m_fixedPlayDataList.size();

    QVector<PlayJson::CustomPlayDataJson>::iterator customIter = m_customPlayDataList.begin();
    for( ; customIter != m_customPlayDataList.end(); )
    {
        if(playingIndex.playData.id != customIter->playData.id)
        {
            ELGO_VIEWER_LOG("Delete PlayData - {id: %d, type: %d}",
                            customIter->playData.id, customIter->playData.playDataType);
            customIter = m_customPlayDataList.erase(customIter);
        }
        else
        {
            ++customIter;
        }
    }

    QVector<PlayJson::FixedPlayDataJson>::iterator fixedIter = m_fixedPlayDataList.begin();
    for(; fixedIter != m_fixedPlayDataList.end(); )
    {
        if(playingIndex.playData.id != fixedIter->playData.id)
        {
            ELGO_VIEWER_LOG("Delete PlayData - {id: %d, type: %d}",
                            fixedIter->playData.id, fixedIter->playData.playDataType);
            fixedIter = m_fixedPlayDataList.erase(fixedIter);
        }
        else
        {
            ++fixedIter;
        }
    }

    ELGO_VIEWER_LOG("Custom PlayData List Size { %d -> %d }",
                    prevCustomPlayDataListSize, m_customPlayDataList.size());
    ELGO_VIEWER_LOG("Fixed PlayData List Size { %d -> %d }",
                    prevFixedPlayDataListSize, m_fixedPlayDataList.size());

    // Delete Prev Item
    ClearOtherPlayDataItem(playingIndex);
}

//========================================================
void ContentsPlayer::ClearOtherPlayDataItem(const ScheduleTimer::PlayingIndex& playingIndex)
//========================================================
{
    // image
    const int prevImageListSize = m_imageItemList.size();

    QVector<ImageItemInfo>::iterator imageIter = m_imageItemList.begin();
    for(; imageIter != m_imageItemList.end(); )
    {
        if( (playingIndex.playData.id != imageIter->first.playData.id) ||
            (playingIndex.playData.playDataType != imageIter->first.playData.playDataType) )
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
        if( (playingIndex.playData.id != videoIter->first.playData.id) ||
            (playingIndex.playData.playDataType != videoIter->first.playData.playDataType) )
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
        if( (playingIndex.playData.id != clockIter->first.playData.id) ||
            (playingIndex.playData.playDataType != clockIter->first.playData.playDataType) )
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
        if( (playingIndex.playData.id != dateIter->first.playData.id) ||
            (playingIndex.playData.playDataType != dateIter->first.playData.playDataType) )
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
        if( (playingIndex.playData.id != newsIter->first.playData.id) ||
            (playingIndex.playData.playDataType != newsIter->first.playData.playDataType) )
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
        if( (playingIndex.playData.id != weatherIter->first.playData.id) ||
            (playingIndex.playData.playDataType != weatherIter->first.playData.playDataType))
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
        if( (playingIndex.playData.id != subIter->first.playData.id) ||
            (playingIndex.playData.playDataType != subIter->first.playData.playDataType) )
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
        if( (playingIndex.playData.id != proxyIter->first.playData.id) ||
            (playingIndex.playData.playDataType != proxyIter->first.playData.playDataType) )
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
void ContentsPlayer::PlayerTimeout()
//========================================================
{
    if(PlayJson::PlayDataType::CUSTOM == m_playingIndex.playData.playDataType)
    {
        if( 1 == m_playCountdown.maxPage )
        {
            ELGO_VIEWER_LOG("Not update Scene - Max Page Size: %d",
                            m_playCountdown.maxPage);
            return;
        }

        QVector<PlayJson::CustomPlayDataJson>::iterator customIter = m_customPlayDataList.begin();
        for( ; customIter != m_customPlayDataList.end(); ++customIter)
        {
            if(customIter->playData.id == m_playingIndex.playData.id)
            {
                // Get Current Display Page
                const int currPageIdx = m_playingIndex.pageIdx;
                const PlayJson::PageData& currPageData = customIter->pageDataList[currPageIdx];

                // Reached Timeout
                if(m_playCountdown.pageTimecount[currPageIdx] >= currPageData.duration)
                {
                    // Current Page Timecount Initialize
                    m_playCountdown.pageTimecount[currPageIdx] = 0;

                    // Ready to Next Page
                    int nextPageIdx = currPageIdx + 1;
                    if(m_playCountdown.maxPage <= nextPageIdx)
                    {
                        nextPageIdx = 0;
                    }

                    ScheduleTimer::PlayingIndex nextPlayingIndex = m_playingIndex;
                    nextPlayingIndex.pageIdx = nextPageIdx;
                    ELGO_VIEWER_LOG("Update NextPage - { id: %d, pageIdx: %d, currPageTimeCnt{ %d : %d } }",
                                    nextPlayingIndex.playData.id, nextPlayingIndex.pageIdx,
                                    m_playCountdown.pageTimecount[currPageIdx], currPageData.duration);

                    UpdatePlayerScene(nextPlayingIndex);
                }
                else
                {
                    // Plus Countdown
                    m_playCountdown.pageTimecount[currPageIdx]++;
                    ELGO_VIEWER_LOG("Custom Page[%d] Countdown: %d",
                                    currPageIdx, m_playCountdown.pageTimecount[currPageIdx]);
                }

                break;
            }
        }
    }
    else if(PlayJson::PlayDataType::FIXED == m_playingIndex.playData.playDataType)
    {

    }
    else
    {
        ELGO_VIEWER_LOG("Error - Timeout method is not need {id: %d, type: %d}",
                        m_playingIndex.playData.id, m_playingIndex.playData.playDataType);
    }
}

//========================================================
void ContentsPlayer::ConvertMediaTypeEnumToString(const PlayJson::MediaType src, QString& dest)
//========================================================
{
    if(PlayJson::MediaType::VIDEO == src)
    {
        dest = "video/";
    }
    else if(PlayJson::MediaType::IMAGE == src)
    {
        dest = "image/";
    }
    else if(PlayJson::MediaType::WEATHER == src)
    {
        dest = "icon/";
    }
    else
    {
        ELGO_VIEWER_LOG("Error - Unkwon MediaType : %d", src);
    }
}

//========================================================
bool ContentsPlayer::ComparePlayingIndex(const ScheduleTimer::PlayingIndex& lhs,
                                         const ScheduleTimer::PlayingIndex& rhs)
//========================================================
{
    if(!(lhs.playData.id == rhs.playData.id))
    {
        return false;
    }

    if(!(lhs.playData.playDataType == rhs.playData.playDataType))
    {
        return false;
    }

    if(!(lhs.pageIdx == rhs.pageIdx))
    {
        return false;
    }

    if(!(lhs.layerIdx == rhs.layerIdx))
    {
        return false;
    }

    if(!(lhs.contentIdx == rhs.contentIdx))
    {
        return false;
    }

    return true;
}
