// QT
#include <QSignalTransition>
#include <QPainter>

// Viewer
#include "SubtitleWidget.h"
#include "ui_SubtitleWidget.h"
#include "Logger/ViewerLogger.h"

//========================================================
SubtitleWidget::SubtitleWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::SubtitleWidget)
    , m_bIsStartedAni(false)
//========================================================
{
    ui->setupUi(this);
    this->setObjectName("Subtitle Widget");

    // init
    m_stateMachine = new QStateMachine;
}

//========================================================
SubtitleWidget::~SubtitleWidget()
//========================================================
{
    StopAnimation();

    delete m_startyAni;
    m_startyAni = NULL;

    delete m_endAni;
    m_endAni = NULL;

    delete m_stateMachine;
    m_stateMachine = NULL;
}

//========================================================
void SubtitleWidget::MakeSubTitleWidget(const PlayJson::SubtitleData& subtitleData)
//========================================================
{
    ui->subtitleLabel->setText(subtitleData.text);
    ELGO_VIEWER_LOG("Subtitle : %s", subtitleData.text.toStdString().c_str());

    // Set Style Sheet
    StyleSheet::StyleInfo styleInfo;
//    styleInfo.bTransparency = subtitleData. Not Existed
    styleInfo.backgroundColor = subtitleData.backgroundColor;
    styleInfo.fontColor = subtitleData.fontColor;
    styleInfo.fontSize = subtitleData.fontSize;

    SetStyleSheet(styleInfo);

    // Set Pos Size
    StyleSheet::PosSizeInfo posSizeInfo;
    posSizeInfo.pos.setX(subtitleData.left);
    posSizeInfo.pos.setY(subtitleData.top);
    posSizeInfo.size.setWidth(subtitleData.width);
    posSizeInfo.size.setHeight(subtitleData.height);

    SetPosSizeInfo(posSizeInfo);

    // Set Animation
    SubtitleInfo::Animation animationInfo;
    animationInfo.bIsFixed = subtitleData.bIsFixed;
    animationInfo.direction = subtitleData.direction;
    animationInfo.orientation = subtitleData.orientation;
    animationInfo.action = subtitleData.action;
    animationInfo.speed = subtitleData.speed;

    SetAnimationInfo(animationInfo);
}

//========================================================
void SubtitleWidget::SetStyleSheet(const StyleSheet::StyleInfo& styleInfo)
//========================================================
{
    m_styleInfo = styleInfo;

    QString widgetStyleSheet = QString("QWidget {background-color: %1;}")
                                .arg(m_styleInfo.backgroundColor);
    QString labelStyleSheet = QString("QLabel {color: %1;}")
                                .arg(m_styleInfo.fontColor);
    QFont labelFont;
    labelFont.setPixelSize(m_styleInfo.fontSize);

    ELGO_VIEWER_LOG("widget StyleSheet : %s", widgetStyleSheet.toStdString().c_str());
    ELGO_VIEWER_LOG("Label StyleSheet : %s", labelStyleSheet.toStdString().c_str());
    ELGO_VIEWER_LOG("Label FontSize : %d", m_styleInfo.fontSize);

    this->setStyleSheet(widgetStyleSheet);
    ui->subtitleLabel->setStyleSheet(labelStyleSheet);
    ui->subtitleLabel->setFont(labelFont);
}

//========================================================
void SubtitleWidget::SetPosSizeInfo(const StyleSheet::PosSizeInfo &posSizeInfo)
//========================================================
{
    m_posSizeInfo = posSizeInfo;

    // widget
    const QPointF& widgetPos = m_posSizeInfo.pos;
    const QSize& widgetSize = m_posSizeInfo.size;
    const QRect widgetRect(widgetPos.toPoint(), widgetSize);
    ELGO_VIEWER_LOG("Widget pos{x: %f, y: %f} size{w: %d, h: %d}",
                    widgetPos.x(), widgetPos.y(), widgetSize.width(), widgetSize.height());

    this->setGeometry(widgetRect);

    // Subtitle Label
    const QRect labelRect(QPoint(0,0), widgetRect.size());
    ELGO_VIEWER_LOG("Subtitle Label pos{x: %f, y: %f} size{w: %d, h: %d}",
                    widgetPos.x(), widgetPos.y(), widgetSize.width(), widgetSize.height());

    ui->subtitleLabel->setGeometry(labelRect);
}

