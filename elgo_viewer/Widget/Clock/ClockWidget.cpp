
// Viewer
#include "ClockWidget.h"
#include "ui_ClockWidget.h"
#include "Logger/ViewerLogger.h"

#include <QDebug>

//========================================================
ClockWidget::ClockWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ClockWidget)
    , m_clockKinds(Clock::Kinds::HOUR_12)
    , m_bIsTimerStarted(false)
//========================================================
{
    ui->setupUi(this);

    // init
    m_updateTimer = new QTimer(this);
    m_dateTime = QDateTime::currentDateTime();

    QFont font;
    font.setBold(true);
    font.setPointSize(100);
    ui->amPmLabel->setFont(font);
    ui->timeLabel->setFont(font);

    ui->amPmLabel->setAlignment(Qt::AlignCenter);
    ui->timeLabel->setAlignment(Qt::AlignCenter);

    // Get Geometry
    m_amPmLabelRect = ui->amPmLabel->geometry();
    m_timeLabelRect = ui->timeLabel->geometry();

    // connect
    connect(m_updateTimer, SIGNAL(timeout()), this, SLOT(UpdateCurrentTime()));
}

//========================================================
ClockWidget::~ClockWidget()
//========================================================
{
    delete ui;

    m_updateTimer->stop();
    delete m_updateTimer;
    m_updateTimer = NULL;
}

//========================================================
void ClockWidget::SetStyleSheet(const StyleSheet::StyleInfo& style)
//========================================================
{
    m_styleInfo = style;

    QString widgetStyleSheet = QString("QWidget {background-color: %1;}")
                                .arg(m_styleInfo.backgroundColor);
    QString labelStyleSheet = QString("QLabel {color: %1;}")
                                .arg(m_styleInfo.fontColor);

    if(true == style.bTransparency)
    {
        this->setAttribute(Qt::WA_TranslucentBackground, true);
    }
    else
    {
        this->setStyleSheet(widgetStyleSheet);
    }

    ui->amPmLabel->setStyleSheet(labelStyleSheet);
    ui->timeLabel->setStyleSheet(labelStyleSheet);
}

//========================================================
void ClockWidget::MakeClockWidget(Clock::Kinds kind)
//========================================================
{
    m_dateTime = QDateTime::currentDateTime();
    m_clockKinds = kind;

    // time
    const QTime time = m_dateTime.time();
    const int hour = time.hour();
    const int min = time.minute();

    if(Clock::Kinds::HOUR_12 == kind)
    {
        int newHour = hour;
        if(13 <= hour)
        {
            m_amPmStr = "오후";
            newHour -= 12;
        }
        else
        {
            m_amPmStr = "오전";
        }

        char buffer[64] = {'\0',};
        sprintf(buffer, "%02d:%02d", newHour, min);
        m_timeStr = buffer;

        ui->amPmLabel->setText(m_amPmStr);
        ui->timeLabel->setText(m_timeStr);
    }
    else if(Clock::Kinds::HOUR_24 == kind)
    {
        const QRect newLabelRect = QRect(m_amPmLabelRect.topLeft(),
                                         QSize(m_amPmLabelRect.width() + m_timeLabelRect.width(),
                                         m_amPmLabelRect.height() + m_timeLabelRect.height()));


        char buffer[64] = {'\0',};
        sprintf(buffer, "%02d:%02d", hour, min);
        m_timeStr = buffer;

        ui->amPmLabel->clear();
        ui->timeLabel->setGeometry(newLabelRect);
        ui->timeLabel->setText(m_timeStr);
    }
    else
    {
        ELGO_VIEWER_LOG("Error - Unkown Clock Kind : %d", kind);
    }

    if(false == m_bIsTimerStarted)
    {
        m_bIsTimerStarted = true;
        m_updateTimer->start(1000);
    }
}

//========================================================
void ClockWidget::UpdateCurrentTime()
//========================================================
{
    MakeClockWidget(m_clockKinds);
}
