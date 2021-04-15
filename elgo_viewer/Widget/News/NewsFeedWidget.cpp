
// Viewer
#include "NewsFeedWidget.h"
#include "ui_NewsFeedWidget.h"
#include "Logger/ViewerLogger.h"

//========================================================
NewsFeedWidget::NewsFeedWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::NewsFeedWidget)
    , m_bIsStartedAnimation(false)
//========================================================
{
    ui->setupUi(this);
    this->setObjectName("NewsFeedWidget");

    m_feedScene = new QGraphicsScene(this);
    m_seqAniGroup = new QSequentialAnimationGroup(this);

    ui->feedView->setScene(m_feedScene);
//    ui->feedView->setStyleSheet("border: 0px");
    ui->feedView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->feedView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->feedView->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    ui->feedView->setRenderHint(QPainter::Antialiasing);
}

//========================================================
NewsFeedWidget::~NewsFeedWidget()
//========================================================
{
    delete ui;

    delete m_seqAniGroup;
    m_seqAniGroup = NULL;
}

//========================================================
void NewsFeedWidget::SetNewsFeedList(const PlayJson::NewsCategory category, const QVector<QString>& feedList)
//========================================================
{
    m_category = category;

    const int feedListSize = feedList.size();
    for(int idx = 0; idx < feedListSize; idx++)
    {
        FeedBoxWidget *feedbox = new FeedBoxWidget();

        QString feedStr = "  ";
        feedStr += feedList[idx];
        feedbox->SetFeedBoxText(feedStr);

        m_feedBoxList.push_back(feedbox);
    }
}

//========================================================
void NewsFeedWidget::SetWidgetStyleSheet(const StyleSheet::StyleInfo &style)
//========================================================
{
    m_widgetStyle = style;

    QString widgetStyleSheet = QString("QWidget {background-color: %1;}")
                                .arg(m_widgetStyle.backgroundColor);
    ELGO_VIEWER_LOG("Widget StyleSheet - %s", widgetStyleSheet.toStdString().c_str());

    if(true == m_widgetStyle.bTransparency)
    {
        this->setAttribute(Qt::WA_TranslucentBackground, true);
    }
    else
    {
        this->setStyleSheet(widgetStyleSheet);
    }
}

//========================================================
void NewsFeedWidget::SetFeedLabelStyleSheet(const StyleSheet::StyleInfo &style)
//========================================================
{
    m_feedStyle = style;

    // title - white
    QString titleStyleSheet = QString("QLabel {color: %1}").arg(style.fontColor);
    ui->newsTitle->setStyleSheet(titleStyleSheet);
    ELGO_VIEWER_LOG("Title StyleSheet - %s", titleStyleSheet.toStdString().c_str());

    QString feedStyleSheet = QString("QLabel {background-color: %1; color: %2;}")
                                .arg(m_feedStyle.backgroundColor)
                                .arg(m_feedStyle.fontColor);
    ELGO_VIEWER_LOG("Feed StyleSheet - %s", feedStyleSheet.toStdString().c_str());

    // news feed
    const int feedListSize = m_feedBoxList.size();
    for(int idx = 0; idx < feedListSize; idx++)
    {
        if(true == m_feedStyle.bTransparency)
        {
            m_feedBoxList[idx]->SetTranslucentBackground(true);
        }
        else
        {
            m_feedBoxList[idx]->SetLabelStyleSheet(feedStyleSheet);
        }
    }
}

