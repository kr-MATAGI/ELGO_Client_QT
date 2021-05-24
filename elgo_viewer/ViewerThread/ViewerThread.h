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
#include "Widget/ContentsPlayer.h"

class MainWindow;
class ViewerController;
class ContentsPlayer;

class ViewerThread : public QRunnable
{
public:
    /** @brief */
    ViewerThread();
    /** @brief */
    ~ViewerThread();

public:
    /** @brief  QRunnable run() override*/
    void run() override;

    /** @brief  Make and dispay QrCode */
    void ExecMakeQrCodeThread();

    /** @brief */
    void ExecPlayCustomPlayData();
    /** @brief */
    void ExecPlayFixedPlayData();

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
