// Common
#include "LocalSocketEvent/EFCEvent.h"

// Main
#include "PowerScheduleTimer.h"
#include "Logger/MainLogger.h"
#include "Utils/DeviceManager.h"

//========================================================
PowerScheduleTimer::PowerScheduleTimer(QObject *parent)
    : QTimer(parent)
    , m_bIsActive(false)
//========================================================
{
    m_powerInfo.second = ScheduleJson::PowerStatus::POWER_NONE;

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
            ELGO_MAIN_LOG("Add Power Schedule - {id: %s, status: %d}",
                          src[idx].id.toStdString().c_str(), src[idx].status);
        }
        else
        {
            m_scheduleList[pos] = src[idx];
            ELGO_MAIN_LOG("Replace Power Schedule - {pos: %d, id: %s, status: %d}",
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
    if(true == m_scheduleList.empty())
    {
        ELGO_MAIN_LOG("Schedule List is Empty() - Stop Timer");
        StopPowerTimer();

        return;
    }

    const QDateTime currDateTime = QDateTime::currentDateTime();
    const qint64 currSecEpoch = currDateTime.toSecsSinceEpoch();

    // Check Schedule
    QVector<ScheduleJson::PowerScheduleData>::iterator dataIter = m_scheduleList.begin();
    while(dataIter != m_scheduleList.end())
    {
        // Check Expired
        if(currSecEpoch >= dataIter->endTime.toSecsSinceEpoch())
        {
            ELGO_MAIN_LOG("Expired - {id: %s, start: %s, end: %s}",
                          dataIter->id.toStdString().c_str(),
                          ConvertDateTimeToString(dataIter->startTime).toStdString().c_str(),
                          ConvertDateTimeToString(dataIter->endTime).toStdString().c_str());

            MainController::GetInstance()->GetDBCtrl().DeletePowerScheduleById(dataIter->id);
            dataIter = m_scheduleList.erase(dataIter);

            continue;
        }
        else if( (currSecEpoch >= dataIter->startTime.toSecsSinceEpoch()) &&
                 (currSecEpoch < dataIter->endTime.toSecsSinceEpoch()) )
        {
            const bool bIsValid = CheckValidPowerSchedule(currDateTime, dataIter->cron);
            if( (true == bIsValid) &&
                ( 0 != strcmp(m_powerInfo.first.toStdString().c_str(),
                              dataIter->id.toStdString().c_str()) &&
                  m_powerInfo.second != dataIter->status) )
            {
                m_powerInfo = POWER_INFO(dataIter->id, dataIter->status);

                // EXEC
                DEVICE::OS os = MainController::GetInstance()->GetMainCtrl().GetDeviceInfo().os;
                const bool bIsSleep = (ScheduleJson::PowerStatus::POWER_OFF == dataIter->status) ? true : false;
                DeviceManager::UpdateSleepStatus(os, bIsSleep);

                return;
            }
        }

        ++ dataIter;
    }
}

//========================================================
QString PowerScheduleTimer::ConvertDateTimeToString(const QDateTime& src)
//========================================================
{
    // hh:mm::ss.msec
    QString timeStr = src.time().toString();
    timeStr.append(".");

    //date
    QDate date = src.date();
    QString year = QString::number(date.year());
    QString month = QString::number(date.month());
    QString day = QString::number(date.day());

    // yyyy-mm-dd
    QString dateStr;
    dateStr.append(year);
    dateStr.append("-");
    dateStr.append(month);
    dateStr.append("-");
    dateStr.append(day);

    QString dateTimeStr = dateStr + ":" + timeStr;
    return dateTimeStr;
}

//========================================================
bool PowerScheduleTimer::CheckValidPowerSchedule(const QDateTime& currDateTime,
                                                 const ScheduleJson::Cron& cron)
//========================================================
{
    bool retValue = false;

    const int month = currDateTime.date().month();
    const int day = currDateTime.date().day();
    const int dow = currDateTime.date().dayOfWeek();

    const int hour = currDateTime.time().hour();
    const int min = currDateTime.time().minute();

    if(cron.monthList.end() == std::find(cron.monthList.begin(), cron.monthList.end(), month))
    {
        return retValue;
    }

    if(cron.dayList.end() == std::find(cron.dayList.begin(), cron.dayList.end(), day))
    {
        return retValue;
    }

    if(cron.dowList.end() == std::find(cron.dowList.begin(), cron.dowList.end(), dow))
    {
        return retValue;
    }

    if(cron.hourList.end() == std::find(cron.hourList.begin(), cron.hourList.end(), hour))
    {
        return retValue;
    }

    if(cron.minList.end() == std::find(cron.minList.begin(), cron.minList.end(), min))
    {
        return retValue;
    }

    retValue = true;
    return retValue;
}
