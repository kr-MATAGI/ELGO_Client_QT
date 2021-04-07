// QT
#include <QRect>
#include <QScreen>
#include <QWindow>
#include <QGraphicsScene>
#include <QGraphicsItem>

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
        m_displaySize = screens[0]->size();
        ELGO_VIEWER_LOG("Screen Size {w : %d, h : %d}", m_displaySize.width(), m_displaySize.height());
    }

    const QRect screenRect = QRect(QPoint(0.0, 0.0), m_displaySize);
    ui->playerView->setGeometry(screenRect);
    ui->playerView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->playerView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->playerView->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    ui->playerView->setRenderHint(QPainter::Antialiasing);
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
    m_schedulerTimer = new SchedulesTimer;
    m_schedulerTimer->start(900);

    m_singleTimer = new SinglePlayTimer;
    m_singleTimer->start(990);
}

//========================================================
void ContentsPlayer::AddPlayDataList(const PlayJson::CustomPlayDataJson& src)
//========================================================
{
    m_singleTimer->AddPlayData(src);
}

//========================================================
void ContentsPlayer::AddPlayDataList(const PlayJson::FixedPlayDataJson& src)
//========================================================
{
    m_singleTimer->AddPlayData(src);
}

//========================================================
void ContentsPlayer::ExecSinglePlayData(const PlayJson::PlayData& src)
//========================================================
{
    SchedulerDef::PlayDataInfo playDataInfo;
    playDataInfo.id = src.id;
    playDataInfo.type = src.playDataType;

    m_singleTimer->ExecPlayData(playDataInfo);
}

//========================================================
void ContentsPlayer::UpdatePlayerNewCustomScene(const SchedulerDef::PlayDataIndexInfo& playDataIdxInfo)
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

                break;
            }
        }

        if(false == bIsFindScene)
        {
            QGraphicsScene *newScene = new QGraphicsScene;
            newScene->setSceneRect(ui->playerView->rect());

            SearchItemAndAddToScene(playDataIdxInfo, newScene);
            ELGO_VIEWER_LOG("Made New Custom Scene {id: %d, pageIdx: %d, itemSize: %d}",
                            playDataIdxInfo.playDataInfo.id, playDataIdxInfo.pageIdx, newScene->items().size());

            ui->playerView->setScene(newScene);

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
void ContentsPlayer::UpdatePlayerNewFixedScene(SchedulerDef::PlayDataIndexInfo& playDataIdxInfo, const int layerCount)
//========================================================
{
    if(PlayJson::PlayDataType::FIXED == playDataIdxInfo.playDataInfo.type)
    {
        QGraphicsScene *newScene = new QGraphicsScene;
        newScene->setSceneRect(ui->playerView->rect());
        for(int idx =0; idx < layerCount; idx++)
        {
            SearchItemAndAddToScene(playDataIdxInfo, newScene);
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
void ContentsPlayer::UpdatePlayerFixedLayerContent(const SchedulerDef::PlayDataIndexInfo& prevDataIdxInfo,
                                                    const SchedulerDef::PlayDataIndexInfo& newDataIdxInfo)
//========================================================
{
    // Find Scene
    QVector<SceneInfo>::iterator sceneIter = m_sceneList.begin();
    for(; sceneIter != m_sceneList.end(); ++sceneIter)
    {
        if( (sceneIter->first.playDataInfo.id == newDataIdxInfo.playDataInfo.id) &&
            (sceneIter->first.playDataInfo.type == newDataIdxInfo.playDataInfo.type) )
        {
            // ADD New Image
            QVector<ImageItemInfo>::iterator imageIter = m_imageItemList.begin();
            // Remove Prev Image

            for(; imageIter != m_imageItemList.end(); ++imageIter)
            {
                if(imageIter->first == prevDataIdxInfo)
                {
                    ui->playerView->scene()->removeItem(imageIter->second);

                    ELGO_VIEWER_LOG("Remove Image Item to Scene : %s, Index {layer: %d, content: %d}",
                                    imageIter->second->GetImageFileName().toUtf8().constData(),
                                    prevDataIdxInfo.layerIdx, prevDataIdxInfo.contentIdx);
                }
            }

            imageIter = m_imageItemList.begin();
            for(; imageIter != m_imageItemList.end(); ++imageIter)
            {
                if(imageIter->first == newDataIdxInfo)
                {
                    ui->playerView->scene()->addItem(imageIter->second);

                    ELGO_VIEWER_LOG("ADD Image Item to Scene : %s, Index {layer: %d, content: %d}",
                                    imageIter->second->GetImageFileName().toUtf8().constData(),
                                    newDataIdxInfo.layerIdx, newDataIdxInfo.contentIdx);
                }
            }

            QVector<VideoItemInfo>::iterator videoIter = m_videoItemList.begin();
            // Remove Prev Video
            for(; videoIter != m_videoItemList.end(); ++videoIter)
            {
                if(videoIter->first == prevDataIdxInfo)
                {
                    ui->playerView->scene()->removeItem(videoIter->second);

                    ELGO_VIEWER_LOG("ADD Video Item to Scene : %s, Index {layer: %d, content: %d}",
                                    videoIter->second->GetVideoFileName().toUtf8().constData(),
                                    prevDataIdxInfo.layerIdx, prevDataIdxInfo.contentIdx);
                }
            }

            // ADD New Video
            videoIter = m_videoItemList.begin();
            for(; videoIter != m_videoItemList.end(); ++videoIter)
            {
                if(videoIter->first == newDataIdxInfo)
                {
                    ui->playerView->scene()->addItem(videoIter->second);

                    ELGO_VIEWER_LOG("ADD Video Item to Scene : %s, Index {layer: %d, content: %d}",
                                    videoIter->second->GetVideoFileName().toUtf8().constData(),
                                    newDataIdxInfo.layerIdx, newDataIdxInfo.contentIdx);
                }
            }



            break;
        }
    }
}

//========================================================
void ContentsPlayer::MakeFileTypeItem(const SchedulerDef::PlayDataIndexInfo& contentIndxInfo,
                      const PlayJson::ContentData& contentData,
                      const StyleSheet::PosSizeInfo& posSizeInfo)
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

        VideoItemInfo newVideoItem;
        newVideoItem.first = contentIndxInfo;
        newVideoItem.second = videoItem;
        m_videoItemList.push_back(newVideoItem);
    }
}

//========================================================
void ContentsPlayer::SearchItemAndAddToScene(const SchedulerDef::PlayDataIndexInfo& playDataIdxInfo,
                                              QGraphicsScene* scene)
//========================================================
{
    if(NULL != scene)
    {
        // Find Image Item
        QVector<ImageItemInfo>::iterator imageIter = m_imageItemList.begin();
        for(; imageIter != m_imageItemList.end(); ++imageIter)
        {
            if(imageIter->first == playDataIdxInfo)
            {
                scene->addItem(imageIter->second);
                ELGO_VIEWER_LOG("ADD Image Item to Scene : %s",
                                imageIter->second->GetImageFileName().toUtf8().constData());
            }
        }

        // Find Widget Item

        // Find Subtitle Item

        // Find Video Item
        QVector<VideoItemInfo>::iterator videoIter = m_videoItemList.begin();
        for(; videoIter != m_videoItemList.end(); ++videoIter)
        {
            if(videoIter->first == playDataIdxInfo)
            {
                scene->addItem(videoIter->second);
                videoIter->second->setZValue(100);
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
    else
    {
        ELGO_VIEWER_LOG("Error - Unkwon MediaType : %d", type);
    }

    return retValue;
}
