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
    ~DateWidget();

    /** @brief */
    void MakeDateWidget();
    /** @brief */
    void SetStyleSheet(const StyleSheet::StyleInfo& style);
    /** @brief */
    void ConvertToStringFromDayOfWeek(const int src, QString& dest);

private slots:
    /** @brief */
    void UpdateCurrentDate();

private:
    Ui::DateWidget *ui;

    QRect m_dateLabelRect;
    StyleSheet::StyleInfo m_styleInfo;
    QDateTime m_dateTime;
    QTimer *m_updateTimer;
    bool m_bIsTimerStarted;
};

#endif // DATEWIDGET_H
