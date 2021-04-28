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

    // icon svg widget
    const QPoint svgWidgetPos(widgetSize.width() * 0.1, widgetSize.height() * 0.1);
    const QSize svgWidgetSize(widgetSize.width() * 0.4, widgetSize.height() * 0.6);
    const QRect svgWidgetRect(svgWidgetPos, svgWidgetSize);
    ui->weatherIconView->setGeometry(svgWidgetRect);
    m_iconWidget->setGeometry(svgWidgetRect);
    ELGO_VIEWER_LOG("svgWidget pos{x: %f, y: %f}, size{w: %d, h: %d}",
                    svgWidgetPos.x(), svgWidgetPos.y(), svgWidgetSize.width(), svgWidgetSize.height());

    // city label
    const QPoint cityLabelPos(svgWidgetPos.x(), svgWidgetSize.height() * 1.15);
    const QSize cityLabelSize(svgWidgetSize.width(), widgetSize.height() * 0.13);
    const QRect cityLabelRect(cityLabelPos, cityLabelSize);
    ui->cityLabel->setGeometry(cityLabelRect);
    ELGO_VIEWER_LOG("cityLabel pos{x: %f, y: %f}, size{w: %d, h: %d}",
                    cityLabelPos.x(), cityLabelPos.y(), cityLabelSize.width(), cityLabelSize.height());

    // dateTime label
    const QPoint dateTimeLabelPos(cityLabelPos.x(), cityLabelPos.y() + cityLabelSize.height());
    const QSize dateTimeLabelSize(widgetSize.width(), cityLabelSize.height());
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

    // temperature label
    const QPoint temperLabelPos(svgWidgetRect.right() * 1.1, svgWidgetPos.y() * 0.9);
    const QSize temperLabelSize(widgetSize.width() * 0.4, widgetSize.height() * 0.4);
    const QRect temperLabelRect(temperLabelPos, temperLabelSize);
    ui->temperLabel->setGeometry(temperLabelRect);
    ELGO_VIEWER_LOG("temperLabel pos{x: %f, y: %f}, size{w: %d, h: %d}",
                    temperLabelPos.x(), temperLabelPos.y(), temperLabelSize.width(), temperLabelSize.height());

    QFont temperLabelFont;
    const int temperLabelFontSize = temperLabelSize.height() * 0.6;
    temperLabelFont.setBold(true);
    temperLabelFont.setPixelSize(temperLabelFontSize);
    ELGO_VIEWER_LOG("temperLabelFontSize : %d", temperLabelFontSize);
    ui->temperLabel->setFont(temperLabelFont);

    // status label
    const QPoint statusLabelPos(temperLabelPos.x(), temperLabelRect.bottom());
    const QSize statusLabelSize(temperLabelSize.width(), temperLabelSize.height() * 0.5);
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
    const QString iconPath = MakeWeatherIconFilePath(m_displayValue);
    ELGO_VIEWER_LOG("Weather icon path : %s", iconPath.toStdString().c_str());

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
    const QString statusStr = MakeStatusLabelString(m_displayValue);
    ui->statusLabel->setText(statusStr);

    // set dateTime
    const QString& dateTimeStr = MakeDateTimeStr();
    ui->dateTimeLabel->setText(dateTimeStr);
}

