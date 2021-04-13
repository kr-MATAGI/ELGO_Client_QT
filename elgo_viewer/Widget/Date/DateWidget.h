#ifndef DATEWIDGET_H
#define DATEWIDGET_H

// QT
#include <QWidget>
#include <QTimer>
#include <QDateTime>

// Viewer
#include "Widget/Definition/ContentsDef.h"

namespace Ui {
class DateWidget;
}

class DateWidget : public QWidget
{
    Q_OBJECT

public:
    explicit DateWidget(QWidget *parent = nullptr);
    virtual ~DateWidget();

    /** @brief */
    void MakeDateLabelString();
    /** @brief */
    void SetStyleSheet(const StyleSheet::StyleInfo& style);
    /** @brief */
    void SetPosSizeinfo(const StyleSheet::PosSizeInfo& posSizeInfo);

    /** @brief */
    void StartDateWidget();
    /** @brief */
    void StopDateWidget();
    /** @brief */
    bool IsStartedDateTimer();

    /** @brief */
    void ConvertToStringFromDayOfWeek(const int src, QString& dest);

private slots:
    /** @brief */
    void UpdateCurrentDate();

private:
    Ui::DateWidget *ui;

    StyleSheet::StyleInfo m_styleInfo;
    StyleSheet::PosSizeInfo m_posSizeInfo;

    QDateTime m_dateTime;
    QTimer *m_updateTimer;
    bool m_bIsTimerStarted;
};

#endif // DATEWIDGET_H
