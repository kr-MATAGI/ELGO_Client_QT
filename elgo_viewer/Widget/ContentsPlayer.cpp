// QT
#include <QRect>
#include <QScreen>

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
    m_scene = new QGraphicsScene(this);
    m_scene->setSceneRect(screenRect);

    ui->playerView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->playerView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->playerView->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    ui->playerView->setRenderHint(QPainter::Antialiasing);
    ui->playerView->setScene(m_scene);

}

//========================================================
ContentsPlayer::~ContentsPlayer()
//========================================================
{
    delete ui;

    delete m_scene;
    m_scene = NULL;

    // pixmap
    foreach(auto item, m_pixmapList)
    {
        delete item;
        item = NULL;
    }
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
bool ContentsPlayer::GetCurrentScreenCapture()
//========================================================
{
    bool retValue = true;

    QScreen *screen = QGuiApplication::primaryScreen();
    if(NULL == screen)
    {
        retValue = false;
        ELGO_VIEWER_LOG("ERROR - NULL == screen");
    }
    else
    {
        QString path = CAPATURE_SAVE_PATH;
        path.append("screenCapture.png");
        QPixmap pixmap = screen->grabWindow(0);
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

    delete screen;
    screen = NULL;

    return retValue;
}

//========================================================
void ContentsPlayer::StartContentsPlayer()
//========================================================
{
    // TODO : Check Schdule and Display Contents
    ELGO_VIEWER_LOG("Start Contents Player");
    this->showFullScreen();
}

//========================================================
void ContentsPlayer::DisplayImageContent(const QString& path, const QPointF& pos, const QSize& size)
//========================================================
{
    QPixmap pixmap(path);
    QPixmap scaledPixmap = pixmap.scaled(size, Qt::IgnoreAspectRatio);

    QGraphicsPixmapItem *pixmapItem = new QGraphicsPixmapItem();
    pixmapItem->setPos(pos);
    pixmapItem->setPixmap(scaledPixmap);
    m_pixmapList.push_back(pixmapItem);
}
