#ifndef VIEWEREVENTSTATE_H
#define VIEWEREVENTSTATE_H

#include <QByteArray>
#include <QMap>

#include "LocalSocketEvent/EventState.h"

typedef void(*func)(QByteArray&);

class ViewerEventState : public EventState
{
public:
    ViewerEventState();
};


#endif // VIEWEREVENTSTATE_H
