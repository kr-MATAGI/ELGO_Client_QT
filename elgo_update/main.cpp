#include "UpdateWindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    UpdateManager updateManager;

    updateManager.show();

    updateManager.GetLatestVersion();
    updateManager.CheckVersion();

    return a.exec();
}