//========================================================
void NewsFeedWidget::SetPosSizeInfo(const StyleSheet::PosSizeInfo& posSizeInfo)
//========================================================
{
    m_widgetPosSize = posSizeInfo;

    const QPointF& widgetPos = posSizeInfo.pos;
    const QSize& widgetSize = posSizeInfo.size;
    const QRect widgetRect(widgetPos.toPoint(), widgetSize);
    ELGO_VIEWER_LOG("Widget pos{x: %d, y: %d}, size{w: %d, h: %d}",
                    widgetRect.x(), widgetRect.y(), widgetRect.width(), widgetRect.height());
    this->setGeometry(widgetRect);

    // title
    const QPoint titlePos(widgetSize.width() * 0.01, 0);
    const QSize titleSize(widgetSize.width() * 0.9, widgetSize.height() * 0.1);
    const QRect titleRect(titlePos, titleSize);
    ELGO_VIEWER_LOG("Title pos{x: %d, y: %d}, size{w: %d, h: %d}",
                    titleRect.x(), titleRect.y(), titleRect.width(), titleRect.height());

    QFont titleFont;
    const int titleFontSize = titleSize.height() * 0.75;
    titleFont.setBold(true);
    titleFont.setPointSize(titleFontSize);
    ELGO_VIEWER_LOG("Title Font Size : %d", titleFontSize);

    ui->newsTitle->setGeometry(titleRect);
    ui->newsTitle->setFont(titleFont);

    // Scene And View
    const QPoint scenePos(titlePos.x(), titleSize.height() * 1.2);
    const QSize sceneSize(widgetSize.width() * 0.85, widgetSize.height() * 0.95);
    const QRect sceneRect(scenePos, sceneSize);
    ELGO_VIEWER_LOG("Scene pos{x: %d, y: %d}, size{w: %d, h: %d}",
                    sceneRect.x(), sceneRect.y(), sceneRect.width(), sceneRect.height());

    m_feedScene->setSceneRect(sceneRect);
    ui->feedView->setGeometry(sceneRect);

    // news Feed    
    const QSize feedSize = QSize(sceneSize.width(), sceneSize.height() * 0.06);

    QFont feedFont;
    const int feedFontSize = feedSize.height() * 0.7;
    feedFont.setBold(true);
    feedFont.setPointSize(feedFontSize);
    ELGO_VIEWER_LOG("Feed Font Size : %d", feedFontSize);

    int feedPosX = sceneRect.x();
    int feedPosY = sceneRect.y();

    const int feedListSize = m_feedBoxList.size();
    for(int idx = 0; idx < feedListSize; idx++)
    {
        const QPoint originFeedPos(feedPosX, feedPosY);;
        const QRect originFeedRect(originFeedPos, feedSize);
        ELGO_VIEWER_LOG("Feed pos{x: %d, y: %d}, size{w: %d, h: %d}",
                        originFeedPos.x(), originFeedPos.y(), feedSize.width(), feedSize.height());

        const int chagnedFeedPosY = feedPosY + feedSize.height();
        const QPoint changedFeedPos(feedPosX, chagnedFeedPosY);
        const QSize chagnedFeedSize(feedSize.width(), 0);
        const QRect changedFeedRect(changedFeedPos, chagnedFeedSize);
        ELGO_VIEWER_LOG("Changed Feed pos{x: %d, y: %d}", changedFeedPos.x(), changedFeedPos.y());

        m_feedBoxList[idx]->SetFeedBoxGeometry(originFeedRect, changedFeedRect);
        m_feedBoxList[idx]->SetLabelFont(feedFont);

        m_feedScene->addWidget(m_feedBoxList[idx]);

        feedPosY += widgetSize.height() * 0.08;
    }
}

//========================================================
void NewsFeedWidget::MakeNewsFeedWidget()
//========================================================
{
    // title
    QString title = QString("오늘의 %1 소식")
                        .arg(ConvertCategoryEnumToKrStr(m_category));
    ui->newsTitle->setText(title);

    // animation
    const int newsFeedListSize = m_feedBoxList.size();
    for(int idx = 0; idx < newsFeedListSize; idx++)
    {
        // check text overflow
        m_feedBoxList[idx]->CheckTextOverflowStatus();
        m_feedBoxList[idx]->MakeFeedBoxAnimation();

        m_seqAniGroup->setLoopCount(1000);
        m_seqAniGroup->addAnimation(m_feedBoxList[idx]->GetParallelAnimationGroup());
        if(true == m_feedBoxList[idx]->GetTextOverflowStatus())
        {
            m_seqAniGroup->addAnimation(m_feedBoxList[idx]->GetOverflowStartAnimation());
            m_seqAniGroup->addAnimation(m_feedBoxList[idx]->GetOverflowEndAnimation());
            ELGO_VIEWER_LOG("FeedBox[%d] make overflow animation", idx);
        }
    }

    QPropertyAnimation *waitNextAni = new QPropertyAnimation(this);
    waitNextAni->setDuration(20000);
    m_seqAniGroup->addAnimation(waitNextAni);
}

//========================================================
void NewsFeedWidget::StartAnimation()
//========================================================
{
    m_seqAniGroup->start();
    m_bIsStartedAnimation = true;
}

//========================================================
void NewsFeedWidget::StopAnimation()
//========================================================
{
    m_seqAniGroup->stop();
    m_bIsStartedAnimation = false;
}

//========================================================
bool NewsFeedWidget::IsStartedAnimation()
//========================================================
{
    return m_bIsStartedAnimation;
}

//========================================================
QString NewsFeedWidget::ConvertCategoryEnumToKrStr(const PlayJson::NewsCategory category)
//========================================================
{
    const char* krStrArray[] = {"NONE", "최신", "헤드라인",
                                "정치", "경제", "사회",
                                "스포츠", "문화연예", "지역",
                                "세계", "날씨"};

    return krStrArray[category];
}
