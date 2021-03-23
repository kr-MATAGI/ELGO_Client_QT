#ifndef NEWSFEEDWIDGET_H
#define NEWSFEEDWIDGET_H

#include <QWidget>

namespace Ui {
class NewsFeedWidget;
}

class NewsFeedWidget : public QWidget
{
    Q_OBJECT

public:
    explicit NewsFeedWidget(QWidget *parent = nullptr);
    ~NewsFeedWidget();

private:
    Ui::NewsFeedWidget *ui;
};

#endif // NEWSFEEDWIDGET_H
