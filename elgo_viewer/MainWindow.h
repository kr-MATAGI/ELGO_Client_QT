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
#include "Widget/SchedulesTimer/SchedulesTimer.h"

class ViewerController;
class ContentsPlayer;
class SchedulesTimer;

namespace Ui {
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
    void DrawQRCode();

private slots:
    /** @brief */
    void DrawQRCodeByThread();

    /** @brief */
    void CloseMainWindowByTimeout();


private:
    Ui::MainWindow *ui;

    QGraphicsScene *m_logoScene;
    QSvgWidget *m_logoWidget;
    QTimer m_closeTimer;
};

#endif // MAINWINDOW_H
