#include "SchedulesTimer.h"

//========================================================
SchedulesTimer::SchedulesTimer(QObject *parent)
    : QTimer(parent)
    , m_bIsSinglePlay(false)
    , m_bIsExecSchedule(false)
//========================================================
{
    // init
    m_currentPlayData = std::make_pair(0, PlayJson::PlayDataType::NONE_PLAY_DATA_TYPE);
    m_prevPlayData = std::make_pair(0, PlayJson::PlayDataType::NONE_PLAY_DATA_TYPE);

    // connect
    connect(this, SIGNAL(timeout()), this, SLOT(CheckContentPlayerSchedules()));
}

//========================================================
SchedulesTimer::~SchedulesTimer()
//========================================================
{

}

//========================================================
void SchedulesTimer::AddPlayDataList(const PlayJson::FixedPlayDataJson& src,
                                     const bool bIsSinglePlay)
//========================================================
{
    m_bIsSinglePlay = bIsSinglePlay;
    if(true == m_bIsSinglePlay)
    {
        ClearAllSchedulesBySinglePlay();
    }

    if(m_fixedPlayDataList.end() == std::find(m_fixedPlayDataList.begin(),
                                              m_fixedPlayDataList.end(),
                                              src))
    {
        m_fixedPlayDataList.push_back(src);

        ELGO_VIEWER_LOG("ADD Play Data - {id : %d, type : %d}",
                        src.playData.id, src.playData.playDataType);
    }
}

//========================================================
void SchedulesTimer::AddPlayDataList(const PlayJson::CustomPlayDataJson& src,
                                     const bool bIsSinglePlay)
//========================================================
{
    m_bIsSinglePlay = bIsSinglePlay;
    if(true == m_bIsSinglePlay)
    {
        ClearAllSchedulesBySinglePlay();
    }

    if(m_customPlayDataList.end() == std::find(m_customPlayDataList.begin(),
                                               m_customPlayDataList.end(),
                                               src))
    {
        m_customPlayDataList.push_back(src);
        ELGO_VIEWER_LOG("ADD Play Data - {id : %d, type : %d}",
                        src.playData.id, src.playData.playDataType);
    }
}

//========================================================
void SchedulesTimer::AddPlaySchedule(const ScheduleJson::PlaySchedules& src)
//========================================================
{
    m_playScheduleList.push_back(src);
}

//========================================================
void SchedulesTimer::ClearAllSchedulesBySinglePlay()
//========================================================
{
    m_customPlayDataList.clear();
    m_fixedPlayDataList.clear();
    m_playScheduleList.clear();
}

//========================================================
void SchedulesTimer::CheckContentPlayerSchedules()
//========================================================
{
    QDateTime currentTime = QDateTime::currentDateTime();

    // Cron Format Schedules Check
    if(false == m_bIsSinglePlay)
    {
        CheckPlayDateTimeSchedules(currentTime);
    }
    else
    {
        // Single Play
        if(PlayJson::PlayDataType::CUSTOM == m_currentPlayData.second)
        {

        }
        else if(PlayJson::PlayDataType::FIXED == m_currentPlayData.second)
        {

        }
        else
        {
            ELGO_VIEWER_LOG("Error - current playDataType : NONE");
        }
    }
}

//========================================================
void SchedulesTimer::CheckPlayDateTimeSchedules(const QDateTime& currentDateTime)
//========================================================
{
    // Check start, end
    const qint64 currSecEpoch = currentDateTime.toSecsSinceEpoch();

    QVector<ScheduleJson::PlaySchedules>::iterator playScheduleIter = m_playScheduleList.begin();
    for(; playScheduleIter != m_playScheduleList.end(); ++playScheduleIter)
    {
        QVector<ScheduleJson::PlayScheduleData>::iterator scheduleDataIter = playScheduleIter->scheduleList.begin();
        for(; scheduleDataIter != playScheduleIter->scheduleList.end(); ++scheduleDataIter)
        {
            // check end dateTime
            if(currSecEpoch >= scheduleDataIter->endTime.toSecsSinceEpoch())
            {
                ELGO_VIEWER_LOG("Expired - id : %s, start : %s, end : %s",
                                playScheduleIter->id.toUtf8().constData(),
                                ConvertDateTimeFormat(scheduleDataIter->startTime).toUtf8().constData(),
                                ConvertDateTimeFormat(scheduleDataIter->endTime).toUtf8().constData());

                playScheduleIter->scheduleList.erase(scheduleDataIter);


                // TODO : DB Control
            }
            else if(currSecEpoch >= scheduleDataIter->startTime.toSecsSinceEpoch() &&
                    currSecEpoch < scheduleDataIter->endTime.toSecsSinceEpoch())
            {
                // Check Cron format value (rule)
                const bool bIsValidCron = IsValidCronRuleValue(currentDateTime, scheduleDataIter->cron);
                if(true == bIsValidCron && scheduleDataIter->playDataId != m_currentPlayData.first)
                {
                    // Exec Schedule
                    PlayDataInfo schedulePlayData(scheduleDataIter->playDataId, scheduleDataIter->type);
                    ExecSchedule(playScheduleIter->id, schedulePlayData);

                    return;
                }
            }
        }

        // shceduleData is empty
        if(true == playScheduleIter->scheduleList.empty())
        {
            ELGO_VIEWER_LOG("Empty - id : %s", playScheduleIter->id.toUtf8().constData());
            m_playScheduleList.erase(playScheduleIter);


            // TODO : DB Control
        }
    }

    // Not matched schedule
    ExecPlayData(m_prevPlayData);
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

    if(cron.monthRepeat.end() == std::find(cron.monthRepeat.begin(), cron.monthRepeat.end(), month))
    {
        return retValue;
    }

    if(cron.dayRepeat.end() == std::find(cron.dayRepeat.begin(), cron.dayRepeat.end(), day))
    {
        return retValue;
    }

    if(cron.dowRepeat.end() == std::find(cron.dowRepeat.begin(), cron.dowRepeat.end(), dow))
    {
        return retValue;
    }

    if(cron.hourRepeat.end() == std::find(cron.hourRepeat.begin(), cron.hourRepeat.end(), hour))
    {
        return retValue;
    }

    if(cron.minRepeat.end() == std::find(cron.minRepeat.begin(), cron.minRepeat.end(), min))
    {
        return retValue;
    }

    if(cron.secRepeat.end() == std::find(cron.secRepeat.begin(), cron.secRepeat.end(), sec))
    {
        return retValue;
    }

    retValue = true;

    // TODO : Cron options

    return retValue;
}

//========================================================
void SchedulesTimer::ExecSchedule(const QString& scheduleId, const PlayDataInfo& playDataInfo)
//========================================================
{
    const bool bIsPlaySuccess = ExecPlayData(playDataInfo);
    if(true == bIsPlaySuccess && false == m_bIsExecSchedule)
    {
        m_prevPlayData = m_currentPlayData;
        m_bIsExecSchedule = true;

        m_currentPlayData = playDataInfo;
        m_currentScheduleId = scheduleId;
    }

}

//========================================================
bool SchedulesTimer::ExecPlayData(const PlayDataInfo& playDataInfo)
//========================================================
{
    bool retValue = true;


    // exec








    if(m_prevPlayData == playDataInfo)
    {
        m_prevPlayData = PlayDataInfo();
    }

    return retValue;
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
