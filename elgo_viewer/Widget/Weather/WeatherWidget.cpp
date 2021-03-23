// QT
#include <QUrl>

// Viewer
#include "WeatherWidget.h"
#include "ui_WeatherWidget.h"

//========================================================
WeatherWidget::WeatherWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::WeatherWidget)
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

    QFont font;
    font.setBold(true);
    font.setPointSize(30);
    ui->cityLabel->setFont(font);
    ui->temperLabel->setFont(font);
    ui->statusLabel->setFont(font);

    ui->cityLabel->setAlignment(Qt::AlignCenter);
    ui->temperLabel->setAlignment(Qt::AlignCenter);
    ui->statusLabel->setAlignment(Qt::AlignCenter);
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
}

//========================================================
void WeatherWidget::SetWeatherInfo(const Weather::DisplayInfo& info)
//========================================================
{
    m_weatherInfo = info;

    // set weather icon
    // for test
    QString testPath = "/home/jaehoon/바탕화면/MediaPlayer-Test/MediaPlayer/MyWeather/icon/moon.svg";
    m_iconWidget = new QSvgWidget(testPath);
    m_iconWidget->setAttribute(Qt::WA_TranslucentBackground, true);
    m_weatherScene->addWidget(m_iconWidget);

    // set city name
    ui->cityLabel->setText(info.city);

    // set temperature
    QString temperatureStr = QString("%1℃").arg(info.temperature);
    ui->temperLabel->setText(temperatureStr);

    // set status
    ui->statusLabel->setText(info.status);
}
