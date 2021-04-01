#ifndef CLOCKWIDGET_H
#define CLOCKWIDGET_H

// QT
#include <QWidget>
#include <QTimer>
#include <QDateTime>

// Viewer
#include "Widget/Definition/ContentsDef.h"

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
    void MakeClockWidget(ClockInfo::Kinds kind);
    /** @brief */
    void SetStyleSheet(const StyleSheet::StyleInfo& style);

private slots:
    /** @brief */
    void UpdateCurrentTime();

private:
    Ui::ClockWidget *ui;

    QRect m_amPmLabelRect;
    QRect m_timeLabelRect;

    ClockInfo::Kinds m_clockKinds;
    StyleSheet::StyleInfo m_styleInfo;
    QDateTime m_dateTime;
    QString m_amPmStr;
    QString m_timeStr;
    QTimer *m_updateTimer;
    bool m_bIsTimerStarted;
};

#endif // CLOCKWIDGET_H
