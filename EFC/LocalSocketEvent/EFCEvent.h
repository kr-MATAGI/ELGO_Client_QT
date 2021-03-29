#ifndef EFCEVENT_H
#define EFCEVENT_H

// EFC
#include "Common/Deifinition.h"
#include "Common/EventList.h"

class EFCEvent
{
public:
    /** @brief */
    static bool SendEvent(ELGO_SYS::Proc proc, quint16 event, QByteArray &src);
};

#endif // EFCEVENT_H
