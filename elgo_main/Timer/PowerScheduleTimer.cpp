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



    StartPowerTimer();
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
