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

}

//========================================================
void PowerScheduleTimer::PowerScheduleTimeout()
//========================================================
{

}
