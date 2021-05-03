// Common
#include "LocalSocketEvent/EFCEvent.h"

// Main
#include "PlayScheduleTimer.h"
#include "Logger/MainLogger.h"

//========================================================
PlayScheduleTimer::PlayScheduleTimer(QObject *parent)
    : QTimer(parent)
    , m_bIsActive(false)
//========================================================
{
    // connect
    connect(this, &QTimer::timeout,
            this, &PlayScheduleTimer::PlayScheduleTimeout);
}

//========================================================
PlayScheduleTimer::~PlayScheduleTimer()
//========================================================
{

}

//========================================================
void PlayScheduleTimer::AddPlayScheduleList(const QVector<ScheduleJson::PlaySchedule>& src)
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

    // Start Timer
    StartPlayTimer();
}

//========================================================
void PlayScheduleTimer::ClearAllPlayScheduleList()
//========================================================
{
    StopPlayTimer();

    MainController::GetInstance()->GetDBCtrl().ClearAllPlaySchedule();
    m_playScheduleList.clear();

    ELGO_MAIN_LOG("Clear All PlaySchedule List !");
}

//========================================================
void PlayScheduleTimer::ClearPlayScheduleById(const QString& id)
//========================================================
{
    StopPlayTimer();

    MainController::GetInstance()->GetDBCtrl().DeletePlayScheduleById(id);
    QVector<ScheduleJson::PlaySchedule>::iterator iter = m_playScheduleList.begin();
    while(iter != m_playScheduleList.end())
    {
        if(0 == strcmp(iter->id.toStdString().c_str(),
                       id.toStdString().c_str()))
        {
            ELGO_MAIN_LOG("Delete Schedule - id: %s",
                          iter->id.toStdString().c_str());
            iter = m_playScheduleList.erase(iter);
            break;
        }
        else
        {
            ++iter;
        }
    }

    if(0 < m_playScheduleList.size())
    {
        ELGO_MAIN_LOG("Re-Start PlaySchedule Timer !");
        StartPlayTimer();
    }
}

//========================================================
bool PlayScheduleTimer::CheckValidPlayScheduleId(const QString& id)
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
void PlayScheduleTimer::StartPlayTimer()
//========================================================
{
    if(false == m_bIsActive)
    {
        ELGO_MAIN_LOG("Stat Play Schedule Timer !");
        this->start(990);
    }
}

//========================================================
void PlayScheduleTimer::StopPlayTimer()
//========================================================
{
    if(true == m_bIsActive)
    {
        ELGO_MAIN_LOG("Stop Play Schedule Timer !");
        this->stop();
    }
}

//========================================================
bool PlayScheduleTimer::IsActivePlayTimer()
//========================================================
{
    return m_bIsActive;
}

//========================================================
void PlayScheduleTimer::PlayScheduleTimeout()
//========================================================
{
    if(0 < m_playScheduleList.size())
    {
        const QDateTime currDateTime = QDateTime::currentDateTime();
        const qint64 currSecEpoch = currDateTime.toSecsSinceEpoch();

        // Check Schedule
        QVector<ScheduleJson::PlaySchedule>::iterator scheduleIter = m_playScheduleList.begin();
        for(; scheduleIter != m_playScheduleList.end(); ++scheduleIter)
        {
            QVector<ScheduleJson::PlayScheduleData>::iterator dataIter = scheduleIter->scheduleList.begin();
            for(; dataIter != scheduleIter->scheduleList.end(); )
            {
                // Check Expired
                if(currSecEpoch >= dataIter->endTime.toSecsSinceEpoch())
                {
                    ELGO_MAIN_LOG("Expired - {id: %s, start: %s, end: %s}",
                                  scheduleIter->id.toStdString().c_str(),
                                  ConvertDateTimeToString(dataIter->startTime).toStdString().c_str(),
                                  ConvertDateTimeToString(dataIter->endTime).toStdString().c_str());

                    MainController::GetInstance()->GetDBCtrl().DeletePlayScheduleById(scheduleIter->id);
                    dataIter = scheduleIter->scheduleList.erase(dataIter);
                }
                else if( (currSecEpoch >= dataIter->startTime.toSecsSinceEpoch()) &&
                         (currSecEpoch < dataIter->endTime.toSecsSinceEpoch()) )
                {
                    const bool bIsValid = CheckValidDateTimeCron(currDateTime, dataIter->cron);
                    if( (true == bIsValid) &&
                        (0 != strcmp(m_currScheduleId.toStdString().c_str(),
                                     scheduleIter->id.toStdString().c_str())) )
                    {


                        return;
                    }
                }
                else
                {
                    ++dataIter;
                }
            }
        }
    }
}

//========================================================
bool PlayScheduleTimer::CheckValidDateTimeCron(const QDateTime& currentDateTime,
                                               const ScheduleJson::Cron& cron)
//========================================================
{
    bool retValue = false;

    const int month = currentDateTime.date().month();
    const int day = currentDateTime.date().day();
    const int dow = currentDateTime.date().dayOfWeek();

    const int hour = currentDateTime.time().hour();
    const int min = currentDateTime.time().minute();
    const int sec = currentDateTime.time().second();

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

    if(cron.secList.end() == std::find(cron.secList.begin(), cron.secList.end(), sec))
    {
        return retValue;
    }

    retValue = true;

    // TODO : Cron options - Maybe not using on web

    return retValue;
}

//========================================================
QString PlayScheduleTimer::ConvertDateTimeToString(const QDateTime& src)
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
