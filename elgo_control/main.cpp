//QT
#include <QCoreApplication>
#include <QDataStream>

// EFC
#include "Common/Deifinition.h"
#include "Common/EventList.h"
#include "Event/ControlEventHandler.h"
#include "LocalSocketEvent/EFCEvent.h"

static ControlEventHandler *g_EventHandler = NULL;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    g_EventHandler = new ControlEventHandler(ELGO_PROC::Proc::ELGO_CONTROL);

    // Internet Connection
    // Function()

    // Control -> Main : ReadyStatus
    QByteArray readyBytes;
    QDataStream readyStream(&readyBytes, QIODevice::WriteOnly);
    readyStream.setVersion(QDataStream::Qt_5_12);
    readyStream << ELGO_PROC::Proc::ELGO_CONTROL;

    const bool bReadyEventy = EFCEvent::SendEvent(ELGO_PROC::Proc::ELGO_MAIN, MAIN_EVENT::PROCESS_IS_READY, readyBytes);
    if(false == bReadyEventy)
    {
        qDebug() << MAIN_EVENT::PROCESS_IS_READY << "Error SendEvent";
    }

    return a.exec();
}

