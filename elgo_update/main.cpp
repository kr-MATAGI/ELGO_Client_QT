#include "UpdateWindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    UpdateWindow w;

    w.show();

    w.CheckVersion();

    return a.exec();
}
