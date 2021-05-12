#include "UpdateWindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    UpdateManager w;

    w.show();

    w.CheckVersion();

    return a.exec();
}
