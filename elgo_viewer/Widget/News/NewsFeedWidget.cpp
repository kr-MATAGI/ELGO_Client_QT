#include "NewsFeedWidget.h"
#include "ui_NewsFeedWidget.h"

NewsFeedWidget::NewsFeedWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::NewsFeedWidget)
{
    ui->setupUi(this);
}

NewsFeedWidget::~NewsFeedWidget()
{
    delete ui;
}
