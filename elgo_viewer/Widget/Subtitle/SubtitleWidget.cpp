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
    ui->subtitleLabel->setWordWrap(true);
    m_stateMachine = new QStateMachine;
}

//========================================================
SubtitleWidget::~SubtitleWidget()
//========================================================
{
    delete m_propertyAni;
    m_propertyAni = NULL;

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
    m_propertyAni = new QPropertyAnimation(ui->subtitleLabel, "geometry");
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
            verticalText += ui->subtitleLabel->text()[idx];
            verticalText += "\n";
        }
        ui->subtitleLabel->setText(verticalText);
    }

    // subtitle flow
    if(false == m_animationInfo.bIsFixed)
    {
        const QRect& originRect(ui->subtitleLabel->geometry());
        QPoint startPos;
        QPoint endPos;

        // subtitle action
        if(PlayJson::SubtitleAction::SCROLL == m_animationInfo.action)
        {
            GetScrollAnimationMovePos(originRect, startPos, endPos);

            // sec -> msec
            if( (PlayJson::AniFlowDirection::LEFT_TO_RIGHT == m_animationInfo.direction) ||
                (PlayJson::AniFlowDirection::RIGHT_TO_LEFT == m_animationInfo.direction) )
            {
                if(PlayJson::Orientation::VERTICAL == m_animationInfo.orientation)
                {
                    m_propertyAni->setDuration((m_animationInfo.speed / 3) * 1000);
                }
                else
                {
                    m_propertyAni->setDuration(m_animationInfo.speed * 1000);
                }
            }
            else
            {
                if(PlayJson::Orientation::VERTICAL == m_animationInfo.orientation)
                {
                    m_propertyAni->setDuration(m_animationInfo.speed * 1000);
                }
                else
                {
                    m_propertyAni->setDuration((m_animationInfo.speed / 3) * 1000);
                }
            }

            m_propertyAni->setStartValue(QRect(startPos, originRect.size()));
            m_propertyAni->setEndValue(QRect(endPos, originRect.size()));
            m_propertyAni->setLoopCount(1000);
        }
        else
        {
            // Loop Action
            QState *startState = new QState(m_stateMachine);
            startState->assignProperty(ui->subtitleLabel, "geometry", originRect);

            QState *endState = new QState(m_stateMachine);
            endState->assignProperty(ui->subtitleLabel, "geometry", endPos);

            m_stateMachine->setInitialState(startState);
            m_stateMachine->setInitialState(endState);

            QSignalTransition *startSignalTrans;
            startSignalTrans = startState->addTransition(m_stateMachine, SIGNAL(started()), endState);
            startSignalTrans->addAnimation(new QPropertyAnimation(ui->subtitleLabel, "geometry"));

            QSignalTransition *endSingalTrans;
            endSingalTrans = endState->addTransition(ui->subtitleLabel, SIGNAL(started()), startState);
            endSingalTrans->addAnimation(new QPropertyAnimation(ui->subtitleLabel, "geometry"));
        }
    }
    else
    {
        ELGO_VIEWER_LOG("This Subtitle is fixed");
    }
}

//========================================================
void SubtitleWidget::GetScrollAnimationMovePos(const QRect& originRect, QPoint& startPos, QPoint& endPos)
//========================================================
{
    if(PlayJson::AniFlowDirection::LEFT_TO_RIGHT == m_animationInfo.direction)
    {
        startPos.setX(originRect.left() - originRect.width());
        startPos.setY(originRect.top());

        endPos.setX(originRect.right());
        endPos.setY(originRect.top());
    }
    else if(PlayJson::AniFlowDirection::RIGHT_TO_LEFT == m_animationInfo.direction)
    {
        startPos.setX(originRect.right());
        startPos.setY(originRect.top());

        endPos.setX(originRect.left() - originRect.width());
        endPos.setY(originRect.top());
    }
    else if(PlayJson::AniFlowDirection::BOTTOM_TO_TOP == m_animationInfo.direction)
    {
        startPos.setX(originRect.left());
        startPos.setY(originRect.bottom());

        endPos.setX(originRect.left());
        endPos.setY(originRect.top() - originRect.height());
    }
    else
    {
        // TOP_TO_BOTTOM
        startPos.setX(originRect.left());
        startPos.setY(originRect.top() - originRect.height());

        endPos.setX(originRect.left());
        endPos.setY(originRect.bottom());
    }
}

//========================================================
void SubtitleWidget::GetLoopAnimationMovePos(const QRect& originRect, QPoint& movePos)
//========================================================
{
    if( (PlayJson::AniFlowDirection::LEFT_TO_RIGHT == m_animationInfo.direction) ||
        (PlayJson::AniFlowDirection::RIGHT_TO_LEFT == m_animationInfo.direction) )
    {
        movePos.setX(originRect.right() - originRect.width());
        movePos.setY(originRect.top());
    }
    else if( (PlayJson::AniFlowDirection::BOTTOM_TO_TOP == m_animationInfo.direction) ||
             (PlayJson::AniFlowDirection::TOP_TO_BOTTOM == m_animationInfo.direction) )
    {
        movePos.setX(originRect.left());
        movePos.setY(originRect.top() - originRect.height());
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
            m_propertyAni->start();
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
            m_propertyAni->stop();
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