//========================================================
void WeatherWidget::StartDateTimeTimer()
//========================================================
{
    UpdateCurrentDateTime();
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
QString WeatherWidget::MakeWeatherIconFilePath(const WeatherInfo::DisplayValue& displayValue)
//========================================================
{
    /**
      * @note
      *         sky.sunny == sunny.svg
      *         sky.much_cloud == cloud_1_sunny.svg
      *         sky.cloudy == cloud.svg
      *         pty.rain == rain.svg
      *         pty.sleet == s_snow.svg
      *         pty.snow == snow.svg
      *         pty.shower == s_rain.svg
      *         pty.raindrop == s_rain.svg
      *         pty.raindropsnow = rain_snow.svg
      *         lgt(true) == lightning.svg
      *         lgt(true), pty(rain) == rain_lightning
      */

    QString retValue = RESOURCE_ICON_PATH;
    ELGO_VIEWER_LOG("Display Value : {LGT: %d, SKY: %d, PTY: %d}",
                    displayValue.lgt, displayValue.pty, displayValue.sky);

    if(true == displayValue.lgt)
    {
        if(PlayJson::PTY::RAIN == displayValue.pty)
        {
            retValue += "/rain_lightning.svg";
        }
        else
        {
            retValue += "/lightning.svg";
        }
    }
    else if(PlayJson::SKY::SUNNY == displayValue.sky)
    {
        retValue += "/sunny.svg";
    }
    else if(PlayJson::SKY::MUCH_CLOUDY == displayValue.sky)
    {
        retValue += "/cloud_1_sunny.svg";
    }
    else if(PlayJson::SKY::CLOUDY == displayValue.sky)
    {
        retValue += "/cloud.svg";
    }
    else if(PlayJson::PTY::RAIN == displayValue.pty)
    {
        retValue += "/rain.svg";
    }
    else if(PlayJson::PTY::SLEET == displayValue.pty)
    {
        retValue += "/s_snow.svg";
    }
    else if(PlayJson::PTY::SNOW == displayValue.pty)
    {
        retValue += "/snow.svg";
    }
    else if(PlayJson::PTY::SHOWER == displayValue.pty)
    {
        retValue += "/s_rain.svg";
    }
    else if(PlayJson::PTY::RAIN_DROP == displayValue.pty)
    {
        retValue += "/s_rain.svg";
    }
    else if(PlayJson::PTY::RAIN_DROP_SNOW == displayValue.pty)
    {
        retValue += "/rain_snow.svg";
    }
    else
    {
        // Not matcing
        retValue += "/sunny.svg";
        ELGO_VIEWER_LOG("Error - Not matched weather icon")
    }

    return retValue;
}

//========================================================
QString WeatherWidget::MakeStatusLabelString(const WeatherInfo::DisplayValue& displayValue)
//========================================================
{
    /**
      * @note
      *         sky.sunny == 맑음
      *         sky.much_cloud == 구름 많음
      *         sky.cloudy == 흐림
      *         pty.rain == 비
      *         pty.sleet == 눈 조금
      *         pty.snow == 눈
      *         pty.shower == 소나기
      *         pty.raindrop == 빗방울
      *         pty.raindropsnow = 빗방울/눈날림
      *         lgt(true) == 낙뢰
      *         lgt(true), pty(rain) == 낙뢰/비
      */

    QString retValue;
    ELGO_VIEWER_LOG("Display Value : {LGT: %d, SKY: %d, PTY: %d}",
                    displayValue.lgt, displayValue.pty, displayValue.sky);

    if(true == displayValue.lgt)
    {
        if(PlayJson::PTY::RAIN == displayValue.pty)
        {
            retValue = "낙뢰/비";
        }
        else
        {
            retValue = "낙뢰";
        }
    }
    else if(PlayJson::SKY::SUNNY == displayValue.sky)
    {
        retValue = "맑음";
    }
    else if(PlayJson::SKY::MUCH_CLOUDY == displayValue.sky)
    {
        retValue = "구름 많음";
    }
    else if(PlayJson::SKY::CLOUDY == displayValue.sky)
    {
        retValue = "흐림";
    }
    else if(PlayJson::PTY::RAIN == displayValue.pty)
    {
        retValue = "비";
    }
    else if(PlayJson::PTY::SLEET == displayValue.pty)
    {
        retValue = "눈 조금";
    }
    else if(PlayJson::PTY::SNOW == displayValue.pty)
    {
        retValue = "눈";
    }
    else if(PlayJson::PTY::SHOWER == displayValue.pty)
    {
        retValue = "소나기";
    }
    else if(PlayJson::PTY::RAIN_DROP == displayValue.pty)
    {
        retValue = "빗방울/눈";
    }
    else if(PlayJson::PTY::RAIN_DROP_SNOW == displayValue.pty)
    {
        retValue = "눈날림";
    }
    else
    {
        // Not matcing
        retValue = "맑음";
        ELGO_VIEWER_LOG("Error - Not matched weather icon")
    }

    return retValue;
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
