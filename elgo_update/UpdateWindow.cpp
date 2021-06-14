// QT
#include <QProcess>
#include <QScreen>
#include <QNetworkReply>
#include <QtXml>

#include <curl/curl.h>

// Update
#include "UpdateWindow.h"
#include "ui_UpdateWindow.h"
#include "Logger/UpdateLogger.h"

// Define
#define ELGO_Client_URL "https://elgo-common.s3.ap-northeast-2.amazonaws.com/update/ubuntu/elgo_client.tar"
#define ELGO_Remote_URL "https://cloud.elgo.co.kr/update/ubuntu/elgo_remote.tar"
#define VERSION_CHECK_URL   "https://demo.elgo.co.kr/client/version/ubuntu/main"

#define CONFIG_XML   "/opt/ELGO/config.xml"
#define ELGO_Client_PATH    "/opt/ELGO/elgo_client.tar"
#define ELGO_Remote_PATH    "/opt/ELGO/elgo_remote.tar"

#define ELGO_Remote_DECMP_PATH  "/opt/ELGO/Remote/build/"
#define ELGO_Client_DECMP_PATH  "/opt/ELGO/"

#define ELGO_MAIN_PROC  "/opt/ELGO/elgo_main"

#define START_TIMEOUT   3000

//========================================================
UpdateManager::UpdateManager(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::UpdateWindow)
//========================================================
{
    ui->setupUi(this);

    // init
    this->setWindowTitle("ELGO Update Manager");
    ui->progressBar->hide();

    QList<QScreen *> screens = QApplication::screens();
    if(0 < screens.size())
    {
        QSize screenSize = screens[0]->size();
        const QRect screenRect = QRect(QPoint(0.0, 0.0), screenSize);

        m_screenRect = screenRect;
    }

    // move center
    const QRect& windowRect = this->frameGeometry();
    const QPoint& center = m_screenRect.center();
    this->move( (center.x() - windowRect.width() / 2),
                (center.y() - windowRect.height() / 2) );

    // device label name
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
    QUrl url(VERSION_CHECK_URL);
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
    if(true == bIsParsing && false == m_serverVersion.isEmpty())
    {
        ELGO_UPDATE_LOG("version info - {local: %s, server: %s}",
                        currVersion.toStdString().c_str(),
                        m_serverVersion.toStdString().c_str());

        // Compare Version
        const double currVersionNum = std::stod(currVersion.toStdString());
        const double serverVersionNum = std::stod(m_serverVersion.toStdString());
        if(currVersionNum < serverVersionNum)
        {
            QString startMsg = QString("현재: %1, 최신: %2 - 업데이트를 시작합니다.")
                                    .arg(currVersion)
                                    .arg(m_serverVersion);
            ui->label->setText(startMsg);
            bIsNeedNew = true;
            ELGO_UPDATE_LOG("Updating Start !");
        }
    }
    else
    {
        ui->label->setText("XML 파일 읽기 실패");
    }

    // Download New Binary
    if(true == bIsNeedNew)
    {
        const bool bIsClientDownload = StartNextDownload(ELGO_Client_URL, ELGO_Client_PATH);
        const bool bIsRemoteDownload = StartNextDownload(ELGO_Remote_URL, ELGO_Remote_PATH);
        ELGO_UPDATE_LOG("Download Results - {client: %d, remote: %d}",
                        bIsClientDownload, bIsRemoteDownload);
        if(true == bIsClientDownload && true == bIsRemoteDownload)
        {
            DecompressDownloadFile(ELGO_Client_PATH, ELGO_Client_DECMP_PATH);
            DecompressDownloadFile(ELGO_Remote_PATH, ELGO_Remote_DECMP_PATH);

            UpdateXmlVersion();
            ui->label->setText("업데이트 완료 - Signage를 시작합니다.");

            m_startTimer.setSingleShot(true);
            m_startTimer.start(START_TIMEOUT);
        }

        QFile::remove(ELGO_Client_PATH);
        QFile::remove(ELGO_Remote_PATH);
    }
    else
    {
        ui->label->setText("모든 프로그램이 최신 버전입니다.");

        m_startTimer.setSingleShot(true);
        m_startTimer.start(START_TIMEOUT);
    }
}

