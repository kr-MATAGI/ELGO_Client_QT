
// Viewer
#include "NewsFeedWidget.h"
#include "ui_NewsFeedWidget.h"
#include "Logger/ViewerLogger.h"

#define LABEL_ANIMATION_DURATION  1800 //unit : msec

//========================================================
NewsFeedWidget::NewsFeedWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::NewsFeedWidget)
//========================================================
{
    ui->setupUi(this);

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
void NewsFeedWidget::SetNewsFeedList(const NewsInfo::Topic topic, const QList<QString>& feeds)
//========================================================
{
    // for test
    QString styleSheetStr = QString("QLabel {border: %1px solid white;}").arg(1);

    foreach(auto item, feeds)
    {
        NewsInfo::FeedData *feed = new NewsInfo::FeedData();
        feed->originLabel = new QLabel(this);
        feed->changedLabel = new QLabel(this);

        feed->originLabel->setText(item);
        feed->changedLabel->setText(item);

        feed->originLabel->setStyleSheet(styleSheetStr);
        feed->changedLabel->setStyleSheet(styleSheetStr);

        m_newsFeedList.push_back(feed);
    }

    m_topic = topic;
}

//========================================================
void NewsFeedWidget::SetBackgroundStyleSheet(const StyleSheet::StyleInfo &style)
//========================================================
{

}

//========================================================
void NewsFeedWidget::SetFeedLabelStyleSheet(const StyleSheet::StyleInfo &style)
//========================================================
{

}

//========================================================
void NewsFeedWidget::MakeNewsFeedWidget()
//========================================================
{
    QString title = QString("오늘의 %1 소식").arg(NewsInfo::topicEnumToStr[m_topic]);

    // title
    ui->titleLabel->setText(title);

    // add newsFeed
    int ax = 20;
    int ay = 90;
    int width = this->width() * 0.8;
    int height = this->height() * 0.1;

    foreach(auto item, m_newsFeedList)
    {
        item->originLabel->setGeometry(ax, ay, width, height);
        ay += height;
        item->changedLabel->setGeometry(ax, ay, width, height);

        // check text overflow
        QFontMetricsF fontMetrics(item->originLabel->font());
        QRectF fontRect = fontMetrics.boundingRect(item->originLabel->text());
        if(width < fontRect.width())
        {
            item->bIsTextOverflow = true;
        }

        ui->feedVLayout->addWidget(item->originLabel);
        ui->feedVLayout->addWidget(item->changedLabel);
    }

    // animation
    foreach(auto item, m_newsFeedList)
    {
        QPoint curPos = item->originLabel->geometry().topLeft();
        QPoint movePos = QPoint(curPos.x(), curPos.y() + height);

        NewsInfo::FeedAnimation *feedAni = new NewsInfo::FeedAnimation();
        feedAni->originLabelAni = new QPropertyAnimation(item->originLabel, "geometry");
        feedAni->originLabelAni->setDuration(LABEL_ANIMATION_DURATION);
        feedAni->originLabelAni->setStartValue(QRect(curPos, QSize(width, height)));
        feedAni->originLabelAni->setEndValue(QRect(curPos, QSize(width, 0)));

        feedAni->changedLabelAni = new QPropertyAnimation(item->changedLabel, "geometry");
        feedAni->changedLabelAni->setDuration(LABEL_ANIMATION_DURATION);
        feedAni->changedLabelAni->setStartValue(QRect(movePos, QSize(width, 0)));
        feedAni->changedLabelAni->setEndValue(QRect(curPos, QSize(width, height)));

        // animation group
        feedAni->parallAniGroup = new QParallelAnimationGroup();
        feedAni->parallAniGroup->addAnimation(feedAni->originLabelAni);
        feedAni->parallAniGroup->addAnimation(feedAni->changedLabelAni);
        m_newsFeedAniList.push_back(feedAni);
        m_seqAniGroup->addAnimation(feedAni->parallAniGroup);
    }

    m_seqAniGroup->start();

}
