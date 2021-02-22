#include <QCoreApplication>

#include "TCPConnect/NetworkCtrl.h"
#include "JSON/JsonParser.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    NetworkCtrl net;

    return a.exec();
}
