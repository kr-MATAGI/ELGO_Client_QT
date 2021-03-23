
// QT
#include <QDateTime>

// Viewer
#include "DateWidget.h"
#include "ui_DateWidget.h"
#include "Logger/ViewerLogger.h"

//========================================================
DateWidget::DateWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::DateWidget)
    , m_bIsTimerStarted(false)
//========================================================
{
    ui->setupUi(this);

    // init
    m_updateTimer = new QTimer(this);

    QFont font;
    font.setBold(true);
    font.setPointSize(30);
    ui->dateLabel->setFont(font);
    ui->dateLabel->setAlignment(Qt::AlignCenter);

    // Get geometry
    m_dateLabelRect = ui->dateLabel->geometry();

    // connect
    connect(m_updateTimer, SIGNAL(timeout()), this, SLOT(UpdateCurrentDate()));
}

//========================================================
DateWidget::~DateWidget()
//========================================================
{
    delete ui;

    m_updateTimer->stop();
    delete m_updateTimer;
    m_updateTimer = NULL;
}

//========================================================
void DateWidget::SetStyleSheet(const StyleSheet::StyleInfo& style)
//========================================================
{
    m_styleInfo = style;

    QString widgetStyleSheet = QString("QWidget {background-color: %1;}")
                                .arg(style.backgroundColor);
    QString labelStyleSheet = QString("QLabel {color: %1'}")
                                .arg(style.fontColor);

    if(true == style.bTransparency)
    {
        this->setAttribute(Qt::WA_TranslucentBackground, true);
    }
    else
    {
        this->setStyleSheet(widgetStyleSheet);
    }

    ui->dateLabel->setStyleSheet(labelStyleSheet);
}

//========================================================
void DateWidget::ConvertToStringFromDayOfWeek(const int src, QString& dest)
//========================================================
{
    /**
     * @note    dayofWeek - 1: Monday... 7: Sunday
     */

    if(1 == src)
    {
        dest = "월요일";
    }
    else if(2 == src)
    {
        dest = "화요일";
    }
    else if(3 == src)
    {
        dest = "수요일";
    }
    else if(4 == src)
    {
        dest = "목요일";
    }
    else if(5 == src)
    {
        dest = "금요일";
    }
    else if(6 == src)
    {
        dest = "토요일";
    }
    else if(7 == src)
    {
        dest = "일요일";
    }
    else
    {
        ELGO_VIEWER_LOG("Error - Unkwon day of week");
    }
}

//========================================================
void DateWidget::MakeDateWidget()
//========================================================
{
    m_dateTime = QDateTime::currentDateTime();

    const QDate date = m_dateTime.date();
    const int year = date.year(); // not using
    const int month = date.month();
    const int day = date.day();
    const int dayOfWeek = date.dayOfWeek();

    QString dayOfWeekStr;
    ConvertToStringFromDayOfWeek(dayOfWeek, dayOfWeekStr);
    QString dateStr = QString("%1월 %2일 %3")
                        .arg(month)
                        .arg(day)
                        .arg(dayOfWeekStr);
    ui->dateLabel->setText(dateStr);

    if(false == m_bIsTimerStarted)
    {
        m_bIsTimerStarted = true;
        m_updateTimer->start(1000);
    }
}

//========================================================
void DateWidget::UpdateCurrentDate()
//========================================================
{
    MakeDateWidget();
}
