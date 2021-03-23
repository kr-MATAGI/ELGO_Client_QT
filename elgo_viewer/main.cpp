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

#include "Widget/Weather/WeatherWidget.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    g_MainWindow = MainWindow::GetInstance();
//    g_MainWindow->setStyleSheet("background-color: rgb(255, 255, 0);");

    // Event Listenner
    g_EventHandler = new ViewerEventHandler(ELGO_PROC::Proc::ELGO_VIEWER);

    // Send proccess ready status to MAIN
    // TODO : IF socket is not opened, occured App crush.
    g_ViewerController->GetInstance()->GetViewerCtrl().SendViewerIsReady();

    // Contetns Player Init
    g_ContentsPlayer = ContentsPlayer::GetInstance();

    // Display Main Window
//    g_MainWindow->showFullScreen();
    g_MainWindow->show();

    WeatherWidget dateWidget;
    StyleSheet::StyleInfo styleInfo;
    styleInfo.backgroundColor = "#333333";
    styleInfo.fontColor = "#7FFFD4";
    styleInfo.bTransparency = true;

    Weather::DisplayInfo displayinfo;
    displayinfo.city = "부산";
    displayinfo.status = "맑음";
    displayinfo.temperature = 16;
    dateWidget.SetWeatherInfo(displayinfo);
    dateWidget.SetStyleSheet(styleInfo);
    dateWidget.show();

    return app.exec();
}
