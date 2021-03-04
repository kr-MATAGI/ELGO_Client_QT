#ifndef VIEWERCTRL_H
#define VIEWERCTRL_H

// QT
#include <QString>

class ViewerCtrl
{
public:
    ViewerCtrl();
    ~ViewerCtrl();

public:
    /** @brief  notify process status */
    void SendViewerIsReady();

    /** @brief */
    void SetQRCodeURL(const QString &url);

    /** @brief */
    QString& GetQRCodeURL();

private:
    QString m_qrCodeUrl;
};

#endif // VIEWERCTRL_H
