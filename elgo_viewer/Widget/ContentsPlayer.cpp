// QT
#include <QScreen>
#include <QWindow>

// Viewer
#include "ContentsPlayer.h"
#include "ui_ContentsPlayer.h"
#include "Logger/ViewerLogger.h"

// Common
#include "Common/CommonDef.h"
#include "Common/Interface/ContentsPlayDataImpl.h"

ContentsPlayer* ContentsPlayer::pInstance = nullptr;

//========================================================
ContentsPlayer::ContentsPlayer(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ContentsPlayer)
    , m_heading(VIEWER_DEF::HEADING::TOP)
    , m_playDataIndex(NOT_EXISTED_DATA)
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
    qRegisterMetaType<PlayJson::CustomPlayDataJson>("PlayJson::CustomPlayDataJson");
    qRegisterMetaType<PlayJson::FixedPlayDataJson>("PlayJson::FixedPlayDataJson");
    qRegisterMetaType<PlayJson::PlayData>("PlayJson::PlayData");

    // Connect Singal and Slot
    connect(this, &ContentsPlayer::StartContentsPlayerSignal,
            this, &ContentsPlayer::StartContentsPlayerSlot);

    connect(this, SIGNAL(AddPlayDataSignal(const PlayJson::CustomPlayDataJson&)),
            this, SLOT(AddPlayDataSlot(const PlayJson::CustomPlayDataJson&)));
    connect(this, SIGNAL(AddPlayDataSignal(const PlayJson::FixedPlayDataJson&)),
            this, SLOT(AddPlayDataSlot(const PlayJson::FixedPlayDataJson&)));

    connect(this, &ContentsPlayer::ExecPlayDataSignal,
            this, &ContentsPlayer::ExecPlayDataSlot);

    connect(this, &ContentsPlayer::RotateScreenSignal,
            this, &ContentsPlayer::RotateScreenSlot);

    connect(this, &ContentsPlayer::UpdatePlayerPauseSignal,
            this, &ContentsPlayer::UpdatePlayerPauseSlot);

    // timer
    connect(&m_playerTimer, &QTimer::timeout,
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
void ContentsPlayer::AddPlayDataSlot(const PlayJson::CustomPlayDataJson& src)
//========================================================
{
    int pos = -1;
    QVector<PlayJson::CustomPlayDataJson>::const_iterator iter = m_customPlayDataList.constBegin();
    while(iter != m_customPlayDataList.constEnd())
    {
        if((*iter).playData.id == src.playData.id)
        {
            pos = iter - m_customPlayDataList.constBegin();
            break;
        }
        ++iter;
    }

    if(-1 == pos)
    {
        m_customPlayDataList.push_back(src);
        ELGO_VIEWER_LOG("ADD PlayData - {id: %d, type: %d}",
                        src.playData.id, src.playData.playDataType);
    }
    else
    {
        m_customPlayDataList[pos] = src;
        ELGO_VIEWER_LOG("[Replace] Existed Data - {pos: %d, id: %d, type: %d}",
                        pos, src.playData.id, src.playData.playDataType);
    }
}

//========================================================
void ContentsPlayer::AddPlayDataSlot(const PlayJson::FixedPlayDataJson& src)
//========================================================
{
    int pos = -1;
    QVector<PlayJson::FixedPlayDataJson>::const_iterator iter = m_fixedPlayDataList.constBegin();
    while(iter != m_fixedPlayDataList.constEnd())
    {
        if((*iter).playData.id == src.playData.id)
        {
            pos = iter - m_fixedPlayDataList.constBegin();
            break;
        }
        ++iter;
    }

    if(-1 == pos)
    {
        m_fixedPlayDataList.push_back(src);
        ELGO_VIEWER_LOG("ADD PlayData - {id: %d, type: %d}",
                        src.playData.id, src.playData.playDataType);
    }
    else
    {
        m_fixedPlayDataList[pos] = src;
        ELGO_VIEWER_LOG("[Replace] Existed Data - {pos: %d, id: %d, type: %d}",
                        pos, src.playData.id, src.playData.playDataType);
    }
}

//========================================================
void ContentsPlayer::RotateScreenSlot(const VIEWER_DEF::HEADING heading)
//========================================================
{
    ELGO_VIEWER_LOG("Heading: %d", heading);

    if(m_heading != heading)
    {
        if(VIEWER_DEF::HEADING::TOP == heading)
        {

        }
        else if(VIEWER_DEF::HEADING::RIGHT == heading)
        {

        }
        else if(VIEWER_DEF::HEADING::BOTTOM == heading)
        {

        }
        else if(VIEWER_DEF::HEADING::LEFT == heading)
        {

        }
        else
        {
            ELGO_VIEWER_LOG("Error - Unknown Heading: %d", heading);
        }

        m_heading = heading;
    }
    else
    {
        ELGO_VIEWER_LOG("Not Rotation Screen: {curr: %d, heading: %d}",
                        m_heading, heading);
    }
}

//========================================================
void ContentsPlayer::UpdatePlayerPauseSlot(const bool bIsPause)
//========================================================
{
    const int playDataId = m_playingIndex.playData.id;
    const PlayJson::PlayDataType playType = m_playingIndex.playData.playDataType;

    if(true == bIsPause)
    {
        QVector<VideoItemInfo>::iterator videoIter = m_videoItemList.begin();
        while(videoIter != m_videoItemList.end())
        {
            if( (playDataId == videoIter->first.playData.id) &&
                (playType == videoIter->first.playData.playDataType) )
            {
                videoIter->second->PauseVideoItem();
            }

            ++videoIter;
        }

        QVector<SubtitleWidgetInfo>::iterator subIter = m_subtitleWidgetList.begin();
        while(subIter != m_subtitleWidgetList.end())
        {
            if( (playDataId == subIter->first.playData.id) &&
                (playType == subIter->first.playData.playDataType) )
            {
                subIter->second->StopAnimation();
            }

            ++subIter;
        }

        m_playerTimer.stop();
    }
    else
    {
        QVector<VideoItemInfo>::iterator videoIter = m_videoItemList.begin();
        while(videoIter != m_videoItemList.end())
        {
            if( (playDataId == videoIter->first.playData.id) &&
                (playType == videoIter->first.playData.playDataType) )
            {
                videoIter->second->PlayVideoItem();
            }

            ++videoIter;
        }

        QVector<SubtitleWidgetInfo>::iterator subIter = m_subtitleWidgetList.begin();
        while(subIter != m_subtitleWidgetList.end())
        {
            if( (playDataId == subIter->first.playData.id) &&
                (playType == subIter->first.playData.playDataType) )
            {
                subIter->second->StartAnimation();
            }

            ++subIter;
        }

        m_playerTimer.start(990);
    }
}

//========================================================
void ContentsPlayer::ExecPlayDataSlot(const PlayJson::PlayData& playData,
                                      const bool bDelPrevData)
//========================================================
{
    m_playerTimer.stop();

    ELGO_VIEWER_LOG("Ready to Play - {id: %d, type: %d}",
                    playData.id, playData.playDataType);

    // Get Play Data Index
    GetSuitablePlayDataIndex(playData);
    if(NOT_EXISTED_DATA == m_playDataIndex)
    {
        ELGO_VIEWER_LOG("Error - Not Existed Play Data {id: %d, type: %d",
                        playData.id, playData.playDataType);
        return;
    }

    // Make Countdown Info
    PlayScheduleTimer::PlayingIndex playingIndex;
    playingIndex.playData = playData;
    MakePlayDataCountdownInfo(playingIndex);

    // Update Display Scene
    UpdatePlayerScene(playingIndex, bDelPrevData);

    m_playerTimer.start(990);
}

//========================================================
void ContentsPlayer::UpdatePlayerScene(const PlayScheduleTimer::PlayingIndex& playingIndex,
                                       const bool bDelPrevData)
//========================================================
{
    QGraphicsScene *newScene = new QGraphicsScene(this);
    newScene->setSceneRect(m_screenRect);

    PlayScheduleTimer::PlayingIndex prePlayingIndex = m_playingIndex;

    // Delete
    if(0 != prePlayingIndex.playData.id)
    {
        PauseItemAndWidgetContents(prePlayingIndex);
        if(NULL != m_currScene.second)
        {
            QList<QGraphicsItem *> items = m_currScene.second->items();

            foreach(auto item, items)
            {
                m_currScene.second->removeItem(item);
            }
            m_currScene.second->deleteLater();
        }
        ClearPrevPlayingData(prePlayingIndex);
    }

    if( (true == bDelPrevData) && (0 != prePlayingIndex.playData.id) )
    {
        if( ( PlayJson::PlayDataType::FIXED == prePlayingIndex.playData.playDataType &&
              m_playingIndex.playData.id != playingIndex.playData.id ) ||
            ( PlayJson::PlayDataType::CUSTOM == prePlayingIndex.playData.playDataType &&
              m_playingIndex.playData.id != playingIndex.playData.id) )
        {
            ClearOtherPlayDataJsonInfo(playingIndex);
            ClearOtherPlayDataItem(playingIndex);
        }
    }

    // ADD
    if(PlayJson::PlayDataType::CUSTOM == playingIndex.playData.playDataType)
    {
        SearchContentAndAddToScene(playingIndex, newScene);
        PlayItemAndWidgetContents(playingIndex);
    }
    else if(PlayJson::PlayDataType::FIXED == playingIndex.playData.playDataType)
    {
        PlayScheduleTimer::PlayingIndex fixedPlayingIndex = playingIndex;
        for(int layIdx = 0; layIdx < m_playCountdown.maxLayer; layIdx++)
        {
            SearchContentAndAddToScene(fixedPlayingIndex, newScene);
            PlayItemAndWidgetContents(fixedPlayingIndex);
            fixedPlayingIndex.layerIdx++;
        }

        // Make Subtitle
        const PlayJson::FixedPlayDataJson& fixedPlayData = m_fixedPlayDataList[m_playDataIndex];
        const int subtitleListSize = fixedPlayData.subtitleDataList.size();
        for(int subIdx = 0; subIdx < subtitleListSize; subIdx++)
        {
            PlayScheduleTimer::PlayingIndex subtitlePlayingIndex = playingIndex;
            subtitlePlayingIndex.mediaType = PlayJson::MediaType::SUBTITLE;

            MakeSubtitleTypeContent(subtitlePlayingIndex, fixedPlayData.subtitleDataList[subIdx], newScene);
            PlayItemAndWidgetContents(subtitlePlayingIndex);
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

    ELGO_VIEWER_LOG("Made New Scene { id: %d, pageIdx: %d, sceneItemSize: %d }",
                    playingIndex.playData.id, playingIndex.pageIdx, newScene->items().size());
    ELGO_VIEWER_LOG("Item / Wiget {image: %d, video: %d, clock: %d, date: %d}",
                    m_imageItemList.size(), m_videoItemList.size(),
                    m_clockWidgetList.size(), m_dateWidgetList.size());
    ELGO_VIEWER_LOG("Item / Wiget {news: %d, weather: %d, subtitle: %d, proxy: %d}",
                    m_newsFeedWigetList.size(), m_weatherWidgetList.size(),
                    m_subtitleWidgetList.size(), m_proxyWidgetList.size());

    ui->playerView->setScene(newScene);
    m_playingIndex = playingIndex;

    SceneInfo newSceneInfo(playingIndex, newScene);
    m_currScene = newSceneInfo;
}

//========================================================
void ContentsPlayer::SearchContentAndAddToScene(const PlayScheduleTimer::PlayingIndex& playingIndex,
                                                QGraphicsScene* dest)
//========================================================
{
    if(NULL != dest)
    {
        // Make Custom or Fixed Contents
        if(PlayJson::PlayDataType::CUSTOM == playingIndex.playData.playDataType)
        {
            MakeCustomPlayDataContents(playingIndex, dest);
        }
        else if(PlayJson::PlayDataType::FIXED == playingIndex.playData.playDataType)
        {
            MakeFixedPlayDataContents(playingIndex, dest);
        }
        else
        {
            ELGO_VIEWER_LOG("Error - Unknwon PlayData {id: %d, type: %d}",
                            playingIndex.playData.id,
                            playingIndex.playData.playDataType);
        }
    }
    else
    {
        ELGO_VIEWER_LOG("Error - Scene is NULL");
    }
}

//========================================================
void ContentsPlayer::GetSuitablePlayDataIndex(const PlayJson::PlayData& playData)
//========================================================
{
    if(PlayJson::PlayDataType::CUSTOM == playData.playDataType)
    {
        const int playDataListSize = m_customPlayDataList.size();
        for(int idx = 0; idx < playDataListSize; idx++)
        {
            if(playData.id == m_customPlayDataList[idx].playData.id)
            {
                m_playDataIndex = idx;
                ELGO_VIEWER_LOG("Success Finding - {id: %d, type: %d, idx: %d}",
                                playData.id, playData.playDataType, m_playDataIndex);
                break;
            }
        }
    }
    else if(PlayJson::PlayDataType::FIXED == playData.playDataType)
    {
        const int playDataListSize = m_fixedPlayDataList.size();
        for(int idx = 0; idx < playDataListSize; idx++)
        {
            if(playData.id == m_fixedPlayDataList[idx].playData.id)
            {
                m_playDataIndex = idx;
                ELGO_VIEWER_LOG("Success Finding - {id: %d, type: %d, idx: %d}",
                                playData.id, playData.playDataType, m_playDataIndex);
                break;
            }
        }
    }
    else
    {
        ELGO_VIEWER_LOG("Error - Unknown Data Type: %d", playData.playDataType);
    }
}

//========================================================
void ContentsPlayer::MakePlayDataCountdownInfo(const PlayScheduleTimer::PlayingIndex& playingIndex)
//========================================================
{
    PlayScheduleTimer::CountdownInfo countdownInfo;

    if(PlayJson::PlayDataType::CUSTOM == playingIndex.playData.playDataType)
    {
        const PlayJson::CustomPlayDataJson& customPlayData = m_customPlayDataList[m_playDataIndex];

        countdownInfo.id = customPlayData.playData.id;
        countdownInfo.type = customPlayData.playData.playDataType;
        countdownInfo.maxPage = customPlayData.pageDataList.size();
        countdownInfo.pageTimecount.resize(countdownInfo.maxPage);
        ELGO_VIEWER_LOG("Custom Countdown - {id: %d, type: %d, maxPage: %d}",
                        countdownInfo.id, countdownInfo.type, countdownInfo.maxPage);
    }
    else if(PlayJson::PlayDataType::FIXED == playingIndex.playData.playDataType)
    {
        const PlayJson::FixedPlayDataJson& fixedPlayData = m_fixedPlayDataList[m_playDataIndex];

        countdownInfo.id = fixedPlayData.playData.id;
        countdownInfo.type = fixedPlayData.playData.playDataType;
        countdownInfo.maxLayer = fixedPlayData.layerDataList.size();
        countdownInfo.layerContentIdxList.resize(countdownInfo.maxLayer);
        countdownInfo.layerTimecountList.resize(countdownInfo.maxLayer);
        ELGO_VIEWER_LOG("Fixed Countdown - {id: %d, type: %d, maxLayer: %d}",
                        countdownInfo.id, countdownInfo.type, countdownInfo.maxLayer);

        const int layerListSize = fixedPlayData.layerDataList.size();
        for(int layIdx = 0; layIdx < layerListSize; layIdx++)
        {
            const PlayJson::FixedLayerData& layerData = fixedPlayData.layerDataList[layIdx];

            // Set layer timeout info
            const int maxContentSize = layerData.contentDataList.size();
            countdownInfo.layerTimecountList[layIdx].maxContent = maxContentSize;
            countdownInfo.layerTimecountList[layIdx].contentTimeout.resize(maxContentSize);
            ELGO_VIEWER_LOG("Fixed Layer Countdown - {id: %d, type: %d, layIdx: %d, maxContent: %d}",
                            countdownInfo.id, countdownInfo.type, layIdx, maxContentSize);
        }
    }
    else
    {
        ELGO_VIEWER_LOG("Unknwon PlayData Type: %d", playingIndex.playData.playDataType);
        return;
    }

    m_playCountdown = countdownInfo;
}

//========================================================
void ContentsPlayer::MakeCustomPlayDataContents(const PlayScheduleTimer::PlayingIndex& playingIndex,
                                                QGraphicsScene* dest)
//========================================================
{
    if(true == m_customPlayDataList.empty())
    {
        ELGO_VIEWER_LOG("Error - m_customPlayDataList is Empty()");
        return;
    }

    // Set play data
    const PlayJson::CustomPlayDataJson& customPlayData = m_customPlayDataList[m_playDataIndex];

    // Make item or widget content
    const PlayJson::PageData& pageData = customPlayData.pageDataList[playingIndex.pageIdx];

    const int layerListSize = pageData.layerDataList.size();
    for(int layIdx = 0; layIdx < layerListSize; layIdx++)
    {
        const PlayJson::CustomLayerData& layerData = pageData.layerDataList[layIdx];

        // Set pos and size
        StyleSheet::PosSizeInfo posSize;
        posSize.pos = QPoint(layerData.left, layerData.top);
        posSize.size = QSize(layerData.width, layerData.height);

        // Set media type
        PlayScheduleTimer::PlayingIndex contentKeyIndex = playingIndex;
        contentKeyIndex.mediaType = layerData.layerContent.contentInfo.mediaType;

        // Make Content
        if(PlayJson::ContentType::FILE == layerData.layerContent.contentInfo.contentType)
        {
            MakeFileTypeContent(playingIndex, layerData.layerContent, posSize, dest);
        }
        else if(PlayJson::ContentType::WIDGET == layerData.layerContent.contentInfo.contentType)
        {
            MakeWidgetTypeContent(playingIndex, layerData.layerContent, posSize, dest);
        }
        else
        {
            ELGO_VIEWER_LOG("Error - Unknwon Content Type: %d", layerData.layerContent.contentInfo.contentType);
        }
    }

    // Make Subtitle
    const int subtitleListSize = pageData.subtitleDataList.size();
    for(int subIdx = 0; subIdx < subtitleListSize; subIdx++)
    {
        PlayScheduleTimer::PlayingIndex subtitleKeyIndex = playingIndex;
        subtitleKeyIndex.mediaType = PlayJson::MediaType::SUBTITLE;

        MakeSubtitleTypeContent(subtitleKeyIndex, pageData.subtitleDataList[subIdx], dest);
    }
}

//========================================================
void ContentsPlayer::MakeFixedPlayDataContents(const PlayScheduleTimer::PlayingIndex& playingIndex,
                                               QGraphicsScene* dest)
//========================================================
{
    if(true == m_fixedPlayDataList.empty())
    {
        ELGO_VIEWER_LOG("ERROR - m_fixedPlayDataList is Empty()");
        return;
    }

    // Set play data
    const PlayJson::FixedPlayDataJson& fixedPlayData = m_fixedPlayDataList[m_playDataIndex];

    // Make item or widget contents
    const PlayJson::FixedLayerData& layerData = fixedPlayData.layerDataList[playingIndex.layerIdx];
    const PlayJson::ContentData& contentData = layerData.contentDataList[playingIndex.contentIdx];

    // Content index info
    PlayScheduleTimer::PlayingIndex fixedDataKeyIndex = playingIndex;
    fixedDataKeyIndex.layerIdx = playingIndex.layerIdx;
    fixedDataKeyIndex.contentIdx = playingIndex.contentIdx;
    fixedDataKeyIndex.mediaType = contentData.contentInfo.mediaType;

    // Set pos and size
    StyleSheet::PosSizeInfo posSize;
    posSize.pos = QPoint(layerData.left, layerData.top);
    posSize.size = QSize(layerData.width, layerData.height);

    // Make Content
    if(PlayJson::ContentType::FILE == contentData.contentInfo.contentType)
    {
        MakeFileTypeContent(fixedDataKeyIndex, contentData, posSize, dest);
    }
    else if(PlayJson::ContentType::WIDGET == contentData.contentInfo.contentType)
    {
        MakeWidgetTypeContent(fixedDataKeyIndex, contentData, posSize, dest);
    }
    else
    {
        ELGO_VIEWER_LOG("Error - Unknwon Content Type: %d", contentData.contentInfo.contentType);
    }
}

//========================================================
void ContentsPlayer::MakeFileTypeContent(const PlayScheduleTimer::PlayingIndex& playingIndex,
                                         const PlayJson::ContentData& contentData,
                                         const StyleSheet::PosSizeInfo& posSize,
                                         QGraphicsScene *dest)
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
            dest->addItem(newImageItem);

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

        dest->addItem(newVideoItem);

        VideoItemInfo videoItemInfo(playingIndex, newVideoItem);
        m_videoItemList.push_back(videoItemInfo);
    }
    else
    {
        ELGO_VIEWER_LOG("Error - Unknwon Media Type: %d", contentData.contentInfo.mediaType);
    }
}

//========================================================
void ContentsPlayer::MakeWidgetTypeContent(const PlayScheduleTimer::PlayingIndex& playingIndex,
                                           const PlayJson::ContentData& contentData,
                                           const StyleSheet::PosSizeInfo& posSize,
                                           QGraphicsScene *dest)
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
        ELGO_VIEWER_LOG("Error - Unknwon Media Type: %d", contentData.contentInfo.mediaType);
        return;
    }

    dest->addItem(newProxyWidget);
}

