// QT
#include <QScreen>
#include <QNetworkReply>
#include <QtXml>

// Update
#include "UpdateWindow.h"
#include "ui_UpdateWindow.h"
#include "Logger/UpdateLogger.h"

// Define
#define ELGO_UPDATE_URL "http://cloud.elgo.co.kr/update/ubuntu"
#define VERSION_CHECK_URL   ""

#define CONFIG_XML   "/home/jaehoon/바탕화면/ELGO/config.xml"
#define ELGO_MAIN_PATH  "/home/jaehoon/바탕화면/ELGO/build-ELGO_Client-Desktop_Qt_5_15_2_GCC_64bit-Release/elgo_main/elgo_main"
#define ELGO_REMOTE_PATH    "/home/jaehoon/바탕화면/ELGO/ELGO_Remote/elgo_remote/build"

#define START_TIMEOUT   3000

//========================================================
UpdateManager::UpdateManager(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::UpdateWindow)
    , m_successCnt(0)
//========================================================
{
    ui->setupUi(this);

    // init
    this->setWindowTitle("ELGO Update Manager");

    QList<QScreen *> screens = QApplication::screens();
    if(0 < screens.size())
    {
        QSize screenSize = screens[0]->size();
        const QRect screenRect = QRect(QPoint(0.0, 0.0), screenSize);

        m_screenRect = screenRect;
    }

    const QRect& windowRect = this->frameGeometry();
    const QPoint& center = m_screenRect.center();
    this->move( (center.x() - windowRect.width() / 2),
                (center.y() - windowRect.height() / 2) );

    QFont labelFont;
    labelFont.setPointSize(15);
    ui->label->setFont(labelFont);

    // connect
    connect(&m_startTimer, &QTimer::timeout,
            this, &UpdateManager::StartElgoClient);
}

//========================================================
UpdateManager::~UpdateManager()
//========================================================
{
    delete ui;
}

//========================================================
void UpdateManager::GetLatestVersion()
//========================================================
{
    QUrl url;
    QNetworkRequest request(url);
    m_getVersionReply = m_netManager.get(request);

    // connect
    connect(m_getVersionReply, &QNetworkReply::readyRead,
            this, &UpdateManager::ReadyVersionRead);
    connect(m_getVersionReply, &QNetworkReply::finished,
            this, &UpdateManager::ReadVersionFinish);
}

//========================================================
void UpdateManager::CheckVersion()
//========================================================
{
    bool bIsNeedNew = false;

    // Read Current Version From XML
    QString currVersion;
    const bool bIsParsing = GetCurrentVersion(currVersion);
    if(true == bIsParsing)
    {
        // Recv Latest Version
        QString serverVersion = "1.0";

        // Compare Version
        const double currVersionNum = std::stod(currVersion.toStdString());
        const double serverVersionNum = std::stod(serverVersion.toStdString());
        if(currVersionNum < serverVersionNum)
        {
            QString startMsg = QString("현재: %1, 최신: %2 - 업데이트를 시작합니다.")
                                    .arg(currVersion)
                                    .arg(serverVersion);
            ui->label->setText(startMsg);
            bIsNeedNew = true;
        }
    }
    else
    {
        ui->label->setText("XML 파일 읽기 실패");
    }

    // Download New Binary
    if(true == bIsNeedNew)
    {
        QVector<QString> procList = {"elgo_main", "elgo_control",
                                     "elgo_viewer", "elgo_remote"};

        for(int idx = 0; idx < procList.size(); idx++)
        {
            QString url = ELGO_UPDATE_URL;
            url.append("/");
            url.append(procList[idx]);
            QUrl downloadUrl(url);

            m_downloadQueue.enqueue(downloadUrl);
        }

        StartNextDownload();
    }
    else
    {
        ui->label->setText("모든 프로그램이 최신 버전입니다.");
        ui->progressBar->hide();

        m_startTimer.setSingleShot(true);
        m_startTimer.start(START_TIMEOUT);
    }
}

//========================================================
void UpdateManager::StartNextDownload()
//========================================================
{
    if(true == m_downloadQueue.isEmpty())
    {
        QString endLog;
        if(4 == m_successCnt)
        {
            endLog = QString("업데이트를 완료하였습니다. (%1/3)")
                                .arg(QString::number(m_successCnt));

            // Update XML
            UpdateXmlVersion();
        }
        else
        {
            endLog = QString("일부 파일의 업데이트가 실패 했습니다. -{ ");
            foreach(auto item, m_failedList)
            {
                endLog.append(item + " ");
            }
            endLog.append(" }");
        }
        ui->label->setText(endLog);

        m_startTimer.setSingleShot(true);
        m_startTimer.start(START_TIMEOUT);
    }

    QUrl url = m_downloadQueue.dequeue();
    QString fileName = url.toString().split("/").back();
    m_outFile.setFileName(fileName);
    if(false == m_outFile.open(QIODevice::WriteOnly))
    {
        QString errMsg = QString("%1 파일 열기 실패").arg(fileName);
        ui->label->setText(errMsg);
        StartNextDownload();

        return;
    }

    QNetworkRequest request(url);
    m_downloadReply = m_netManager.get(request);
    QString downloadMsg = QString("%1 을 업데이트하고 있습니다.").arg(fileName);
    ui->label->setText(downloadMsg);

    //connect
    connect(m_downloadReply, &QNetworkReply::downloadProgress,
            this, &UpdateManager::DownloadProgress);
    connect(m_downloadReply, &QNetworkReply::finished,
            this, &UpdateManager::DownloadFinished);
    connect(m_downloadReply, &QNetworkReply::readyRead,
            this, &UpdateManager::DonwloadReadyToRead);
}

