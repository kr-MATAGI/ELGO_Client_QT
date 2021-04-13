// QT
#include <QLayout>

// MainWindow
#include "MainWindow.h"
#include "ui_MainWindow.h"

// Viewer
#include "Logger/ViewerLogger.h"
#include "QrCode/QrMaker.h"

MainWindow* MainWindow::pInstance = nullptr;

#define CLOSE_TIMEOUT   5000

//========================================================
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
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
    connect(this, SIGNAL(DrawQRCode()), this, SLOT(DrawQRCodeByThreadSignal()));
    connect(&m_closeTimer, SIGNAL(timeout()), this, SLOT(CloseMainWindowByTimeout()));
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
void MainWindow::DrawQRCodeByThreadSignal()
//========================================================
{
    const int width = MainWindow::GetInstance()->width() / 4;
    const int height = MainWindow::GetInstance()->height() / 4;

    QPixmap pixmap(width,height);
    pixmap.fill(Qt::white);
    QPainter painter(&pixmap);
    QColor color = Qt::black;
    QString url = ViewerController::GetInstance()->GetViewerCtrl().GetQRCodeURL();
    QSize qrSize(0, 0);
    qrSize.setWidth(width);
    qrSize.setHeight(height);

    QrMaker qrMaker;
    qrMaker.DrawQrCode(painter, qrSize, url, color);
    ELGO_VIEWER_LOG("QR code - size : %d x %d, url : %s", width, height,url.toUtf8().constData());

    ui->qrLabel->setPixmap(pixmap);

    // show Content Player after 10 sec
    m_closeTimer.start(CLOSE_TIMEOUT);

    ContentsPlayer::GetInstance()->StartScheduleTimer();
}

//========================================================
void MainWindow::CloseMainWindowByTimeout()
//========================================================
{
    ContentsPlayer::GetInstance()->StartContentsPlayer();
    m_closeTimer.stop();
    this->close();
}
