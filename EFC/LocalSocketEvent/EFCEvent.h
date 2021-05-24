#ifndef EFCEVENT_H
#define EFCEVENT_H

// QT
#include <QLocalSocket>

// EFC
#include "Common/Deifinition.h"
#include "Common/EventList.h"

class EFCEvent
{
public:
    /** @brief */
    static bool SendEvent(const ELGO_SYS::Proc proc, const quint16 event, const QByteArray &src);

private:
    /** @brief */
    static bool SendMessage(QLocalSocket* socket, const quint16 event, const QByteArray &src);
};

#endif // EFCEVENT_H