//========================================================
void SubtitleWidget::SetAnimationInfo(const SubtitleInfo::Animation& animationInfo)
//========================================================
{
    m_animationInfo = animationInfo;
    m_startyAni = new QPropertyAnimation(ui->subtitleLabel, "geometry");
    m_endAni = new QPropertyAnimation(ui->subtitleLabel, "geometry");
    ELGO_VIEWER_LOG("isFixed: %d, action: %d, direction: %d, orientation: %d, speed: %d",
                    m_animationInfo.bIsFixed, m_animationInfo.action,
                    m_animationInfo.direction, m_animationInfo.orientation,
                    m_animationInfo.speed);

    // vertical text
    if(PlayJson::Orientation::VERTICAL == m_animationInfo.orientation)
    {
        QString verticalText;
        for(int idx = 0; idx < ui->subtitleLabel->text().length(); idx++ )
        {
            QChar ch = ui->subtitleLabel->text().at(idx);
            verticalText += ch;
            if(idx != (ui->subtitleLabel->text().length() - 1))
            {
                verticalText += "\n";
            }
        }
        ui->subtitleLabel->setText(verticalText);
        ui->subtitleLabel->setAlignment(Qt::AlignHCenter);
        ELGO_VIEWER_LOG("Vertical Text : %s", verticalText.toStdString().c_str());
    }

    // subtitle flow
    if(false == m_animationInfo.bIsFixed)
    {
        const QRect& originRect(ui->subtitleLabel->geometry());

        // subtitle action
        if(PlayJson::SubtitleAction::SCROLL == m_animationInfo.action)
        {
            QPoint startPos;
            QPoint endPos;

            GetScrollAnimationMovePos(originRect, startPos, endPos);

            // sec -> msec
            int aniSpeed = 0;
            if( (PlayJson::AniFlowDirection::LEFT_TO_RIGHT == m_animationInfo.direction) ||
                (PlayJson::AniFlowDirection::RIGHT_TO_LEFT == m_animationInfo.direction) )
            {
                if(PlayJson::Orientation::VERTICAL == m_animationInfo.orientation)
                {
                    aniSpeed = (m_animationInfo.speed / 3) * 1000;
                    m_startyAni->setDuration(aniSpeed);
                }
                else
                {
                    aniSpeed = m_animationInfo.speed * 1000;
                    m_startyAni->setDuration(aniSpeed);
                }
            }
            else
            {
                if(PlayJson::Orientation::VERTICAL == m_animationInfo.orientation)
                {
                    aniSpeed = m_animationInfo.speed * 1000;
                    m_startyAni->setDuration(m_animationInfo.speed * 1000);
                }
                else
                {
                    aniSpeed = (m_animationInfo.speed / 3) * 1000;
                    ui->subtitleLabel->setAlignment(Qt::AlignHCenter);
                    m_startyAni->setDuration(aniSpeed);
                }
            }
            ELGO_VIEWER_LOG("Animation Speed : %d msec", aniSpeed);

            m_startyAni->setStartValue(QRect(startPos, originRect.size()));
            m_startyAni->setEndValue(QRect(endPos, originRect.size()));
            m_startyAni->setLoopCount(1000);
        }
        else if(PlayJson::SubtitleAction::LOOP == m_animationInfo.action)
        {
            // Loop Action
            const QFontMetricsF fontMetrics(ui->subtitleLabel->font());

            QRect resizefontRect = fontMetrics.boundingRect(ui->subtitleLabel->text()).toRect();
            if(PlayJson::Orientation::HORIZONTAL == m_animationInfo.orientation)
            {
                ui->subtitleLabel->setAlignment(Qt::AlignLeft);
            }
            else if(PlayJson::Orientation::VERTICAL == m_animationInfo.orientation)
            {
                ui->subtitleLabel->setAlignment(Qt::AlignHCenter);
                const int vertiWidth = this->width() * 0.4;
                const int vertiHeight = ui->subtitleLabel->text().length() * ui->subtitleLabel->font().pixelSize();
                resizefontRect.setSize(QSize(vertiWidth, vertiHeight * 0.8));
            }

            const int resizePosY = (this->geometry().height() / 2) - (resizefontRect.size().height() / 2);
            const QRect labelResizeRect(QPoint(0, resizePosY), resizefontRect.size());
            ui->subtitleLabel->setGeometry(labelResizeRect);
            ELGO_VIEWER_LOG("fontRect - {w: %d, h: %d}", resizefontRect.width(), resizefontRect.height());
            ELGO_VIEWER_LOG("lablResizeRect - {x: %d, y: %d}, {w: %d, h: %d}",
                            labelResizeRect.x(), labelResizeRect.y(),
                            labelResizeRect.width(), labelResizeRect.height());

            QRect startRect;
            QRect endRect;
            GetLoopAnimationMovePos(this->geometry(), labelResizeRect, startRect, endRect);
            ELGO_VIEWER_LOG("startRect - {x: %d, y: %d}, {w: %d, h: %d}",
                            startRect.x(), startRect.y(),
                            startRect.width(), startRect.height());
            ELGO_VIEWER_LOG("endRect - {x: %d, y: %d}, {w: %d, h: %d}",
                            endRect.x(), endRect.y(),
                            endRect.width(), endRect.height());

            QState *startState = new QState(m_stateMachine);
            QState *endState = new QState(m_stateMachine);

            const int aniSpeed = (m_animationInfo.speed * 1000) / 3;
            ELGO_VIEWER_LOG("Animation Speed : %d msec", aniSpeed);

            m_startyAni->setDuration(aniSpeed);
            m_endAni->setDuration(aniSpeed);

            startState->assignProperty(ui->subtitleLabel, "geometry", startRect);
            endState->assignProperty(ui->subtitleLabel, "geometry", endRect);
            m_stateMachine->setInitialState(startState);

            QSignalTransition *startSignalTrans;
            startSignalTrans = startState->addTransition(m_stateMachine, SIGNAL(started()), endState);
            startSignalTrans->addAnimation(m_startyAni);

            QSignalTransition *loopSignalTrans;
            loopSignalTrans = startState->addTransition(m_endAni, SIGNAL(finished()), endState);
            loopSignalTrans->addAnimation(m_startyAni);

            QSignalTransition *endSingalTrans;
            endSingalTrans = endState->addTransition(m_startyAni, SIGNAL(finished()), startState);
            endSingalTrans->addAnimation(m_endAni);
        }
        else
        {
            ELGO_VIEWER_LOG("None Animation Action !");
        }
    }
    else
    {
        ELGO_VIEWER_LOG("This Subtitle is fixed");
        ui->subtitleLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    }
}

