
// Viewer
#include "FeedBoxWidget.h"
#include "ui_FeedBoxWidget.h"
#include "Logger/ViewerLogger.h"

#define DOWN_UP_ANIMATION_DURATION  1500 // unit : msec
#define OVERFLOW_ANIMATION_START 10000 // unit : msec
#define OVERFLOW_ANIMATION_END  0 // unit : msec
#define SCENE_RECT_CALC_VALUE    3

//========================================================
FeedBoxWidget::FeedBoxWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::FeedBoxWidget)
    , m_bIsTextOverflow(false)
//========================================================
{
    ui->setupUi(this);
    this->setAttribute(Qt::WA_TranslucentBackground, true);

    m_originLabelAni = new QPropertyAnimation(ui->originLabel, "geometry", this);
    m_changedLabelAni = new QPropertyAnimation(ui->changedLabel, "geometry", this);

    m_overflowStartAni = new QPropertyAnimation(ui->changedLabel, "geometry", this);
    m_overflowEndAni = new QPropertyAnimation(ui->changedLabel, "geometry", this);

    m_parallAniGroup = new QParallelAnimationGroup(this);
}

//========================================================
FeedBoxWidget::~FeedBoxWidget()
//========================================================
{
    delete ui;

    delete m_originLabelAni;
    m_originLabelAni = NULL;

    delete m_changedLabelAni;
    m_changedLabelAni = NULL;

    delete m_overflowStartAni;
    m_overflowStartAni = NULL;

    delete m_overflowEndAni;
    m_overflowEndAni = NULL;

    delete m_parallAniGroup;
    m_parallAniGroup = NULL;
}

//========================================================
void FeedBoxWidget::SetFeedBoxText(const QString& text)
//========================================================
{
    ui->originLabel->setText(text);
    ui->changedLabel->setText(text);
}

//========================================================
void FeedBoxWidget::SetTranslucentBackground(const bool value)
//========================================================
{
    ui->originLabel->setAttribute(Qt::WA_TranslucentBackground, value);
    ui->changedLabel->setAttribute(Qt::WA_TranslucentBackground, value);
}

//========================================================
void FeedBoxWidget::SetLabelStyleSheet(const QString& styleSheet)
//========================================================
{
    ui->originLabel->setStyleSheet(styleSheet);
    ui->changedLabel->setStyleSheet(styleSheet);
}

//========================================================
void FeedBoxWidget::SetFeedBoxGeometry(const QRect& originRect, const QRect& changedRect)
//========================================================
{
    // Widget
    const QPoint widgetPos(originRect.topLeft());
    const QSize widgetSize(originRect.width() * SCENE_RECT_CALC_VALUE, originRect.height());
    const QRect widgetRect(widgetPos, widgetSize);
    this->setGeometry(widgetRect);
    ELGO_VIEWER_LOG("FeedBox Widget - pos{x: %d, y: %d}, size{w: %d, h: %d}",
                    widgetPos.x(), widgetPos.y(), widgetSize.width(), widgetSize.height());

    // Label
    ui->originLabel->setGeometry(QRect(QPoint(0,0), widgetSize));
    ui->changedLabel->setGeometry(QRect(QPoint(0,widgetSize.height()), QSize(widgetSize.width(), 0)));
    ELGO_VIEWER_LOG("Origin FeedBox - pos{x: %d, y: %d}, size{w: %d, h: %d}",
                    originRect.x(), originRect.y(), originRect.width(), originRect.height());
    ELGO_VIEWER_LOG("Changed FeedBox - pos{x: %d, y: %d}, size{w: %d, h: %d}",
                    changedRect.x(), changedRect.y(), changedRect.width(), changedRect.height());
}

//========================================================
void FeedBoxWidget::SetLabelFont(const QFont& font)
//========================================================
{
    ui->originLabel->setFont(font);
    ui->changedLabel->setFont(font);
}

//========================================================
void FeedBoxWidget::CheckTextOverflowStatus()
//========================================================
{
    const QFontMetricsF fontMetrics(ui->originLabel->font());
    const QRectF fontRect = fontMetrics.boundingRect(ui->originLabel->text());

    if(fontRect.width() > ui->originLabel->width() / SCENE_RECT_CALC_VALUE)
    {
        m_bIsTextOverflow = true;
    }
}

//========================================================
bool FeedBoxWidget::GetTextOverflowStatus()
//========================================================
{
    return m_bIsTextOverflow;
}

//========================================================
void FeedBoxWidget::MakeFeedBoxAnimation()
//========================================================
{
    const QPoint originPos = ui->originLabel->pos();
    const QPoint movePos = ui->changedLabel->pos();
    const QSize feedSize = ui->originLabel->size();

    m_originLabelAni->setDuration(DOWN_UP_ANIMATION_DURATION);
    m_originLabelAni->setStartValue(QRect(originPos, feedSize));
    m_originLabelAni->setEndValue(QRect(originPos, QSize(feedSize.width(), 0)));

    m_changedLabelAni->setDuration(DOWN_UP_ANIMATION_DURATION);
    m_changedLabelAni->setStartValue(QRect(movePos, QSize(feedSize.width(), 0)));
    m_changedLabelAni->setEndValue(QRect(originPos, feedSize));

    m_parallAniGroup->addAnimation(m_originLabelAni);
    m_parallAniGroup->addAnimation(m_changedLabelAni);

    if(true == m_bIsTextOverflow)
    {
        const QFontMetricsF fontMetrics(ui->originLabel->font());
        const QRectF fontRect = fontMetrics.boundingRect(ui->originLabel->text());

        const QRect overflowRect(QPoint(this->x() - fontRect.width() * 1.05, originPos.y()),
                                 QSize(feedSize));

        m_overflowStartAni->setDuration(OVERFLOW_ANIMATION_START);
        m_overflowStartAni->setStartValue(QRect(originPos, feedSize));
        m_overflowStartAni->setEndValue(overflowRect);

        m_overflowEndAni->setDuration(OVERFLOW_ANIMATION_END);
        m_overflowEndAni->setStartValue(overflowRect);
        m_overflowEndAni->setEndValue(QRect(originPos, feedSize));
    }
}

//========================================================
QParallelAnimationGroup* FeedBoxWidget::GetParallelAnimationGroup()
//========================================================
{
    return m_parallAniGroup;
}

//========================================================
QPropertyAnimation* FeedBoxWidget::GetOverflowStartAnimation()
//========================================================
{
    return m_overflowStartAni;
}

//========================================================
QPropertyAnimation* FeedBoxWidget::GetOverflowEndAnimation()
//========================================================
{
    return m_overflowEndAni;
}