//========================================================
void ContentsPlayer::MakeSubtitleTypeContent(const PlayScheduleTimer::PlayingIndex& playingIndex,
                                             const PlayJson::SubtitleData& subtitleData,
                                             QGraphicsScene *dest)
//========================================================
{
    QGraphicsProxyWidget *newProxyWidget = new QGraphicsProxyWidget;
    SubtitleWidget *newSubtitle = new SubtitleWidget;

    newSubtitle->MakeSubTitleWidget(subtitleData);
    newProxyWidget->setWidget(newSubtitle);
    newProxyWidget->setZValue(100); // top level

    dest->addItem(newProxyWidget);

    SubtitleWidgetInfo subtitleWidgetInfo(playingIndex, newSubtitle);
    m_subtitleWidgetList.push_back(subtitleWidgetInfo);

    ProxyWidgetInfo proxyWidgetInfo(playingIndex, newProxyWidget);
    m_proxyWidgetList.push_back(proxyWidgetInfo);
}

//========================================================
void ContentsPlayer::PlayItemAndWidgetContents(const PlayScheduleTimer::PlayingIndex& playingIndex)
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
void ContentsPlayer::PauseItemAndWidgetContents(const PlayScheduleTimer::PlayingIndex& playingIndex,
                                                const bool bIsFixedTimeout)
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
    if(false == bIsFixedTimeout)
    {
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
}

