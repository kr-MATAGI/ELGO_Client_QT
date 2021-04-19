
// Common
#include "Common/CommonDef.h"

// Viewer
#include "SchedulesTimer.h"

//========================================================
SchedulesTimer::SchedulesTimer(QObject *parent)
    : QTimer(parent)
    , m_bIsStartedTimer(false)
//========================================================
{
    // connect
    connect(this, SIGNAL(timeout()), this, SLOT(SchedulerTimeout()));
}

//========================================================
SchedulesTimer::~SchedulesTimer()
//========================================================
{
    this->stop();
}

//========================================================
void SchedulesTimer::AddPlaySchedule(const QVector<ScheduleJson::PlaySchedule>& src)
//========================================================
{
    const int scheduleListSize = src.size();
    for(int idx = 0; idx < scheduleListSize; idx++)
    {
        ELGO_VIEWER_LOG("ADD Schedule - id: %s", src[idx].id.toStdString().c_str());
        m_playScheduleList.push_back(src[idx]);
    }

    if(false == m_bIsStartedTimer)
    {
        ELGO_VIEWER_LOG("Start Schedule Timer (900 msec)");
        this->start(900);
    }
}

//========================================================
void SchedulesTimer::ClearPlaySchedule()
//========================================================
{
    this->stop();

    const int prevScheduleListSize = m_playScheduleList.size();
    ELGO_VIEWER_LOG("Prev Schedule List Size : %d, Current Schedule id : %s, palyData{id: %d, type: %d}",
                    prevScheduleListSize, m_currScheduleId.toStdString().c_str(),
                    m_currPlayDataInfo.id, m_currPlayDataInfo.type);

    foreach(auto item, m_playScheduleList)
    {
        ELGO_VIEWER_LOG("Ready Delete Schedule - id: %s", m_currScheduleId.toStdString().c_str());
    }

    m_currScheduleId.clear();
    m_playScheduleList.clear();
    m_currPlayDataInfo = ScheduleTimer::PlayDataInfo();
    m_prevSinglePlayDataInfo = ScheduleTimer::PlayDataInfo();
}

//========================================================
void SchedulesTimer::SchedulerTimeout()
//========================================================
{
    const QDateTime currentDateTime = QDateTime::currentDateTime();

    // Check start, end
    const qint64 currSecEpoch = currentDateTime.toSecsSinceEpoch();

    QVector<ScheduleJson::PlaySchedule>::iterator playScheduleIter = m_playScheduleList.begin();
    for(; playScheduleIter != m_playScheduleList.end(); ++playScheduleIter)
    {
        QVector<ScheduleJson::PlayScheduleData>::iterator scheduleDataIter = playScheduleIter->scheduleList.begin();
        for(; scheduleDataIter != playScheduleIter->scheduleList.end(); )
        {
            // check end dateTime
            if(currSecEpoch >= scheduleDataIter->endTime.toSecsSinceEpoch())
            {
                ELGO_VIEWER_LOG("Expired - id : %s, start : %s, end : %s",
                                playScheduleIter->id.toUtf8().constData(),
                                ConvertDateTimeFormat(scheduleDataIter->startTime).toStdString().c_str(),
                                ConvertDateTimeFormat(scheduleDataIter->endTime).toStdString().c_str());

                scheduleDataIter = playScheduleIter->scheduleList.erase(scheduleDataIter);

                // TODO : delete DB Control
            }
            else if( (currSecEpoch >= scheduleDataIter->startTime.toSecsSinceEpoch()) &&
                     (currSecEpoch < scheduleDataIter->endTime.toSecsSinceEpoch()) )
            {
                // Check Cron format value (rule)
                const bool bIsValidCron = IsValidCronRuleValue(currentDateTime, scheduleDataIter->cron);
                if( (true == bIsValidCron) &&
                    (scheduleDataIter->playDataId != m_currPlayDataInfo.id) &&
                    (scheduleDataIter->type != m_currPlayDataInfo.type) )
                {
                    // Exec Schedule
                    ScheduleTimer::PlayDataInfo nextPlayData;
                    nextPlayData.id = scheduleDataIter->playDataId;
                    nextPlayData.type = scheduleDataIter->type;
                    ELGO_VIEWER_LOG("Exec Schedule {id : %d, type : %d}",
                                    nextPlayData.id, nextPlayData.type);

                    ExecSchedule(playScheduleIter->id, nextPlayData);
                    m_playScheduleList.push_back(*playScheduleIter);

                    return;
                }
            }
            else
            {
                ++scheduleDataIter;
            }
        }

        // shceduleData is empty - delete schedule
        if(true == playScheduleIter->scheduleList.empty())
        {
            ELGO_VIEWER_LOG("Empty - id : %s", playScheduleIter->id.toUtf8().constData());
            m_playScheduleList.erase(playScheduleIter);

            // TODO : DB Control
        }
    }
}

//========================================================
bool SchedulesTimer::IsValidCronRuleValue(const QDateTime& currentDateTime, const ScheduleJson::Cron& cron)
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
void SchedulesTimer::ExecSchedule(const QString& scheduleId, const ScheduleTimer::PlayDataInfo& playDataInfo)
//========================================================
{
    if( m_currPlayDataInfo != playDataInfo)
    {
        PlayJson::PlayData playData;
        playData.id = playData.id;
        playData.playDataType = playData.playDataType;
        emit ContentsPlayer::GetInstance()->ExecPlayDataSignal(playData);

        m_currScheduleId = scheduleId;
        m_currPlayDataInfo = playDataInfo;
    }
}

//========================================================
QString SchedulesTimer::ConvertDateTimeFormat(const QDateTime &src)
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
