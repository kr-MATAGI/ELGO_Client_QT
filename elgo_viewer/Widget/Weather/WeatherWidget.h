#ifndef WEATHERWIDGET_H
#define WEATHERWIDGET_H

// QT
#include <QWidget>
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
    ~WeatherWidget();

    /** @brief */
    void SetWeatherInfo(const WeatherInfo::DisplayInfo& info);

    /** @brief */
    void SetStyleSheet(const StyleSheet::StyleInfo& style);

private:
    Ui::WeatherWidget *ui;

    StyleSheet::StyleInfo m_styleInfo;
    WeatherInfo::DisplayInfo m_weatherInfo;
    QGraphicsScene *m_weatherScene;
    QSvgWidget *m_iconWidget;
};

#endif // WEATHERWIDGET_H
