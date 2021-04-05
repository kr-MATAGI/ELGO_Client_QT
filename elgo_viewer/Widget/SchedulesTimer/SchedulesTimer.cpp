
// Common
#include "Common/CommonDef.h"

// Viewer
#include "SchedulesTimer.h"

SchedulesTimer* SchedulesTimer::pInstance = nullptr;

//========================================================
SchedulesTimer::SchedulesTimer(QObject *parent)
    : QTimer(parent)
//========================================================
{
    // content play control (custom / fixed)
    m_singleTimer.start(990);

    // connect
    connect(this, SIGNAL(timeout()), this, SLOT(SchedulerTimeout()));
}

//========================================================
SchedulesTimer::~SchedulesTimer()
//========================================================
{
    m_singleTimer.stop();
}

//========================================================
SchedulesTimer* SchedulesTimer::GetInstance()
//========================================================
{
    if(nullptr == pInstance)
    {
        pInstance = new SchedulesTimer();
    }
    return pInstance;
}

//========================================================
void SchedulesTimer::DestoryInstance()
//========================================================
{
    if(nullptr != pInstance)
    {
        return;
    }
    delete pInstance;
    pInstance = NULL;
}

//========================================================
void SchedulesTimer::AddPlayDataList(const PlayJson::CustomPlayDataJson& src)
//========================================================
{
    m_singleTimer.AddPlayData(src);
}

//========================================================
void SchedulesTimer::AddPlayDataList(const PlayJson::FixedPlayDataJson& src)
//========================================================
{
    m_singleTimer.AddPlayData(src);
}

//========================================================
void SchedulesTimer::AddPlaySchedule(const ScheduleJson::PlaySchedules& src)
//========================================================
{
    m_playScheduleList.push_back(src);
}

//========================================================
void SchedulesTimer::ExecSinglePlayEvent(const PlayJson::PlayData& src)
//========================================================
{
    SchedulerDef::PlayDataInfo playDataInfo;
    playDataInfo.id = src.id;
    playDataInfo.type = src.playDataType;

    m_singleTimer.ExecPlayData(playDataInfo);
}

//========================================================
void SchedulesTimer::SchedulerTimeout()
//========================================================
{
    const QDateTime currentDateTime = QDateTime::currentDateTime();

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
                                ConvertDateTimeFormat(scheduleDataIter->startTime).toStdString().c_str(),
                                ConvertDateTimeFormat(scheduleDataIter->endTime).toStdString().c_str());

                playScheduleIter->scheduleList.erase(scheduleDataIter);

                // TODO : delete DB Control
            }
            else if(currSecEpoch >= scheduleDataIter->startTime.toSecsSinceEpoch() &&
                    currSecEpoch < scheduleDataIter->endTime.toSecsSinceEpoch())
            {
                // Check Cron format value (rule)
                const bool bIsValidCron = IsValidCronRuleValue(currentDateTime, scheduleDataIter->cron);
                if( (true == bIsValidCron) &&
                    (scheduleDataIter->playDataId != m_currPlayDataInfo.id) &&
                    (scheduleDataIter->type != m_currPlayDataInfo.type) )
                {
                    // Exec Schedule
                    ELGO_VIEWER_LOG("Exec Schedule {id : %d, type : %d}",
                                    scheduleDataIter->playDataId, scheduleDataIter->type);

                    ExecSchedule(playScheduleIter->id);
                    m_playScheduleList.push_back(*playScheduleIter);

                    return;
                }
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
void SchedulesTimer::ExecSchedule(const QString& scheduleId)
//========================================================
{

}

#if 0
//========================================================
void SchedulesTimer::UpdatePlayerScene(const PlayDataInfo& playDataInfo, const bool bIsSchedule)
//========================================================
{
    // Find New Scene
    QVector<SceneInfo>::iterator sceneIter = m_sceneList.begin();
    for(; sceneIter != m_sceneList.end(); ++sceneIter)
    {
        if( (playDataInfo.first == sceneIter->first.first) &&
            (playDataInfo.second == sceneIter->first.second) )
        {
            // Update scene
//            ContentsPlayer::GetInstance()->UpdatePlayerScene(*sceneIter->second);
            ELGO_VIEWER_LOG("Updated Scene - id : %d, type : %d", sceneIter->first.first, sceneIter->first.second);

            // PlayVideo
            QVector<VideoItemInfo>::const_iterator videoIter = m_videoItemList.constBegin();
            for(; videoIter != m_videoItemList.constEnd(); ++videoIter)
            {
                if( (playDataInfo.first == videoIter->first.first) &&
                    (playDataInfo.second == videoIter->first.second))
                {
                    videoIter->second->PlayVideoItem();
                }
            }

            break;
        }
    }

    // Prev Video Item Stop and Erase
    QVector<VideoItemInfo>::iterator prevVideoIter = m_videoItemList.begin();
    for(; prevVideoIter != m_videoItemList.end(); ++prevVideoIter)
    {
        if( (m_prevPlayData.first == prevVideoIter->first.first) &&
            (m_prevPlayData.second == prevVideoIter->first.second))
        {
            ELGO_VIEWER_LOG("Erase Image - id : %d, type : %d", prevVideoIter->first.first, prevVideoIter->first.second);

            prevVideoIter->second->StopVideoItem();

            delete prevVideoIter->second;
            prevVideoIter->second = NULL;

            m_videoItemList.erase(prevVideoIter);
        }
    }

    // Erase Image Item
    QVector<ImageItemInfo>::iterator imageIter = m_imageItemList.begin();
    for(; imageIter != m_imageItemList.end(); ++imageIter)
    {
        if( (m_prevPlayData.first == imageIter->first.first) &&
            (m_prevPlayData.second == imageIter->first.second) )

        {
            ELGO_VIEWER_LOG("Erase Image - id : %d, type : %d", imageIter->first.first, imageIter->first.second);

            delete imageIter->second;
            imageIter->second = NULL;

            m_imageItemList.erase(imageIter);
        }
    }

    // earse widget
    // earse subtitle
    // erase scene
    sceneIter = m_sceneList.begin();
    for(; sceneIter != m_sceneList.end(); ++sceneIter)
    {
        if( (m_prevPlayData.first == sceneIter->first.first) &&
            (m_prevPlayData.second == sceneIter->first.second) )
        {
            ELGO_VIEWER_LOG("Erase Scene - id : %d, type : %d", sceneIter->first.first, sceneIter->first.second);

            delete sceneIter->second;
            sceneIter->second = NULL;

            m_sceneList.erase(sceneIter);
        }
    }

    // Update value
    m_bIsSchedule = bIsSchedule;
    if(true == m_bIsSchedule)
    {
        m_prevSinglePlayData = m_currentPlayData;
        m_prevPlayData = m_currentPlayData;
        m_currentPlayData = playDataInfo;
    }
}
#endif

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
