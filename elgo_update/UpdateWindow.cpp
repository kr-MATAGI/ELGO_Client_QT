// QT
#include <QScreen>

// Update
#include "UpdateWindow.h"
#include "ui_UpdateWindow.h"

#define ELGO_UPDATE_URL "http://cloud.elgo.co.kr/update/ubuntu"

//========================================================
UpdateWindow::UpdateWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::UpdateWindow)
//========================================================
{
    ui->setupUi(this);

    // init
    this->setWindowTitle("업데이트 확인");

    QList<QScreen *> screens = QApplication::screens();
    if(0 < screens.size())
    {
        QSize screenSize = screens[0]->size();
        const QRect screenRect = QRect(QPoint(0.0, 0.0), screenSize);

        m_screenRect = screenRect;
    }

    const QRect& windowRect = this->frameGeometry();
    const QPoint& center = m_screenRect.center();
    this->move(center.x() - windowRect.width() / 2, center.y() - windowRect.height() / 2);

    QFont labelFont;
    labelFont.setPointSize(15);
    ui->label->setFont(labelFont);
}

//========================================================
UpdateWindow::~UpdateWindow()
//========================================================
{
    delete ui;

    m_manager->deleteLater();
}

//========================================================
void UpdateWindow::CheckVersion()
//========================================================
{
    m_manager = new QNetworkAccessManager(this);

    QUrl updateUrl(ELGO_UPDATE_URL);
    QNetworkRequest request(updateUrl);



}
