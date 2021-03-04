#pragma once
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

// QT
#include <QMainWindow>

// MainWindow
#include <QGraphicsScene>
#include <QSvgWidget>

// Viewer
#include "ViewerCtrl/ViewerController.h"

class ViewerController;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    static MainWindow *pInstance;
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public:
    /** @brief */
    static MainWindow* GetInstance();

    /** @brief */
    void DestoryInstance();

signals:
    void DrawQRCode();

private slots:
    void DrawQRCodeByThreadSignal();

private:
    Ui::MainWindow *ui;

    QGraphicsScene *m_logoScene;
    QSvgWidget *m_logoWidget;
};

#endif // MAINWINDOW_H
