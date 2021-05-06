// QT
#include <QLayout>

// Common
#include "LocalSocketEvent/EFCEvent.h"

// MainWindow
#include "MainWindow.h"
#include "ui_MainWindow.h"

// Viewer
#include "Logger/ViewerLogger.h"
#include "QrCode/QrMaker.h"

MainWindow* MainWindow::pInstance = nullptr;

#define CLOSE_TIMEOUT   5000

//========================================================
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_isDrawStartQR(false)
//========================================================
{
    ui->setupUi(this);

    // Window setting
    this->setWindowTitle("ELGO");
    this->setStyleSheet("background-color: rgb(255, 255, 0);");
    ui->verticalLayout->setAlignment(Qt::AlignCenter);

    // ELGO Logo
    m_logoScene = new QGraphicsScene;
    m_logoWidget = new QSvgWidget(QString(":/img/Images/logo.svg"));
    m_logoWidget->setAttribute(Qt::WA_TranslucentBackground, true);
    m_logoScene->addWidget(m_logoWidget);
    ui->elgoLogo->setScene(m_logoScene);

    // connect
    connect(this, &MainWindow::DrawQRCode,
            this, &MainWindow::DrawQRCodeByThread);
    connect(&m_closeTimer, SIGNAL(timeout()),
            this, SLOT(CloseMainWindowByTimeout()));
}

//========================================================
MainWindow::~MainWindow()
//========================================================
{
    delete ui;
    delete m_logoScene;
    m_logoScene = NULL;
}

//========================================================
MainWindow* MainWindow::GetInstance()
//========================================================
{
    if(nullptr == pInstance)
    {
        pInstance = new MainWindow();
    }
    return pInstance;
}

//========================================================
void MainWindow::DestoryInstance()
//========================================================
{
    if(nullptr != pInstance)
    {
        return;
    }
    delete pInstance;
    pInstance = NULL;
}

//========================================================
void MainWindow::DrawQRCodeByThread(QString url)
//========================================================
{
    m_qrUrl = url;

    const int width = MainWindow::GetInstance()->width() / 4;
    const int height = MainWindow::GetInstance()->height() / 4;

    QPixmap pixmap(width,height);
    pixmap.fill(Qt::white);
    QPainter painter(&pixmap);
    QColor color = Qt::black;

    QSize qrSize(0, 0);
    qrSize.setWidth(width);
    qrSize.setHeight(height);

    QrMaker qrMaker;
    qrMaker.DrawQrCode(painter, qrSize, m_qrUrl, color);
    ELGO_VIEWER_LOG("QR code - size : %d x %d, url : %s", width, height,
                    m_qrUrl.toUtf8().constData());

    ui->qrLabel->setPixmap(pixmap);
    this->show();

    // show Content Player after 10 sec
    m_closeTimer.start(CLOSE_TIMEOUT);
}

//========================================================
void MainWindow::CloseMainWindowByTimeout()
//========================================================
{
    if(false == m_isDrawStartQR)
    {
        emit ContentsPlayer::GetInstance()->StartContentsPlayerSignal();
        m_isDrawStartQR = true;

        /**
         *  @note
         *          ELGO_VIEWER -> ELGO_MAIN
         *          Request data for offline single play
         *  @param
         *          NONE
         */
        QByteArray bytes;
        const bool bSendEvent = EFCEvent::SendEvent(ELGO_SYS::Proc::ELGO_MAIN,
                                                    MAIN_EVENT::Event::REQUEST_OFFLINE_SINGLE_PLAY,
                                                    bytes);
        if(false == bSendEvent)
        {
            ELGO_VIEWER_LOG("ERROR - Send Event: %d", MAIN_EVENT::Event::REQUEST_OFFLINE_SINGLE_PLAY)
        }
    }

    m_closeTimer.stop();
    this->close();
}