//========================================================
void SubtitleWidget::GetScrollAnimationMovePos(const QRect& originRect, QPoint& startPos, QPoint& endPos)
//========================================================
{
    if(PlayJson::AniFlowDirection::LEFT_TO_RIGHT == m_animationInfo.direction)
    {
        startPos.setX(originRect.left() - (originRect.width() / 2));
        startPos.setY(originRect.top());

        endPos.setX(originRect.right());
        endPos.setY(originRect.top());
    }
    else if(PlayJson::AniFlowDirection::RIGHT_TO_LEFT == m_animationInfo.direction)
    {
        startPos.setX(originRect.right());
        startPos.setY(originRect.top());

        endPos.setX(originRect.left() - (originRect.width() / 2));
        endPos.setY(originRect.top());
    }
    else if(PlayJson::AniFlowDirection::BOTTOM_TO_TOP == m_animationInfo.direction)
    {
        startPos.setX(originRect.left());
        startPos.setY(originRect.bottom());

        endPos.setX(originRect.left());
        endPos.setY(originRect.top() - (originRect.height() / 2));
    }
    else
    {
        // TOP_TO_BOTTOM
        startPos.setX(originRect.left());
        startPos.setY(originRect.top() - (originRect.height() / 2));

        endPos.setX(originRect.left());
        endPos.setY(originRect.bottom());
    }
}

//========================================================
void SubtitleWidget::GetLoopAnimationMovePos(const QRect& widgetRect, const QRect& labelRect,
                                             QRect& startRect, QRect& endRect)
//========================================================
{
    const int widgetVMid = labelRect.top();
    const int widgetHMid = (widgetRect.width() / 2) - (labelRect.size().width() / 2);
    if(PlayJson::AniFlowDirection::LEFT_TO_RIGHT == m_animationInfo.direction)
    {
        startRect.setTopLeft(QPoint(0, widgetVMid));
        startRect.setSize(labelRect.size());

        endRect.setTopLeft(QPoint(widgetRect.width() - labelRect.width(), widgetVMid));
        endRect.setSize(labelRect.size());
    }
    else if(PlayJson::AniFlowDirection::RIGHT_TO_LEFT == m_animationInfo.direction)
    {
        startRect.setTopLeft(QPoint(widgetRect.width() - labelRect.width(), widgetVMid));
        startRect.setSize(labelRect.size());

        endRect.setTopLeft(QPoint(0, widgetVMid));
        endRect.setSize(labelRect.size());
    }
    else if(PlayJson::AniFlowDirection::TOP_TO_BOTTOM == m_animationInfo.direction)
    {
        startRect.setTopLeft(QPoint(widgetHMid, 0));
        startRect.setSize(labelRect.size());

        endRect.setTopLeft(QPoint(widgetHMid, widgetRect.height() - labelRect.height()));
        endRect.setSize(labelRect.size());
    }
    else if(PlayJson::AniFlowDirection::BOTTOM_TO_TOP == m_animationInfo.direction)
    {
        startRect.setTopLeft(QPoint(widgetHMid, widgetRect.height() - labelRect.height()));
        startRect.setSize(labelRect.size());

        endRect.setTopLeft(QPoint(widgetHMid, 0));
        endRect.setSize(labelRect.size());
    }
}

//========================================================
void SubtitleWidget::StartAnimation()
//========================================================
{
    if(false == m_animationInfo.bIsFixed)
    {
        if(PlayJson::SubtitleAction::SCROLL == m_animationInfo.action)
        {
            m_startyAni->start();
        }
        else
        {
            m_stateMachine->start();
        }
    }

    m_bIsStartedAni = true;
}

//========================================================
void SubtitleWidget::StopAnimation()
//========================================================
{
    if(false == m_animationInfo.bIsFixed)
    {
        if(PlayJson::SubtitleAction::SCROLL == m_animationInfo.action)
        {
            m_startyAni->stop();
        }
        else
        {
            m_stateMachine->stop();
        }
    }

    m_bIsStartedAni = false;
}

//========================================================
bool SubtitleWidget::IsStartedAnimation()
//========================================================
{
    return m_bIsStartedAni;
}
