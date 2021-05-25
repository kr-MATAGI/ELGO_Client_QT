#ifndef UPDATEWIDGETTIMER_H
#define UPDATEWIDGETTIMER_H

// QT
#include <QTimer>

// Control
#include "Common/Interface/ContentsPlayDataDef.h"
#include "NetworkCtrl/NetworkController.h"

// def
#define UPDATE_TIMEOUT (30 * 60 * 1000)

class NetworkController;

class UpdateWidgetTimer : public QTimer
{
    Q_OBJECT

public:
    /** @brief */
    explicit UpdateWidgetTimer(QObject *parent = nullptr);
    /** @brief */
    virtual ~UpdateWidgetTimer();

signals:
    /** @brief */
    void UpdateWidgetInfoSignal(const QVector<PlayJson::UpdateWidgetInfo>& src);
    /** @brief */
    void AddWidgetInfoSignal(const QVector<PlayJson::UpdateWidgetInfo>& src);
    /** @brief */
    void CleaerUpdateWidgetListSignal();

    /** @brief */
    void StartUpdateWidgetTimer();
    /** @brief */
    void StopUpdateWidgetTimer();

private slots:
    /** @brief */
    void UpdateWidgetInfoSlot(const QVector<PlayJson::UpdateWidgetInfo>& src);
    /** @brief */
    void AddWidgetInfoSlot(const QVector<PlayJson::UpdateWidgetInfo>& src);
    /** @brief */
    void CleaerUpdateWidgetListSlot();

    /** @brief */
    void StartUpdateTimer();
    /** @brief */
    void StopUpdateTimer();

    /** @brief */
    void UpdateWidgetTimeout();

private:
    QVector<PlayJson::UpdateWidgetInfo> m_updateInfoList;
};

#endif // UPDATEWIDGETTIMER_H
