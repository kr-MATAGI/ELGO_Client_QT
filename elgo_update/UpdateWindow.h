#ifndef UPDATEWINDOW_H
#define UPDATEWINDOW_H

// QT
#include <QMainWindow>
#include <QNetworkAccessManager>

QT_BEGIN_NAMESPACE
namespace Ui { class UpdateWindow; }
QT_END_NAMESPACE

class UpdateWindow : public QMainWindow
{
    Q_OBJECT

public:
    /** @brief */
    UpdateWindow(QWidget *parent = nullptr);
    /** @brief */
    ~UpdateWindow();

    /** @brief */
    void CheckVersion();

private:
    Ui::UpdateWindow *ui;

    QRect m_screenRect;
    QNetworkAccessManager *m_manager;
};
#endif // UPDATEWINDOW_H
