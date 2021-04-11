// Viewer
#include "ClockWidget.h"
#include "ui_ClockWidget.h"
#include "Logger/ViewerLogger.h"

//========================================================
ClockWidget::ClockWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ClockWidget)
    , m_hourType(PlayJson::HourType::NONE_HOUR_TYPE)
    , m_bIsTimerStarted(false)
//========================================================
{
    ui->setupUi(this);

    // init
    m_updateTimer = new QTimer(this);
    m_dateTime = QDateTime::currentDateTime();

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
void ClockWidget::SetStyleSheet(const StyleSheet::StyleInfo& styleInfo)
//========================================================
{
    m_styleInfo = styleInfo;

    QString widgetStyleSheet = QString("QWidget {background-color: %1;}")
                                .arg(m_styleInfo.backgroundColor);
    QString labelStyleSheet = QString("QLabel {color: %1;}")
                                .arg(m_styleInfo.fontColor);
    ELGO_VIEWER_LOG("widget StyleSheet : %s", widgetStyleSheet.toStdString().c_str());
    ELGO_VIEWER_LOG("label StyleSheet : %s", labelStyleSheet.toStdString().c_str());
    ELGO_VIEWER_LOG("background transparency : %d", styleInfo.bTransparency);

    if(true == styleInfo.bTransparency)
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
void ClockWidget::SetPosSizeInfo(const StyleSheet::PosSizeInfo& posSizeInfo)
//========================================================
{
    m_posSizeInfo = posSizeInfo;
    const QPointF& widgetPos = m_posSizeInfo.pos;
    const QSize& widgetSize = m_posSizeInfo.size;
    const QRect widgetRect(widgetPos.toPoint(), widgetSize);
    ELGO_VIEWER_LOG("Widget pos{x: %f, y: %f} size{w: %d, h: %d}",
                    widgetPos.x(), widgetPos.y(), widgetSize.width(), widgetSize.height());

    this->setGeometry(widgetRect);
    if(PlayJson::HourType::HOUR_12== m_hourType)
    {
        // AM/PM label
        const QPoint amPmLabelPos(0,0);
        const QSize amPmLabelSize(widgetSize.width() / 3, widgetSize.height());
        const QRect amPmLabelRect(amPmLabelPos, amPmLabelSize);
        ELGO_VIEWER_LOG("amPmStrLabel Pos{x: %f, y: %f}, size{w: %d, h: %d}",
                        amPmLabelPos.x(), amPmLabelPos.y(), amPmLabelSize.width(), amPmLabelSize.height());

        QFont amPmFont;
        const int amPmFontSize = CalcLabelFontSize(amPmLabelSize.width());
        amPmFont.setBold(true);
        amPmFont.setPointSize(amPmFontSize);
        ELGO_VIEWER_LOG("amPmFont Size : %d", amPmFontSize);

        ui->amPmLabel->setGeometry(amPmLabelRect);
        ui->amPmLabel->setFont(amPmFont);

        // Time Label
        const QPoint timeLabelPos(ui->amPmLabel->rect().right(),0);
        const QSize timeLabelSize(widgetSize.width() / 3 * 2, widgetSize.height());
        const QRect timeLabelRect(timeLabelPos, timeLabelSize);
        ELGO_VIEWER_LOG("timeStrLabel Pos{x: %d, y: %d}, Size{w: %d, h: %d}",
                        timeLabelPos.x(), timeLabelPos.y(), timeLabelSize.width(), timeLabelSize.height());

        QFont timeStrFont;
        const int timeStrFontSize = CalcLabelFontSize(timeLabelSize.width());
        timeStrFont.setBold(true);
        timeStrFont.setPointSize(timeStrFontSize);
        ELGO_VIEWER_LOG("timeStrFont Size : %d", timeStrFontSize);

        ui->timeLabel->setGeometry(timeLabelRect);
        ui->timeLabel->setFont(timeStrFont);
    }
    else
    {
        ui->amPmLabel->clear();
        ui->amPmLabel->setGeometry(0,0,0,0);

        const QPoint timeLabelPos(0, 0);
        const QSize timeLabelSize(widgetSize.width(), widgetSize.height());
        const QRect timeLabelRect(timeLabelPos, timeLabelSize);
        ELGO_VIEWER_LOG("timeStrLabel Pos{x: %f, y: %f}, size{w: %d, h: %d}",
                        timeLabelPos.x(), timeLabelPos.y(), timeLabelSize.width(), timeLabelSize.height());

        QFont timeStrFont;
        const int fontSize = CalcLabelFontSize(timeLabelSize.width());
        timeStrFont.setBold(true);
        timeStrFont.setPointSize(fontSize);
        ELGO_VIEWER_LOG("Font Size : %d", fontSize);

        ui->timeLabel->setGeometry(timeLabelRect);
        ui->timeLabel->setFont(timeStrFont);
    }
}

//========================================================
void ClockWidget::MakeClockTimeString(PlayJson::HourType type)
//========================================================
{
    m_dateTime = QDateTime::currentDateTime();
    m_hourType = type;

    // time
    const QTime time = m_dateTime.time();
    const int hour = time.hour();
    const int min = time.minute();

    if(PlayJson::HourType::HOUR_12 == type)
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
    else if(PlayJson::HourType::HOUR_24 == type)
    {
        char buffer[64] = {'\0',};
        sprintf(buffer, "%02d:%02d", hour, min);
        m_timeStr = buffer;

        ui->amPmLabel->clear();
        ui->timeLabel->setText(m_timeStr);
    }
    else
    {
        ELGO_VIEWER_LOG("Error - Unkown Clock type : %d", type);
    }
}

//========================================================
void ClockWidget::StartClock()
//========================================================
{
    m_updateTimer->start(1000);
    m_bIsTimerStarted = true;
}

//========================================================
void ClockWidget::StopClock()
//========================================================
{
    m_updateTimer->stop();
    m_bIsTimerStarted = false;
}

//========================================================
bool ClockWidget::IsStartedClock()
//========================================================
{
    return m_bIsTimerStarted;
}

//========================================================
void ClockWidget::UpdateCurrentTime()
//========================================================
{
    MakeClockTimeString(m_hourType);
}

//========================================================
int ClockWidget::CalcLabelFontSize(const int labelWidth)
//========================================================
{
    int retValue = DEFAULT_FONT_SIZE;
    retValue += (labelWidth / 5);

    return retValue;
}
