#ifndef UPDATEMANAGER_H
#define UPDATEMANAGER_H

// QT
#include <QMainWindow>
#include <QNetworkAccessManager>
#include <QQueue>
#include <QFile>
#include <QTimer>

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
    void CheckVersion();

private:
    /** @brief */
    void StartNextDownload();

    /** @brief */
    bool GetCurrentVersion(QString& currVersion);

    /** @brief */
    void UpdateXmlVersion();

private slots:
    /** @brief */
    void DownloadProgress(qint64 bytesReceived, qint64 bytesTotal);
    /** @brief */
    void DonwloadReadyToRead();
    /** @brief */
    void DownloadFinished();

    /** @brief */
    void StartElgoClient();

private:
    Ui::UpdateWindow *ui;
    QRect m_screenRect;

    QNetworkAccessManager m_netManager;
    QQueue<QUrl> m_downloadQueue;
    QFile m_outFile;
    QNetworkReply *m_netReply;

    int m_successCnt;
    QVector<QString> m_failedList;
    QString m_serverVersion;

    QTimer m_startTimer;
};
#endif // UPDATEMANAGER_H
