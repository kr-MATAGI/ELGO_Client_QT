// Common
#include "LocalSocketEvent/EFCEvent.h"

// Main
#include "PowerScheduleTimer.h"
#include "Logger/MainLogger.h"

//========================================================
PowerScheduleTimer::PowerScheduleTimer(QObject *parent)
    : QTimer(parent)
    , m_bIsActive(false)
//========================================================
{
    // connect
    connect(this, &QTimer::timeout,
            this, &PowerScheduleTimer::PowerScheduleTimeout);
}

//========================================================
PowerScheduleTimer::~PowerScheduleTimer()
//========================================================
{

}

//========================================================
void PowerScheduleTimer::AddPowerScheduleList(const QVector<ScheduleJson::PowerScheduleData>& src)
//========================================================
{
    int pos = -1;

    for(int idx = 0; idx < src.size(); idx++)
    {
        QVector<ScheduleJson::PowerScheduleData>::const_iterator iter = m_scheduleList.begin();
        while(iter != m_scheduleList.begin())
        {
            if( (0 == strcmp(iter->id.toStdString().c_str(), src[idx].id.toStdString().c_str())) &&
                (iter->status == src[idx].status) )
            {
                pos = iter - m_scheduleList.begin();
                break;
            }
        }

        if(-1 == pos)
        {
            m_scheduleList.push_back(src[idx]);
            ELGO_MAIN_LOG("Add Power Schedule - {id: %d, status: %d}",
                          src[idx].id.toStdString().c_str(), src[idx].status);
        }
        else
        {
            m_scheduleList[pos] = src[idx];
            ELGO_MAIN_LOG("Replace Power Schedule - {pos: %d, id: %d, status: %d}",
                          pos, src[idx].id.toStdString().c_str(), src[idx].status);
        }
    }

    StartPowerTimer();
}

//========================================================
void PowerScheduleTimer::DeletePowerScheduleById(const QString& id)
//========================================================
{
    StopPowerTimer();

    QVector<ScheduleJson::PowerScheduleData>::iterator iter = m_scheduleList.begin();
    while(iter != m_scheduleList.end())
    {
        if( 0 == strcmp(iter->id.toStdString().c_str(), id.toStdString().c_str()) )
        {
            ELGO_MAIN_LOG("Delete Schedule - {id: %s, status: %d",
                          iter->id.toStdString().c_str(), iter->status);
            iter = m_scheduleList.erase(iter);
        }
        else
        {
            ++iter;
        }
    }

    if(0 < m_scheduleList.size())
    {
        ELGO_MAIN_LOG("Re-Start PowerSchedule Timer !");
        StartPowerTimer();
    }
}

//========================================================
void PowerScheduleTimer::StartPowerTimer()
//========================================================
{
    if(false == m_bIsActive)
    {
        ELGO_MAIN_LOG("Start Power Schedule Timer !");
        m_bIsActive = true;
        this->start(990);
    }
}

//========================================================
void PowerScheduleTimer::StopPowerTimer()
//========================================================
{
    if(true == m_bIsActive)
    {
        ELGO_MAIN_LOG("Stop Power Schedule Timer !");
        m_bIsActive = false;
        this->stop();
    }
}

//========================================================
bool PowerScheduleTimer::IsActivePowerTimer()
//========================================================
{
    return m_bIsActive;
}

//========================================================
void PowerScheduleTimer::PowerScheduleTimeout()
//========================================================
{

}