//========================================================
void ContentsPlayer::RemoveItemAndWidgetFromScene(const PlayScheduleTimer::PlayingIndex& playingIndex,
                                                  const bool bIsFixedTimeout)
//========================================================
{
    // Video
    QVector<VideoItemInfo>::iterator videoIter = m_videoItemList.begin();
    for(; videoIter != m_videoItemList.end(); ++videoIter)
    {
        const bool bIsSameIndex = ComparePlayingIndex(playingIndex, videoIter->first);
        if(true == bIsSameIndex)
        {
            m_currScene.second->removeItem(videoIter->second);
            ELGO_VIEWER_LOG("Remove Item - {id: %d, playDataType: %d, mediaType:%d, name: %s}",
                            videoIter->first.playData.id, videoIter->first.playData.playDataType,
                            videoIter->first.mediaType, videoIter->second->GetVideoFileName().toStdString().c_str());
            return;
        }
    }

    // Image
    QVector<ImageItemInfo>::iterator imageIter = m_imageItemList.begin();
    for(; imageIter != m_imageItemList.end(); ++imageIter)
    {
        const bool bIsSameIndex = ComparePlayingIndex(playingIndex, imageIter->first);
        if(true == bIsSameIndex)
        {
            m_currScene.second->removeItem(imageIter->second);
            ELGO_VIEWER_LOG("Remove Item - {id: %d, playDataType: %d, mediaType:%d, name: %s}",
                            imageIter->first.playData.id, imageIter->first.playData.playDataType,
                            imageIter->first.mediaType, imageIter->second->GetImageFileName().toStdString().c_str());
            return;
        }
    }

    // Proxy Widget
    QVector<ProxyWidgetInfo>::iterator proxyIter = m_proxyWidgetList.begin();
    for(; proxyIter != m_proxyWidgetList.end(); ++proxyIter)
    {
        const bool bIsSameIndex = ComparePlayingIndex(playingIndex, proxyIter->first);
        if(true == bIsSameIndex)
        {
            if( (true == bIsFixedTimeout) &&
                (PlayJson::MediaType::SUBTITLE == proxyIter->first.mediaType) )
            {
                continue;
            }
            m_currScene.second->removeItem(proxyIter->second);
            ELGO_VIEWER_LOG("Remove Item - {id: %d, playDataType: %d, mediaType:%d, name: %s}",
                            proxyIter->first.playData.id, proxyIter->first.playData.playDataType,
                            proxyIter->first.mediaType, proxyIter->second->objectName().toStdString().c_str());
            return;
        }
    }
}

