#ifndef EVENTSTATE_H
#define EVENTSTATE_H

#include <QByteArray>
#include <QMap>

typedef void(*func)(QByteArray&);
class EventState
{
public:
    EventState();

    /** @brief */
    void ReigsterEvent(quint16 event, func linkedFunc);

    /** @brief */
    void Exec(quint16 event, QByteArray& src);

protected:
    QMap<quint16, func> m_eventList;
};

#endif // EVENTSTATE_H
