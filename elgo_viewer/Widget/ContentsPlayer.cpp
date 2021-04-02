// QT
#include <QRect>
#include <QScreen>
#include <QWindow>

// Viewer
#include "ContentsPlayer.h"
#include "ui_ContentsPlayer.h"
#include "Logger/ViewerLogger.h"

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
        return;
    }
    delete pInstance;
    pInstance = NULL;
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
void ContentsPlayer::UpdatePlayerScene(QGraphicsScene& scene)
//========================================================
{
    ELGO_VIEWER_LOG("Update Scene - {items size : %d}", scene.items().size());
    const QRect screenRect = QRect(QPoint(0.0, 0.0), m_displaySize);

    scene.setSceneRect(screenRect);
    ui->playerView->setScene(&scene);
}

//========================================================
void ContentsPlayer::StartContentsPlayer()
//========================================================
{
    ELGO_VIEWER_LOG("Start Contents Player");
    this->showFullScreen();

    // TEST
//    QGraphicsScene *scene = new QGraphicsScene;
//    scene->setSceneRect(QRect(0,0, 1950, 1080));

//    VideoItem *videoItem = new VideoItem;
//    StyleSheet::PosSizeInfo widgetInfo;
//    widgetInfo.fileName = "Mountains.mp4";
//    widgetInfo.pos = QPointF(0.0, 0.0);
//    widgetInfo.size = QSize(500, 500);

//    videoItem->SetVideoFileToBuffer("/home/jaehoon/바탕화면/ELGO/resource/video/1615956328721_18mb_1920x1080_earth.mp4", VideoInfo::MetaData());
//    videoItem->SetVideoPosAndSize(widgetInfo);

//    scene->addItem(videoItem);
//    videoItem->PlayVideoItem();

//    ui->playerView->setScene(scene);
}
