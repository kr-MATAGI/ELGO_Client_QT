
// Common
#include "LocalSocketEvent/EFCEvent.h"

// Main
#include "ScheduleTimer.h"
#include "Logger/MainLogger.h"

//========================================================
ScheduleTimer::ScheduleTimer(QObject *parent)
    : QTimer(parent)
//========================================================
{
    // connect
    connect(this, &QTimer::timeout,
            this, &ScheduleTimer::ScheduleTimeout);
}

//========================================================
ScheduleTimer::~ScheduleTimer()
//========================================================
{

}

//========================================================
void ScheduleTimer::AddPlayScheduleList(const QVector<ScheduleJson::PlaySchedule>& src)
//========================================================
{
    const int srcListSize = src.size();
    for(int idx = 0; idx < srcListSize; idx++)
    {
        const bool bIsValid = CheckValidPlayScheduleId(src[idx].id);
        if(true == bIsValid)
        {
            m_playScheduleList.push_back(src[idx]);
            ELGO_MAIN_LOG("Add New PlaySchedule - {id: %s}",
                          src[idx].id.toStdString().c_str());
        }
        else
        {
            ELGO_MAIN_LOG("Already Inserted PlaySchedule - {id: %s}",
                          src[idx].id.toStdString().c_str());
        }
    }

    // Update schedule.db
    MainController::GetInstance()->GetDBCtrl().UpdateNewPlaySchedule(m_playScheduleList);
}

//========================================================
bool ScheduleTimer::CheckValidPlayScheduleId(const QString& id)
//========================================================
{
    QVector<ScheduleJson::PlaySchedule>::const_iterator iter = m_playScheduleList.constBegin();
    while(iter != m_playScheduleList.constEnd())
    {
        const QString& iterId = (*iter).id;
        if(0 == strcmp(iterId.toStdString().c_str(), id.toStdString().c_str()))
        {
            return false;
        }
        ++iter;
    }

    return true;
}

//========================================================
void ScheduleTimer::ScheduleTimeout()
//========================================================
{

}
