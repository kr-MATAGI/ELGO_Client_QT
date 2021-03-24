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

    /** @brief */
    void SetIsDisplayQr(const bool value);
    /** @brief */
    bool GetIsDisplayQr() const;

private:
    QString m_qrCodeUrl;
    bool m_bIsDisplayQr;
};

#endif // VIEWERCTRL_H
