#ifndef EVENTSTATE_H
#define EVENTSTATE_H

#include <QByteArray>
#include <QDebug>
#include <QMap>
#include <unordered_map>
#include <iostream>

template <class T>
class EventState
{
public:
    /** @brief */
    EventState()
    {

    }

    //========================================================
    void RegisterEvent(quint16 keyEvent, std::function<void(T&, const QByteArray&)> linkedFunc)
    //========================================================
    {
        std::pair<quint16, std::function<void(T&, const QByteArray&)>> eventCallBack(keyEvent, linkedFunc);
        m_eventList.insert(eventCallBack);
    }

    //========================================================
    void Exec(const quint16 keyEvent, const QByteArray& src)
    //========================================================
    {
        if(m_eventList.end() == m_eventList.find(keyEvent))
        {
            qDebug() << keyEvent << "is not enrolld";
        }
        else
        {
            T templateClass;
            std::function<void(T&, const QByteArray&)> linkedFunc = m_eventList.at(keyEvent);
            linkedFunc(templateClass, src);
        }
    }

protected:
    std::unordered_map<quint16, std::function<void(T&, const QByteArray&)>> m_eventList;
};

#endif // EVENTSTATE_H
