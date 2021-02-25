#include <QCoreApplication>

#include "Common/Deifinition.h"
#include "Common/EventList.h"
#include "TCPConnect/NetworkCtrl.h"
#include "JSON/JsonParser.h"

#include "Event/ControlEventHandler.h"

static ControlEventHandler *g_EventHandler = NULL;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    g_EventHandler = new ControlEventHandler(ELGO_PROC::Proc::ELGO_CONTROL);

    QByteArray ar;

    qDebug() << VIEWER_EVENT::Event::MAX_VIEWER_EVENT;

    return a.exec();
}

