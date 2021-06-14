#ifndef UPDATEMANAGER_H
#define UPDATEMANAGER_H

// QT
#include <QNetworkReply>
#include <QMainWindow>
#include <QNetworkAccessManager>
#include <QQueue>
#include <QFile>
#include <QTimer>

// Common
#include "Common/Deifinition.h"

QT_BEGIN_NAMESPACE
namespace Ui { class UpdateWindow; }
QT_END_NAMESPACE

class UpdateManager : public QMainWindow
{
    Q_OBJECT

public:
    /** @brief */
    UpdateManager(QWidget *parent = nullptr);
    /** @brief */
    ~UpdateManager();

    /** @brief */
    void GetLatestVersion();

    /** @brief */
    void CheckVersion();

private:
    /** @brief */
    bool StartNextDownload(const std::string url, const std::string savePath);

    /** @brief */
    bool GetCurrentVersion(QString& currVersion);

    /** @brief */
    void UpdateXmlVersion();

    /** @brief */
    bool ParseLatestVersion(const QString& src, QString& dest);

    /** @brief */
    void DecompressDownloadFile(const QString& path, const QString& destPath);

    /** @brief */
    static size_t WirteFileFunction(void *ptr, size_t size, size_t nmemb, FILE *stream);

private slots:
    /** @brief */
    void ReadyVersionRead();
    /** @brief */
    void ReadVersionFinish();

    /** @brief */
    void StartElgoClient();

private:
    Ui::UpdateWindow *ui;
    QRect m_screenRect;

    QNetworkAccessManager m_netManager;
    QNetworkReply *m_getVersionReply;

    QString m_serverVersion;
    QTimer m_startTimer;
};
#endif // UPDATEMANAGER_H
