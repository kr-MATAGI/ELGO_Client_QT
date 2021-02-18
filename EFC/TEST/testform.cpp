#include <QPixmap>
#include <QPainter>

#include "testform.h"
#include "ui_testform.h"


void testForm::QrMakerTest(QPaintEvent *event)
{
    Q_UNUSED(event)

    // qr maker
    QPainter painter(this);

    QString url = "http://www.naver.com";
    ::QrMaker::DrawQrCode(painter, QSize(400, 400), url, QColor("balck"));

    painter.end();
}

testForm::testForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::testForm)
{
    ui->setupUi(this);

    QrMakerTest(nullptr);

}

testForm::~testForm()
{
    delete ui;
}