//========================================================
bool UpdateManager::StartNextDownload(const std::string url, const std::string savePath)
//========================================================
{
    bool retValue = false;

    CURL *curl = curl_easy_init();
    if(curl)
    {
        FILE *file = std::fopen(savePath.c_str(), "wb");

        char errorBuffer[CURL_ERROR_SIZE] = {'\0', };
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_TIMEOUT_MS, 10000);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, file);
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &WirteFileFunction);
        curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, errorBuffer);

        // for debug
        long resCode;
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &resCode);

        CURLcode curlCode = curl_easy_perform(curl);
        if(CURLE_OK == curlCode)
        {
            retValue = true;
            ELGO_UPDATE_LOG("Downloaded - %s", savePath.c_str());
        }
        else
        {
            QString errorStr = curl_easy_strerror(curlCode);
            ELGO_UPDATE_LOG("Error - %s : {%s}, {%s}", url.c_str(), errorStr.toUtf8().constData(), errorBuffer);
        }

        fclose(file);
    }
    curl_easy_cleanup(curl);

    return retValue;
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
        ELGO_UPDATE_LOG("ERROR - XML Open !");
        return retValue;
    }

    if(false == xmlDoc.setContent(&xmlFile))
    {
        ELGO_UPDATE_LOG("ERROR - setContent()");
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
                else
                {
                    ELGO_UPDATE_LOG("ERROR - root is NULL");
                }
            }
            else
            {
                ELGO_UPDATE_LOG("ERROR - 'version' is not existed");
            }
        }
        else
        {
            ELGO_UPDATE_LOG("ERROR - root is NULL");
        }
    }
    else
    {
        ELGO_UPDATE_LOG("ERROR - Not Existed 'configuration'");
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
    ELGO_UPDATE_LOG("Start Client");

    // ELGO_Main
    QProcess mainProcess;
    QStringList mainArgs;
    mainProcess.startDetached(ELGO_MAIN_PROC, mainArgs);
    mainProcess.waitForFinished();

    mainProcess.deleteLater();

    exit(0);
}

//========================================================
void UpdateManager::ReadyVersionRead()
//========================================================
{
    QString recvJson = m_getVersionReply->readAll();
    ELGO_UPDATE_LOG("recvJson: %s", recvJson.toStdString().c_str());

    // Parsing
    const bool bIsParsed = ParseLatestVersion(recvJson, m_serverVersion);
    if(true == bIsParsed)
    {
        ELGO_UPDATE_LOG("Server side - latest version: %s",
                        m_serverVersion.toStdString().c_str());
    }
    else
    {
        ELGO_UPDATE_LOG("Not Parsed Json - %s",
                        recvJson.toStdString().c_str());
    }
}

//========================================================
void UpdateManager::ReadVersionFinish()
//========================================================
{
    m_getVersionReply->deleteLater();

    CheckVersion();
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
    else
    {
        ELGO_UPDATE_LOG("Not Existed - version.object");
    }

    return retValue;
}

//========================================================
void UpdateManager::DecompressDownloadFile(const QString& path,
                                           const QString& destPath)
//========================================================
{
    QProcess *newProcess = new QProcess;

    QString cmd;
    QStringList args;

    QString os = QSysInfo::productType();
    ELGO_UPDATE_LOG("os: %s, path: %s",
                    os.toStdString().c_str(),
                    path.toStdString().c_str());

    if( (0 == strcmp(os.toStdString().c_str(), "linux")) ||
        (0 == strcmp(os.toStdString().c_str(), "ubuntu")) )
    {
        cmd = "tar";
        args << "-xvf";
        args << path;
        args << "-C";
        args << destPath;
    }
    ELGO_UPDATE_LOG("cmd: %s", cmd.toStdString().c_str());

    newProcess->start(cmd, args);
    newProcess->waitForFinished();

    qDebug() << newProcess->readAllStandardOutput();

    newProcess->deleteLater();
}

//========================================================
size_t UpdateManager::WirteFileFunction(void *ptr, size_t size, size_t nmemb, FILE *stream)
//========================================================
{
    size_t written = fwrite(ptr, size, nmemb, stream);

    return written;
}
