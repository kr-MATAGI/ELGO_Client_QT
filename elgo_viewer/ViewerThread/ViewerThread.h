#ifndef VIEWERTHREADPOOL_H
#define VIEWERTHREADPOOL_H

// QT
#include <QRunnable>
#include <QByteArray>

// EFC
#include "Common/EventList.h"

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

public:
    /** @brief */
    void SetViewerEvent(VIEWER_EVENT::Event event);

    /** @brief */
    void SetRecvBytes(const QByteArray &src);

private:
    VIEWER_EVENT::Event m_event;
    QByteArray m_bytes;
};

#endif // VIEWERTHREADPOOL_H
