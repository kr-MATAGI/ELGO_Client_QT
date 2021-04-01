#ifndef VIEWERTHREADPOOL_H
#define VIEWERTHREADPOOL_H

// QT
#include <QRunnable>
#include <QByteArray>

// EFC
#include "Common/EventList.h"

// Viewer
#include "MainWindow.h"
#include "ViewerCtrl/ViewerController.h"
#include "Widget/SchedulesTimer/SchedulesTimer.h"

class MainWindow;
class ViewerController;
class SchedulesTimer;

class ViewerThread : public QRunnable
{

public:
    ViewerThread();
    ~ViewerThread();

public:
    /** @brief  QRunnable run() override*/
    void run() override;

    /** @brief  Make and dispay QrCode */
    void ExecMakeQrCodeThread();

    /** @brief */
    void ExecRotateDeviceDisplay();

    /** @brief */
    void ExecCustomPlayData();
    /** @brief */
    void ExecFixedPlayData();

    /** @brief */
    void ExecCustomPlaySchedules();
    /** @brief */
    void ExecFixedPlaySchedules();

    /** @brief */
    void ExecScreenCapture();

public:
    /** @brief */
    void SetViewerEvent(VIEWER_EVENT::Event event);

    /** @brief */
    void SetRecvBytes(const QByteArray &src);

private:
    /** @brief */
    bool CaptureCurrentScreen(const QString& savedPath);

private:
    VIEWER_EVENT::Event m_event;
    QByteArray m_bytes;
};

#endif // VIEWERTHREADPOOL_H
