#include <QCoreApplication>

#include "Common/Deifinition.h"
#include "TCPConnect/NetworkCtrl.h"
#include "JSON/JsonParser.h"

#include "LocalSocketEvent/LocalSocketClient.h"
#include "LocalSocketEvent/LocalSocketServer.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    return a.exec();
}

