// QT
#include <QApplication>
#include <QQuickView>
#include <QGuiApplication>

// EFC
#include "LocalSocketEvent/EFCEvent.h"

// Viewer
#include "MainWindow.h"
#include "Event/ViewerEventHandler.h"
#include "ViewerCtrl/ViewerCtrl.h"
#include "ViewerCtrl/ViewerController.h"
#include "Widget/ContentsPlayer.h"

static ViewerController *g_ViewerController = ViewerController::GetInstance();
static ContentsPlayer *g_ContentsPlayer = NULL;
static MainWindow *g_MainWindow = NULL;
static ViewerEventHandler *g_EventHandler = NULL;

#include "Widget/News/NewsFeedWidget.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    g_MainWindow = MainWindow::GetInstance();
//    g_MainWindow->setStyleSheet("background-color: rgb(255, 255, 0);");

    // Event Listenner
    g_EventHandler = new ViewerEventHandler(ELGO_PROC::Proc::ELGO_VIEWER);

    // Send proccess ready status to MAIN
    // TODO : If socket is not opened, occured App crush.
    g_ViewerController->GetInstance()->GetViewerCtrl().SendViewerIsReady();

    // Contetns Player Init
    g_ContentsPlayer = ContentsPlayer::GetInstance();

    // Display Main Window
//    g_MainWindow->showFullScreen();
    g_MainWindow->show();


    // test
    StyleSheet::StyleInfo styleInfo;
    styleInfo.backgroundColor = "#333333";
    styleInfo.fontColor = "#7FFFD4";
    styleInfo.bTransparency = true;

    NewsFeedWidget newsWidget;
    QList<QString> newsList;
    newsList << "뉴스 피드 1번 입니다.";
    newsList << "뉴스 피드 2번 입니다.";
    newsList << "뉴스 피드 3번 입니다.";
    newsList << "뉴스 피드 4번 입니다.";
    newsList << "뉴스 피드 5번 입니다.";
    newsList << "뉴스 피드 6번 입니다.";
    newsList << "뉴스 피드 7번 입니다.";
    newsList << "뉴스 피드 8번 입니다.";
    newsList << "뉴스 피드 9번 입니다.";
    newsList << "뉴스 피드 10번 입니다.";


    newsWidget.SetNewsFeedList(News::Topic::e_LOCAL, newsList);
    newsWidget.MakeNewsFeedWidget();
    newsWidget.show();

    return app.exec();
}
