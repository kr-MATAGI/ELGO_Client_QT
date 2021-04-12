// QT
#include <QUrl>
#include <QDateTime>

// Viewer
#include "WeatherWidget.h"
#include "ui_WeatherWidget.h"
#include "Logger/ViewerLogger.h"

#define RESOURCE_ICON_PATH   "/home/jaehoon/바탕화면/ELGO/resource/icon";

//========================================================
WeatherWidget::WeatherWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::WeatherWidget)
    , m_bIsTimerStarted(false)
//========================================================
{
    ui->setupUi(this);

    m_iconWidget = new QSvgWidget;
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

    // widget
    const QPointF& widgetPos = posSizeInfo.pos;
    const QSize& widgetSize = posSizeInfo.size;
    const QRect widgetRect(widgetPos.toPoint(), widgetSize);
    this->setGeometry(widgetRect);
    ELGO_VIEWER_LOG("Widget pos{x: %f, y: %f}, size{w: %d, h: %d}",
                    widgetPos.x(), widgetPos.y(), widgetSize.width(), widgetSize.height());

    // city label
    const QPoint cityLabelPos(widgetSize.width() * 0.03, widgetSize.height() * 0.03);
    const QSize cityLabelSize(widgetSize.width() * 0.5, widgetSize.height() * 0.1);
    const QRect cityLabelRect(cityLabelPos, cityLabelSize);
    ui->cityLabel->setGeometry(cityLabelRect);
    ELGO_VIEWER_LOG("cityLabel pos{x: %f, y: %f}, size{w: %d, h: %d}",
                    cityLabelPos.x(), cityLabelPos.y(), cityLabelSize.width(), cityLabelSize.height());

    // dateTime label
    const QPoint dateTimeLabelPos(cityLabelPos.x(), cityLabelPos.y() * 4);
    const QSize dateTimeLabelSize(cityLabelSize);
    const QRect dateTimeLabelRect(dateTimeLabelPos, dateTimeLabelSize);
    ui->dateTimeLabel->setGeometry(dateTimeLabelRect);
    ELGO_VIEWER_LOG("dateTimeLabel pos{x: %f, y: %f}, size{w: %d, h: %d}",
                    dateTimeLabelPos.x(), dateTimeLabelPos.y(), dateTimeLabelSize.width(), dateTimeLabelSize.height());

    QFont leftLabelFont;
    const int leftLabelFontSize = cityLabelSize.height() * 0.5;
    leftLabelFont.setBold(true);
    leftLabelFont.setPixelSize(leftLabelFontSize);
    ELGO_VIEWER_LOG("Left Label Font Size : %d", leftLabelFontSize);
    ui->cityLabel->setFont(leftLabelFont);
    ui->dateTimeLabel->setFont(leftLabelFont);

    // icon svg widget
    const QPoint svgWidgetPos(cityLabelPos.x(), dateTimeLabelPos.y() * 3);
    const QSize svgWidgetSize(widgetSize.width() * 0.4, widgetSize.height() * 0.5);
    const QRect svgWidgetRect(svgWidgetPos, svgWidgetSize);
    ui->weatherIconView->setGeometry(svgWidgetRect);
    m_iconWidget->setGeometry(svgWidgetRect);
    ELGO_VIEWER_LOG("svgWidget pos{x: %f, y: %f}, size{w: %d, h: %d}",
                    svgWidgetPos.x(), svgWidgetPos.y(), svgWidgetSize.width(), svgWidgetSize.height());

    // temperature label
    const int svgRight = ui->weatherIconView->rect().right() * 2;
    const QPoint temperLabelPos(svgRight, svgWidgetPos.y());
    const QSize temperLabelSize(svgWidgetSize.width() * 0.4, svgWidgetSize.height() * 0.4);
    const QRect temperLabelRect(temperLabelPos, temperLabelSize);
    ui->temperLabel->setGeometry(temperLabelRect);
    ELGO_VIEWER_LOG("temperLabel pos{x: %f, y: %f}, size{w: %d, h: %d}",
                    temperLabelPos.x(), temperLabelPos.y(), temperLabelSize.width(), temperLabelSize.height());

    QFont temperLabelFont;
    const int temperLabelFontSize = temperLabelSize.height() * 0.5;
    temperLabelFont.setBold(true);
    temperLabelFont.setPixelSize(temperLabelFontSize);
    ELGO_VIEWER_LOG("temperLabelFontSize : %d", temperLabelFontSize);
    ui->temperLabel->setFont(temperLabelFont);

    // status label
    const QPoint statusLabelPos(temperLabelPos.x() * 0.6, temperLabelPos.y() * 2.5);
    const QSize statusLabelSize(temperLabelSize.width() * 0.6, temperLabelSize.height() * 0.6);
    const QRect statusLabelRect(statusLabelPos, statusLabelSize);
    ui->statusLabel->setGeometry(statusLabelRect);
    ELGO_VIEWER_LOG("statusLabel pos{x: %f, y: %f}, size{w: %d, h: %d}",
                    statusLabelPos.x(), statusLabelPos.y(), statusLabelSize.width(), statusLabelSize.height());

    QFont statusLabelFont;
    const int statusLabelFontSize = statusLabelSize.height() * 0.6;
    statusLabelFont.setBold(true);
    statusLabelFont.setPixelSize(statusLabelFontSize);
    ELGO_VIEWER_LOG("statusLabelFontSize : %d", statusLabelFontSize);
    ui->statusLabel->setFont(statusLabelFont);
}

//========================================================
void WeatherWidget::MakeWeatherWidget(const WeatherInfo::DisplayValue& newValue)
//========================================================
{
    m_displayValue = newValue;

    // set weather icon
    // for test
    const QString iconPath = RESOURCE_ICON_PATH;

    /**
      * @note
      *         sky.sunny == sunny.svg
      *         sky.cloudy == cloud.svg
      *         sky.fog == fog.svg
      *         pty.rain == rain.svg
      *         pty.sleet == s_snow.svg
      *         pty.snow == snow.svg
      *         pty.shower == s_rain.svg
      *         pty.raindrop == rain.svg
      *         pty.raindropsnw = rain_snow.svg
      */

    QString iconFile;
    if(PlayJson::SKY::SUNNY == m_displayValue.sky)
    {

    }
    else if(PlayJson::SKY::CLOUDY == m_displayValue.sky)
    {

    }
    else if(PlayJson::SKY::FOG == m_displayValue.sky)
    {

    }
    else if(PlayJson::PTY::RAIN == m_displayValue.pty)
    {

    }
    else if(PlayJson::PTY::SLEET == m_displayValue.pty)
    {

    }
    else if(PlayJson::PTY::SNOW == m_displayValue.pty)
    {

    }
    else if(PlayJson::PTY::SHOWER == m_displayValue.pty)
    {

    }
    else if(PlayJson::PTY::RAIN_DROP == m_displayValue.pty)
    {

    }
    else if(PlayJson::PTY::RAIN_DROP_SNOW == m_displayValue.pty)
    {

    }

    m_iconWidget->load(iconPath);
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
