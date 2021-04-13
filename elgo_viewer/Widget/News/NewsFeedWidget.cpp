
// Viewer
#include "NewsFeedWidget.h"
#include "ui_NewsFeedWidget.h"
#include "Logger/ViewerLogger.h"

#define LABEL_ANIMATION_DURATION  1500 //unit : msec

//========================================================
NewsFeedWidget::NewsFeedWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::NewsFeedWidget)
    , m_bIsStartedAnimation(false)
//========================================================
{
    ui->setupUi(this);
    this->setObjectName("NewsFeedWidget");

    m_seqAniGroup = new QSequentialAnimationGroup(this);
}

//========================================================
NewsFeedWidget::~NewsFeedWidget()
//========================================================
{
    delete ui;

    foreach(auto item, m_newsFeedList)
    {
        delete item;
        item = NULL;
    }
    m_newsFeedList.clear();

    foreach(auto item, m_newsFeedAniList)
    {
        delete item;
        item = NULL;
    }
    m_newsFeedAniList.clear();

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
        NewsInfo::FeedData *feed = new NewsInfo::FeedData();
        feed->originLabel = new QLabel(this);
        feed->changedLabel = new QLabel(this);

        QString feedStr = "  ";
        feedStr += feedList[idx];
        feed->originLabel->setText(feedStr);
        feed->changedLabel->setText(feedStr);

        m_newsFeedList.push_back(feed);
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

    // news feed
    const int feedListSize = m_newsFeedList.size();
    for(int idx = 0; idx < feedListSize; idx++)
    {
        if(true == m_feedStyle.bTransparency)
        {
            m_newsFeedList[idx]->originLabel->setAttribute(Qt::WA_TranslucentBackground, true);
            m_newsFeedList[idx]->changedLabel->setAttribute(Qt::WA_TranslucentBackground, true);
        }
        else
        {
            QString feedStyleSheet = QString("QLabel {background-color: %1; color: %2;}")
                                        .arg(m_feedStyle.backgroundColor)
                                        .arg(m_feedStyle.fontColor);
            ELGO_VIEWER_LOG("Feed StyleSheet - %s", feedStyleSheet.toStdString().c_str());

            m_newsFeedList[idx]->originLabel->setStyleSheet(feedStyleSheet);
            m_newsFeedList[idx]->changedLabel->setStyleSheet(feedStyleSheet);
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
    ELGO_VIEWER_LOG("Widget pos{x: %f, y: %f}, size{w: %d, h: %d}",
                    widgetPos.x(), widgetPos.y(), widgetSize.width(), widgetSize.height());
    this->setGeometry(widgetRect);

    // title
    const QPoint titlePos(widgetSize.width() * 0.01, 0);
    const QSize titleSize(widgetSize.width() * 0.9, widgetSize.height() * 0.1);
    const QRect titleRect(titlePos, titleSize);
    ELGO_VIEWER_LOG("Title pos{x: %d, y: %d}, size{w: %d, h: %d}",
                    titlePos.x(), titlePos.y(), titleSize.width(), titleSize.height());

    QFont titleFont;
    const int titleFontSize = titleSize.height() * 0.8;
    titleFont.setBold(true);
    titleFont.setPointSize(titleFontSize);
    ELGO_VIEWER_LOG("Title Font Size : %d", titleFontSize);

    ui->newsTitle->setGeometry(titleRect);
    ui->newsTitle->setFont(titleFont);

    // news Feed
    const QSize feedSize = QSize(widgetSize.width() * 0.9, widgetSize.height() * 0.06);

    int feedPosX = widgetSize.width() * 0.01;
    int feedPosY = widgetSize.height() * 0.15;

    const int feedListSize = m_newsFeedList.size();
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

        QFont feedFont;
        const int feedFontSize = feedSize.height() * 0.7;
        feedFont.setBold(true);
        feedFont.setPointSize(feedFontSize);
        ELGO_VIEWER_LOG("Feed Font Size : %d", feedFontSize);

        m_newsFeedList[idx]->originLabel->setGeometry(originFeedRect);
        m_newsFeedList[idx]->changedLabel->setGeometry(changedFeedRect);

        m_newsFeedList[idx]->originLabel->setFont(feedFont);
        m_newsFeedList[idx]->changedLabel->setFont(feedFont);

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
    const int newsFeedListSize = m_newsFeedList.size();
    for(int idx = 0; idx < newsFeedListSize; idx++)
    {
        // check text overflow
        const QFontMetricsF fontMetrics(m_newsFeedList[idx]->originLabel->font());
        const QRectF fontRect = fontMetrics.boundingRect(m_newsFeedList[idx]->originLabel->text());
        if(fontRect.width() > m_newsFeedList[idx]->originLabel->width())
        {
            m_newsFeedList[idx]->bIsTextOverflow = true;
        }

        const QPoint originPos = m_newsFeedList[idx]->originLabel->geometry().topLeft();
        const QPoint movePos = m_newsFeedList[idx]->changedLabel->geometry().topLeft();
        const QSize feedSize = m_newsFeedList[idx]->originLabel->size();

        NewsInfo::FeedAnimation *feedAni = new NewsInfo::FeedAnimation();
        feedAni->originLabelAni = new QPropertyAnimation(m_newsFeedList[idx]->originLabel, "geometry");
        feedAni->originLabelAni->setDuration(LABEL_ANIMATION_DURATION);
        feedAni->originLabelAni->setStartValue(QRect(originPos, feedSize));
        feedAni->originLabelAni->setEndValue(QRect(originPos, QSize(feedSize.width(), 0)));

        feedAni->changedLabelAni = new QPropertyAnimation(m_newsFeedList[idx]->changedLabel, "geometry");
        feedAni->changedLabelAni->setDuration(LABEL_ANIMATION_DURATION);
        feedAni->changedLabelAni->setStartValue(QRect(movePos, QSize(feedSize.width(), 0)));
        feedAni->changedLabelAni->setEndValue(QRect(originPos, feedSize));

        feedAni->parallAniGroup = new QParallelAnimationGroup();
        feedAni->parallAniGroup->addAnimation(feedAni->originLabelAni);
        feedAni->parallAniGroup->addAnimation(feedAni->changedLabelAni);
        m_newsFeedAniList.push_back(feedAni);

        m_seqAniGroup->setLoopCount(1000);
        m_seqAniGroup->addAnimation(feedAni->parallAniGroup);

    }

    QPropertyAnimation *waitNextAni = new QPropertyAnimation(this);
    waitNextAni->setDuration(30000);
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