//========================================================
bool UpdateManager::GetCurrentVersion(QString& currVersion)
//========================================================
{
    bool retValue = false;

    QDomDocument xmlDoc;
    QFile xmlFile(CONFIG_XML);
    if(false == xmlFile.open(QIODevice::ReadOnly))
    {
        return retValue;
    }

    if(false == xmlDoc.setContent(&xmlFile))
    {
        return retValue;
    }

    QDomElement root = xmlDoc.documentElement();
    if("configuration" == root.tagName())
    {
        if(false == root.isNull())
        {
            root = root.firstChildElement();
            if("version" == root.tagName())
            {
                if(false == root.isNull())
                {
                    retValue = true;
                    currVersion = root.firstChild().toText().data();
                }
            }
        }
    }

    xmlFile.close();

    return retValue;
}

//========================================================
void UpdateManager::UpdateXmlVersion()
//========================================================
{
    QDomDocument xmlDoc;
    QFile xmlFile(CONFIG_XML);
    if(false == xmlFile.open(QIODevice::ReadWrite))
    {
        return;
    }

    if(false == xmlDoc.setContent(&xmlFile))
    {
        return;
    }

    QDomElement root = xmlDoc.documentElement();
    if("configuration" == root.tagName())
    {
        if(false == root.isNull())
        {
            root = root.firstChildElement();
            if("version" == root.tagName())
            {
                if(false == root.isNull())
                {
                    root.firstChild().setNodeValue(m_serverVersion);
                    xmlFile.resize(0);
                    xmlFile.write(xmlDoc.toByteArray());
                }
            }
        }
    }

    xmlFile.close();
}

//========================================================
void UpdateManager::StartElgoClient()
//========================================================
{
    // ELGO_Main
    QProcess mainProcess;
    QStringList mainArgs;
    mainProcess.startDetached(ELGO_MAIN_PATH, mainArgs);
    mainProcess.waitForFinished();

    // ELGO_Remote
    QProcess remoteProcess;
    QStringList remoteArgs;
    QString remoteCmd = "/bin/sh";
    QString remoteExec = QString("cd %1 && npm start")
            .arg(ELGO_REMOTE_PATH);
    remoteArgs << "-c";
    remoteArgs << remoteExec;

    remoteProcess.start(remoteCmd, remoteArgs);
    remoteProcess.waitForFinished();

    mainProcess.deleteLater();
    remoteProcess.deleteLater();

    exit(0);
}

//========================================================
void UpdateManager::ReadyVersionRead()
//========================================================
{
    QString recvJson = m_getVersionReply->readAll();

    // Parsing
    ParseLatestVersion(recvJson, m_serverVersion);
}

//========================================================
void UpdateManager::ReadVersionFinish()
//========================================================
{
    m_getVersionReply->deleteLater();
}

//========================================================
void UpdateManager::DownloadProgress(qint64 bytesReceived, qint64 bytesTotal)
//========================================================
{
    int percentVal = (bytesTotal / bytesReceived) * 100;
    ui->progressBar->setValue(percentVal);
}

//========================================================
void UpdateManager::DonwloadReadyToRead()
//========================================================
{
    m_outFile.write(m_downloadReply->readAll());
}

//========================================================
void UpdateManager::DownloadFinished()
//========================================================
{
    m_outFile.close();

    if(m_downloadReply->error())
    {
        ui->label->setText(m_downloadReply->errorString());
        m_outFile.remove();
    }
    else
    {
        m_successCnt++;

        QString successMsg = QString("%1 업데이트 완료").arg(m_outFile.fileName());
        ui->label->setText(successMsg);

        QStringList fileNameSplit = m_outFile.fileName().split("/");
        if(0 == strcmp(fileNameSplit.back().toStdString().c_str(),
                       "elgo_update"))
        {
            // Decompression
//            DecompressDownloadFile(m_outFile.fileName());
        }
    }

    m_downloadReply->deleteLater();
    StartNextDownload();
}

//========================================================
bool UpdateManager::ParseLatestVersion(const QString& src, QString& dest)
//========================================================
{
    bool retValue = false;

    const QJsonDocument& doc = QJsonDocument::fromJson(src.toUtf8());
    const QJsonObject& jsonObj = doc.object();

    if(jsonObj.end() != jsonObj.find("version"))
    {
        retValue = true;
        dest = jsonObj["version"].toString();
    }

    return retValue;
}

//========================================================
void UpdateManager::DecompressDownloadFile(const QString& path)
//========================================================
{

}
