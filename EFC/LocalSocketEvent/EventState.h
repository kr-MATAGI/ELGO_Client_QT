#ifndef EVENTSTATE_H
#define EVENTSTATE_H

#include <QByteArray>
#include <QDebug>
#include <QMap>
#include <unordered_map>

template <class T>
class EventState
{
public:
    EventState()
    {

    }

    /** @brief */
    void ReigsterEvent(quint16 event, std::function<void(T&, QByteArray&)> linkedFunc)
    {
        std::pair<quint16, std::function<void(T&, QByteArray&)>> eventCallBack(event, linkedFunc);
        m_eventList.insert(eventCallBack);
    }

    /** @brief */
    void Exec(quint16 event, QByteArray& src)
    {

    }

protected:
    std::unordered_map<quint16, std::function<void(T&, QByteArray&)>> m_eventList;
};

#endif // EVENTSTATE_H