//========================================================
void ContentsPlayer::ClearOtherPlayDataJsonInfo(const PlayScheduleTimer::PlayingIndex& playingIndex)
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
}

//========================================================
void ContentsPlayer::ClearOtherPlayDataItem(const PlayScheduleTimer::PlayingIndex& playingIndex)
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
void ContentsPlayer::ClearPrevPlayingData(const PlayScheduleTimer::PlayingIndex& playingIndex,
                                          const bool bIsFixedTimeout)
//========================================================
{
    // image
    const int prevImageListSize = m_imageItemList.size();

    QVector<ImageItemInfo>::iterator imageIter = m_imageItemList.begin();
    for(; imageIter != m_imageItemList.end(); )
    {
        const bool bIsSameItem = ComparePlayingIndex(playingIndex, imageIter->first);
        if( true == bIsSameItem )
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
        const bool bIsSameItem = ComparePlayingIndex(playingIndex, videoIter->first);
        if( true == bIsSameItem )
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
        const bool bIsSameItem = ComparePlayingIndex(playingIndex, clockIter->first);
        if( true == bIsSameItem )
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
        const bool bIsSameItem = ComparePlayingIndex(playingIndex, dateIter->first);
        if( true == bIsSameItem )
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
        const bool bIsSameItem = ComparePlayingIndex(playingIndex, newsIter->first);
        if( true == bIsSameItem )
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
        const bool bIsSameItem = ComparePlayingIndex(playingIndex, weatherIter->first);
        if( true == bIsSameItem )
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

    if(false == bIsFixedTimeout)
    {
        QVector<SubtitleWidgetInfo>::iterator subIter = m_subtitleWidgetList.begin();
        for( ; subIter != m_subtitleWidgetList.end(); )
        {
            const bool bIsSameItem = ComparePlayingIndex(playingIndex, subIter->first);
            if( true == bIsSameItem )
            {
                subIter->second->deleteLater();
                subIter = m_subtitleWidgetList.erase(subIter);
            }
            else
            {
                ++subIter;
            }
        }
    }

    // proxy
    const int prevProxyListSize = m_proxyWidgetList.size();

    QVector<ProxyWidgetInfo>::iterator proxyIter = m_proxyWidgetList.begin();
    for( ; proxyIter != m_proxyWidgetList.end(); )
    {
        const bool bIsSameItem = ComparePlayingIndex(playingIndex, proxyIter->first);
        if( true == bIsSameItem )
        {
            if( (true == bIsFixedTimeout) &&
                (PlayJson::MediaType::SUBTITLE == proxyIter->first.mediaType) )
            {
                ++proxyIter;
                continue;
            }
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
    // SinglePlay Data's timeout
    if(PlayJson::PlayDataType::CUSTOM == m_playingIndex.playData.playDataType)
    {
        // Change Page
        if( 1 == m_playCountdown.maxPage )
        {
            ELGO_VIEWER_LOG("Not Update Scene - Max Page Size: %d",
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

                    PlayScheduleTimer::PlayingIndex nextPlayingIndex = m_playingIndex;
                    nextPlayingIndex.pageIdx = nextPageIdx;
                    ELGO_VIEWER_LOG("Update NextPage - { id: %d, pageIdx: %d, currPageTimeCnt{ %d : %d } }",
                                    nextPlayingIndex.playData.id, nextPlayingIndex.pageIdx,
                                    m_playCountdown.pageTimecount[currPageIdx], currPageData.duration);

                    UpdatePlayerScene(nextPlayingIndex, m_playDataIndex);
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
        // Add, Remove Item
        const PlayJson::FixedPlayDataJson& fixdPlayData = m_fixedPlayDataList[m_playDataIndex];

        const int layerListSize = fixdPlayData.layerDataList.size();
        for(int layIdx = 0; layIdx < layerListSize; layIdx++)
        {
            const PlayJson::FixedLayerData& layerData = fixdPlayData.layerDataList[layIdx];
            const int contentListSize = layerData.contentDataList.size();

            // Not Need Update
            if(1 == contentListSize)
            {
                ELGO_VIEWER_LOG("Not Update Layer - {layerIdx: %d, contentListSize: %d}",
                                layIdx, contentListSize);
                continue;
            }

            // Check Timecount
            const int currContentIdx = m_playCountdown.layerContentIdxList[layIdx];
            const int currContentTimecount = m_playCountdown.layerTimecountList[layIdx].contentTimeout[currContentIdx];

            if(currContentTimecount >= layerData.contentDataList[currContentIdx].userDuration)
            {
                // Change Content in layer[layIdx]
                m_playCountdown.layerTimecountList[layIdx].contentTimeout[currContentIdx] = 0;

                // Ready to Next Content
                int nextContentIdx = currContentIdx + 1;
                if(m_playCountdown.layerTimecountList[layIdx].maxContent <= nextContentIdx)
                {
                    nextContentIdx = 0;
                }
                ELGO_VIEWER_LOG("Update Next Content - {layerIdx: %d, contentIdx: %d -> %d}",
                                layIdx, currContentIdx, nextContentIdx);

                PlayScheduleTimer::PlayingIndex prevPlayingIndex;
                prevPlayingIndex = m_playingIndex;
                prevPlayingIndex.layerIdx = layIdx;
                prevPlayingIndex.contentIdx = currContentIdx;

                PlayScheduleTimer::PlayingIndex nextPlayingIndex;
                nextPlayingIndex = m_playingIndex;
                nextPlayingIndex.layerIdx = layIdx;
                nextPlayingIndex.contentIdx = nextContentIdx;

                // Update
                UpdateNextFixedLayerContent(prevPlayingIndex, nextPlayingIndex);
                m_playCountdown.layerContentIdxList[layIdx] = nextContentIdx;
            }
            else
            {
                // Plus Countdown
                m_playCountdown.layerTimecountList[layIdx].contentTimeout[currContentIdx]++;
                ELGO_VIEWER_LOG("Fixd Layer[%d] Content[%d] Countdown : %d, userDuration : %d",
                                layIdx, currContentIdx,
                                m_playCountdown.layerTimecountList[layIdx].contentTimeout[currContentIdx],
                                layerData.contentDataList[currContentIdx].userDuration);
            }
        }
    }
    else
    {
        ELGO_VIEWER_LOG("Error - Timeout method is not need {id: %d, type: %d}",
                        m_playingIndex.playData.id, m_playingIndex.playData.playDataType);
    }
}

//========================================================
void ContentsPlayer::UpdateNextFixedLayerContent(const PlayScheduleTimer::PlayingIndex& prevPlayingIndex,
                                                 const PlayScheduleTimer::PlayingIndex& nextPlayingIndex)
//========================================================
{

    if(NULL != m_currScene.second)
    {
        // ADD Item or Widget
        SearchContentAndAddToScene(nextPlayingIndex, m_currScene.second);
        PlayItemAndWidgetContents(nextPlayingIndex);

        // Remove Item or Widget
        PauseItemAndWidgetContents(prevPlayingIndex, true);
        RemoveItemAndWidgetFromScene(prevPlayingIndex, true);
        ClearPrevPlayingData(prevPlayingIndex, true);
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
        ELGO_VIEWER_LOG("Error - Unknwon MediaType : %d", src);
    }
}

//========================================================
bool ContentsPlayer::ComparePlayingIndex(const PlayScheduleTimer::PlayingIndex& lhs,
                                         const PlayScheduleTimer::PlayingIndex& rhs)
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
