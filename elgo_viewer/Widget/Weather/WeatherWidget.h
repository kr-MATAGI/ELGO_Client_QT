#ifndef WEATHERWIDGET_H
#define WEATHERWIDGET_H

// QT
#include <QWidget>
#include <QTimer>
#include <QGraphicsScene>
#include <QSvgWidget>

// Viewer
#include "Widget/Definition/ContentsDef.h"

namespace Ui {
class WeatherWidget;
}

class WeatherWidget : public QWidget
{
    Q_OBJECT

public:
    explicit WeatherWidget(QWidget *parent = nullptr);
    virtual ~WeatherWidget();

    /** @brief */
    void MakeWeatherWidget(const WeatherInfo::DisplayValue& newValue);
    /** @brief */
    void SetStyleSheet(const StyleSheet::StyleInfo& style);
    /** @brief */
    void SetPosSizeInfo(const StyleSheet::PosSizeInfo& posSizeInfo);

    /** @brief */
    void StartDateTimeTimer();
    /** @brief */
    void StopDateTimeTimer();
    /** @brief */
    bool IsStartedDateTimeTimer();

    /** @brief */
    QString MakeDateTimeStr();
    /** @brief */
    QString MakeWeatherIconFilePath(const WeatherInfo::DisplayValue& displayValue);
    /** @brief */
    QString MakeStatusLabelString(const WeatherInfo::DisplayValue& displayValue);

private slots:
    void UpdateCurrentDateTime();

private:
    Ui::WeatherWidget *ui;

    QGraphicsScene *m_weatherScene;
    QSvgWidget *m_iconWidget;

    StyleSheet::StyleInfo m_styleInfo;
    StyleSheet::PosSizeInfo m_posSizeInfo;
    WeatherInfo::DisplayValue m_displayValue;

    QTimer m_dateTimer;
    bool m_bIsTimerStarted;
};

#endif // WEATHERWIDGET_H
