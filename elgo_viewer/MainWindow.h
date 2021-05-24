#pragma once
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

// QT
#include <QMainWindow>
#include <QTimer>

// MainWindow
#include <QGraphicsScene>
#include <QSvgWidget>

// Viewer
#include "ViewerCtrl/ViewerController.h"
#include "Widget/ContentsPlayer.h"

class ViewerController;
class ContentsPlayer;

namespace Ui {e
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    /** @brief */
    static MainWindow *pInstance;

    /** @brief */
    explicit MainWindow(QWidget *parent = nullptr);
    /** @brief */
    ~MainWindow();

public:
    /** @brief */
    static MainWindow* GetInstance();

    /** @brief */
    void DestoryInstance();

signals:
    /** @brief */
    void DrawQRCode(const QString& url, const QString& deviceName);

    /** @brief */
    void CloseMainWindowByError();

private slots:
    /** @brief */
    void DrawQRCodeSlot(const QString& url, const QString& deviceName);

    /** @brief */
    void CloseMainWindowByTimeout();

    /** @brief */
    void CloseMainWindowTimeout();

private:
    Ui::MainWindow *ui;
    QRect m_screenRect;

    QGraphicsScene *m_logoScene;
    QSvgWidget *m_logoWidget;
    QTimer m_closeTimer;

    QString m_qrUrl;
    bool m_isDrawStartQR;
};

#endif // MAINWINDOW_H
