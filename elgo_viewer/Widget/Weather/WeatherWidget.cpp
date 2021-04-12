// QT
#include <QUrl>
#include <QDateTime>

// Viewer
#include "WeatherWidget.h"
#include "ui_WeatherWidget.h"

//========================================================
WeatherWidget::WeatherWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::WeatherWidget)
    , m_bIsTimerStarted(false)
//========================================================
{
    ui->setupUi(this);

    m_weatherScene = new QGraphicsScene(this);
    ui->weatherIconView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->weatherIconView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->weatherIconView->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    ui->weatherIconView->setRenderHint(QPainter::Antialiasing);
    ui->weatherIconView->setStyleSheet("background: transparent");
    ui->weatherIconView->setScene(m_weatherScene);

    // connect
    connect(&m_dateTimer, SIGNAL(timeout()), this, SLOT(UpdateCurrentDateTime()));
}

//========================================================
WeatherWidget::~WeatherWidget()
//========================================================
{
    delete ui;

    delete m_iconWidget;
    m_iconWidget = NULL;

    delete m_weatherScene;
    m_weatherScene = NULL;
}

//========================================================
void WeatherWidget::SetStyleSheet(const StyleSheet::StyleInfo& style)
//========================================================
{
    m_styleInfo = style;

    QString widgetStyleSheet = QString("QWidget {background-color: %1;}")
                                .arg(style.backgroundColor);
    QString labelStyleSheet = QString("QLabel {color: %1;}")
                                .arg(style.fontColor);

    if(true == style.bTransparency)
    {
        this->setAttribute(Qt::WA_TranslucentBackground, true);
    }
    else
    {
        this->setStyleSheet(widgetStyleSheet);
    }

    ui->cityLabel->setStyleSheet(labelStyleSheet);
    ui->temperLabel->setStyleSheet(labelStyleSheet);
    ui->statusLabel->setStyleSheet(labelStyleSheet);
    ui->dateTimeLabel->setStyleSheet(labelStyleSheet);
}

//========================================================
void WeatherWidget::SetPosSizeInfo(const StyleSheet::PosSizeInfo& posSizeInfo)
//========================================================
{
    m_posSizeInfo = posSizeInfo;
}

//========================================================
void WeatherWidget::MakeWeatherWidget(const WeatherInfo::DisplayValue& newValue)
//========================================================
{
    m_displayValue = newValue;

    // set weather icon
    // for test
    QString testPath = "/home/jaehoon/바탕화면/MediaPlayer-Test/MediaPlayer/MyWeather/icon/moon.svg";
    m_iconWidget = new QSvgWidget(testPath);
    m_iconWidget->setAttribute(Qt::WA_TranslucentBackground, true);
    m_weatherScene->addWidget(m_iconWidget);

    // set city name
    QString fullCityName = QString("%1 %2")
                            .arg(m_displayValue.metroCity)
                            .arg(m_displayValue.city);
    ui->cityLabel->setText(fullCityName);

    // set temperature
    QString temperatureStr = QString("%1℃").arg(m_displayValue.temperature);
    ui->temperLabel->setText(temperatureStr);

    // set status
    ui->statusLabel->setText(m_displayValue.status);

    // set dateTime
    const QString& dateTimeStr = MakeDateTimeStr();
    ui->dateTimeLabel->setText(dateTimeStr);
}

//========================================================
void WeatherWidget::StartDateTimeTimer()
//========================================================
{
    m_dateTimer.start(1000);
    m_bIsTimerStarted = true;
}

//========================================================
void WeatherWidget::StopDateTimeTimer()
//========================================================
{
    m_dateTimer.stop();
    m_bIsTimerStarted = false;
}

//========================================================
bool WeatherWidget::IsStartedDateTimeTimer()
//========================================================
{
    return m_bIsTimerStarted;
}

//========================================================
QString WeatherWidget::MakeDateTimeStr()
//========================================================
{
    QString retValue;

    const QDateTime currDateTime = QDateTime::currentDateTime();

    const char* dayOfWeekStr[] = { " ", "월요일", "화요일",
                               "수요일", "목요일", "금요일",
                                 "토요일", "일요일"};

    char buffer[64] = {'\0', };
    // example - 4월 12일 월요일 오후 4:24
    sprintf(buffer, "%d월 %d일 %s %s %d:%02d",
            currDateTime.date().month(), currDateTime.date().day(),
            dayOfWeekStr[currDateTime.date().dayOfWeek()],
            12 <= currDateTime.time().hour() ? "오후" : "오전",
            12 < currDateTime.time().hour() ? currDateTime.time().hour()-12
                                            : currDateTime.time().hour(),
            currDateTime.time().minute());

    retValue = buffer;

    return retValue;
}

//========================================================
void WeatherWidget::UpdateCurrentDateTime()
//========================================================
{
    const QString& dateTimeStr = MakeDateTimeStr();
    ui->dateTimeLabel->setText(dateTimeStr);
}
