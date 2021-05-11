// QT
#include <QScreen>
#include <QGraphicsDropShadowEffect>

// Common
#include "LocalSocketEvent/EFCEvent.h"

// MainWindow
#include "MainWindow.h"
#include "ui_MainWindow.h"

// Viewer
#include "Logger/ViewerLogger.h"
#include "QrCode/QrMaker.h"

MainWindow* MainWindow::pInstance = nullptr;

#define CLOSE_TIMEOUT   10000

//========================================================
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_isDrawStartQR(false)
//========================================================
{
    ui->setupUi(this);

    QList<QScreen *> screens = QApplication::screens();
    if(0 < screens.size())
    {
        QSize screenSize = screens[0]->size();
        const QRect screenRect = QRect(QPoint(0.0, 0.0), screenSize);

        m_screenRect = screenRect;
        ELGO_VIEWER_LOG("screenRect - {x: %d, y: %d}, {w: %d, y: %d}",
                        screenRect.x(), screenRect.y(),
                        screenRect.width(), screenRect.height());
    }

    // Window setting
    this->setWindowTitle("ELGO");
    this->setStyleSheet("background-color: rgb(245, 245, 245);");

    // ELGO Logo    
    m_logoScene = new QGraphicsScene(this);

    m_logoWidget = new QSvgWidget(QString(":/img/Images/logo.svg"));
    m_logoWidget->setAttribute(Qt::WA_TranslucentBackground, true);
    QRect logoGeometry(m_screenRect.width() * 0.45, m_screenRect.height() * 0.05,
                       m_screenRect.width() * 0.1, m_screenRect.width() * 0.1);
    m_logoWidget->setGeometry(logoGeometry);
    m_logoScene->setSceneRect(logoGeometry);
    ui->elgoLogo->setGeometry(logoGeometry);
    m_logoScene->addWidget(m_logoWidget);

    ui->elgoLogo->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->elgoLogo->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->elgoLogo->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    ui->elgoLogo->setRenderHint(QPainter::Antialiasing);
    ui->elgoLogo->setStyleSheet("border: 0px");
    ui->elgoLogo->setScene(m_logoScene);

    // Device Name Label
    QRect nameGeometry(m_screenRect.width() * 0.4, logoGeometry.bottom() * 1.1,
                       m_screenRect.width() * 0.2, m_screenRect.height() * 0.05);
    ui->nameLabel->setGeometry(nameGeometry);

    QFont nameFont;
    nameFont.setBold(true);
    nameFont.setPointSize(30);
    ui->nameLabel->setFont(nameFont);

    // QR Frame
    QRect qrFrameGeometry(m_screenRect.width() * 0.355, nameGeometry.bottom() * 1.1,
                          m_screenRect.width() * 0.3, m_screenRect.width() * 0.3);
    ui->qrFrame->setGeometry(qrFrameGeometry);
    ui->qrFrame->setStyleSheet("QFrame {background-color: rgb(255, 255, 255);"
                                "border-width: 1;"
                                "border-radius: 20;"
                                "border-style: solid;"
                                "border-color: gray}");
    ui->qrFrame->setFrameShadow(QFrame::Raised);
    ui->qrFrame->setLineWidth(3);
    ui->qrFrame->setMidLineWidth(3);

    QGraphicsDropShadowEffect *effect = new QGraphicsDropShadowEffect(this);
    effect->setBlurRadius(5);
    ui->qrFrame->setGraphicsEffect(effect);

    // QR Label
    QRect qrImgGeometry(m_screenRect.width() * 0.38, qrFrameGeometry.top() * 1.13,
                        m_screenRect.width() * 0.25, m_screenRect.width() * 0.25);
    ui->qrLabel->setStyleSheet("background-color: rgb(49, 42, 36);");
    ui->qrLabel->setGeometry(qrImgGeometry);

    // connect
    connect(this, &MainWindow::DrawQRCode,
            this, &MainWindow::DrawQRCodeSlot);
    connect(&m_closeTimer, SIGNAL(timeout()),
            this, SLOT(CloseMainWindowByTimeout()));
    connect(this, &MainWindow::CloseMainWindowByError,
            this, &MainWindow::CloseMainWindowTimeout);
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
void MainWindow::DrawQRCodeSlot(const QString& url, const QString& deviceName)
//========================================================
{
    m_qrUrl = url;

    const int width = ui->qrLabel->width();
    const int height = ui->qrLabel->height();

    QPixmap pixmap(width,height);
    pixmap.fill(Qt::white);
    QPainter painter(&pixmap);
    QColor color = Qt::black;

    QSize qrSize;
    qrSize.setWidth(width);
    qrSize.setHeight(height);

    QrMaker qrMaker;
    qrMaker.DrawQrCode(painter, qrSize, m_qrUrl, color);
    ELGO_VIEWER_LOG("QR code - {w: %d, h: %d}, url : %s", width, height,
                    m_qrUrl.toUtf8().constData());

    ui->qrLabel->setPixmap(pixmap);
    if(0 < deviceName.length())
    {
        ui->nameLabel->setText(deviceName);
    }

    if(true == m_isDrawStartQR)
    {
        this->showFullScreen();
    }

    // show Content Player after 10 sec
    m_closeTimer.start(CLOSE_TIMEOUT);
}

//========================================================
void MainWindow::CloseMainWindowByTimeout()
//========================================================
{
    if(false == m_isDrawStartQR)
    {
        m_isDrawStartQR = true;
    }

    m_closeTimer.stop();
    this->close();
}

//========================================================
void MainWindow::CloseMainWindowTimeout()
//========================================================
{
    m_closeTimer.start(CLOSE_TIMEOUT);
}
