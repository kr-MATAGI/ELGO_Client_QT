#ifndef CLOCKWIDGET_H
#define CLOCKWIDGET_H

// QT
#include <QWidget>
#include <QTimer>
#include <QDateTime>

// Viewer
#include "Widget/Definition/ContentsDef.h"
#include "Common/Interface/ContentsPlayDataDef.h"

namespace Ui {
class ClockWidget;
}

class ClockWidget : public QWidget
{
    Q_OBJECT

public:
    /** @brief */
    explicit ClockWidget(QWidget *parent = nullptr);
    /** @brief */
    ~ClockWidget();

    /** @brief */
    void MakeClockTimeString(PlayJson::HourType type);
    /** @brief */
    void SetStyleSheet(const StyleSheet::StyleInfo& styleInfo);
    /** @brief */
    void SetPosSizeInfo(const StyleSheet::PosSizeInfo& posSizeInfo);

    /** @brief */
    void StartClock();
    /** @brief */
    void StopClock();
    /** @brief */
    bool IsStartedClock();

    /** @brief */
    int CalcLabelFontSize(const int labelWidth);

private slots:
    /** @brief */
    void UpdateCurrentTime();

private:
    Ui::ClockWidget *ui;

    PlayJson::HourType m_hourType;
    StyleSheet::StyleInfo m_styleInfo;
    StyleSheet::PosSizeInfo m_posSizeInfo;

    QDateTime m_dateTime;
    QString m_amPmStr;
    QString m_timeStr;

    QTimer *m_updateTimer;
    bool m_bIsTimerStarted;
};

#endif // CLOCKWIDGET_H
