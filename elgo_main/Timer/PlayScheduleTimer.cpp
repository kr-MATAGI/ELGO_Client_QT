// Common
#include "LocalSocketEvent/EFCEvent.h"
#include "Common/Interface/ContentsPlayDataImpl.h"

// Main
#include "PlayScheduleTimer.h"
#include "Logger/MainLogger.h"

//========================================================
PlayScheduleTimer::PlayScheduleTimer(QObject *parent)
    : QTimer(parent)
    , m_bIsActive(false)
//========================================================
{
    // Register meta type
    qRegisterMetaType<QVector<ScheduleJson::PlaySchedule>>("QVector<ScheduleJson::PlaySchedule>");

    // connect
    connect(this, &PlayScheduleTimer::AddPlayScheduleListSignal,
            this, &PlayScheduleTimer::AddPlayScheduleListSlot);

    connect(this, &PlayScheduleTimer::PlayScheduleTimerStartSignal,
            this, &PlayScheduleTimer::PlayScheduleTimerStartSlot);
    connect(this, &PlayScheduleTimer::PlayScheduleTimerStopSignal,
            this, &PlayScheduleTimer::PlayScheduleTimerStopSlot);

    connect(this, &QTimer::timeout,
            this, &PlayScheduleTimer::PlayScheduleTimeout);
}

//========================================================
PlayScheduleTimer::~PlayScheduleTimer()
//========================================================
{

}

//========================================================
void PlayScheduleTimer::AddPlayScheduleListSlot(const QVector<ScheduleJson::PlaySchedule>& src)
//========================================================
{
    AddPlayScheduleList(src);
}

//========================================================
void PlayScheduleTimer::AddPlayScheduleList(const QVector<ScheduleJson::PlaySchedule>& src)
//========================================================
{
    const int srcListSize = src.size();
    for(int idx = 0; idx < srcListSize; idx++)
    {
        const int pos = CheckDuplicatedScheduleId(src[idx].id);
        if(-1 == pos)
        {
            m_playScheduleList.push_back(src[idx]);
            ELGO_MAIN_LOG("Add New PlaySchedule - {id: %s}",
                          src[idx].id.toStdString().c_str());
        }
        else
        {
            ELGO_MAIN_LOG("[Replace] Already Inserted PlaySchedule - {pos: %d, id: %s}",
                          pos, src[idx].id.toStdString().c_str());
            m_playScheduleList[pos] = src[idx];
            m_currScheduleId.clear();
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

    m_playScheduleList.clear();

    ELGO_MAIN_LOG("Clear All PlaySchedule List !");
}

//========================================================
void PlayScheduleTimer::DeletePlayScheduleById(const QString& id)
//========================================================
{
    StopPlayTimer();

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
void PlayScheduleTimer::UpdatePlayingData(const int id, const PlayJson::PlayDataType type)
//========================================================
{
    m_playingData.first = id;
    m_playingData.second = type;
    ELGO_MAIN_LOG("Update Playing Data - {id: %d, type: %d}", id, type);
}

//========================================================
int PlayScheduleTimer::CheckDuplicatedScheduleId(const QString& id)
//========================================================
{
    int retValue = -1;

    QVector<ScheduleJson::PlaySchedule>::const_iterator iter = m_playScheduleList.constBegin();
    while(iter != m_playScheduleList.constEnd())
    {
        const QString& iterId = (*iter).id;
        if(0 == strcmp(iterId.toStdString().c_str(), id.toStdString().c_str()))
        {
            retValue = iter - m_playScheduleList.begin();
            break;
        }
        ++iter;
    }

    return retValue;
}

//========================================================
void PlayScheduleTimer::StartPlayTimer()
//========================================================
{
    if(false == m_bIsActive)
    {
        m_bIsActive = true;
        ELGO_MAIN_LOG("Start Play Schedule Timer !");
        this->start(990);
    }
}

//========================================================
void PlayScheduleTimer::StopPlayTimer()
//========================================================
{
    if(true == m_bIsActive)
    {
        m_bIsActive = false;
        ELGO_MAIN_LOG("Stop Play Schedule Timer !");
        this->stop();
    }
}

//========================================================
void PlayScheduleTimer::PlayScheduleTimerStartSlot()
//========================================================
{
    StartPlayTimer();
}

//========================================================
void PlayScheduleTimer::PlayScheduleTimerStopSlot()
//========================================================
{
    StopPlayTimer();
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
    if(true == m_playScheduleList.empty())
    {
        ELGO_MAIN_LOG("Schedule List is Empty() - Stop Timer");
        StopPlayTimer();

        return;
    }

    const QDateTime currDateTime = QDateTime::currentDateTime();
    const qint64 currSecEpoch = currDateTime.toSecsSinceEpoch();

    // Check Schedule
    bool bIsMatched = false;
    QVector<ScheduleJson::PlaySchedule>::iterator scheduleIter = m_playScheduleList.begin();
    for(; scheduleIter != m_playScheduleList.end(); ++scheduleIter)
    {
        QVector<ScheduleJson::PlayScheduleData>::iterator dataIter = scheduleIter->scheduleList.begin();
        for(; dataIter != scheduleIter->scheduleList.end(); )
        {
            // Check Expired
            if(currSecEpoch >= dataIter->endTime.toSecsSinceEpoch())
            {
                bIsMatched = false;

                ELGO_MAIN_LOG("Expired - {id: %s, start: %s, end: %s}",
                              scheduleIter->id.toStdString().c_str(),
                              ConvertDateTimeToString(dataIter->startTime).toStdString().c_str(),
                              ConvertDateTimeToString(dataIter->endTime).toStdString().c_str());

                MainController::GetInstance()->GetDBCtrl().DeletePlayScheduleById(scheduleIter->id);
                dataIter = scheduleIter->scheduleList.erase(dataIter);

                continue;
            }
            else if( (currSecEpoch >= dataIter->startTime.toSecsSinceEpoch()) &&
                     (currSecEpoch < dataIter->endTime.toSecsSinceEpoch()) )
            {
                bIsMatched = true;
                const bool bIsValid = CheckValidPlaySchedule(currDateTime, dataIter->cron);
                if( (true == bIsValid) && (m_playingData.first != dataIter->playDataId || m_playingData.second != dataIter->type) &&
                    (0 != strcmp(m_currScheduleId.toStdString().c_str(),
                                 scheduleIter->id.toStdString().c_str())) )
                {
                    if(true == m_currScheduleId.toStdString().empty())
                    {
                        m_prevSinglePlay = m_playingData;
                    }
                    m_currScheduleId = scheduleIter->id;

                    // Exec
                    if(PlayJson::PlayDataType::CUSTOM == dataIter->type)
                    {
                        PlayJson::CustomPlayDataJson customPlayData;

                        // Get CustomPlayData from DB
                        MainController::GetInstance()->GetDBCtrl().GetPlayDataFromDB(dataIter->playDataId,
                                                                                     dataIter->type,
                                                                                     customPlayData);

                        // Send playData to Viewer
                        /**
                         * @note
                         *       ELGO_CONTROL, ELGO_MAIN -> ELGO_VIEWER
                         *       Send custom play data information
                         * @param
                         *       CustomPlayDataJson customPlayData
                         */

                        QByteArray bytes;
                        QDataStream dataStream(&bytes, QIODevice::WriteOnly);
                        dataStream << customPlayData;

                        const bool bSendEvent = EFCEvent::SendEvent(ELGO_SYS::Proc::ELGO_VIEWER,
                                                                    VIEWER_EVENT::Event::PLAY_CUSTOM_PLAY_DATA,
                                                                    bytes);
                        if(false == bSendEvent)
                        {
                            ELGO_MAIN_LOG("ERROR - SendEvent: %d", VIEWER_EVENT::Event::PLAY_CUSTOM_PLAY_DATA);
                        }
                    }
                    else if(PlayJson::PlayDataType::FIXED == dataIter->type)
                    {
                        PlayJson::FixedPlayDataJson fixedPlayData;

                        // Get FixedPlayData from DB
                        MainController::GetInstance()->GetDBCtrl().GetPlayDataFromDB(dataIter->playDataId,
                                                                                     dataIter->type,
                                                                                     fixedPlayData);
                        // Send playData to Viewer
                        /**
                         * @note
                         *       ELGO_CONTROL, ELGO_MAIN -> ELGO_VIEWER
                         *       Send fixed play data information
                         * @param
                         *       FixedPlayDataJson fixedPlayData
                         */

                        QByteArray bytes;
                        QDataStream dataStream(&bytes, QIODevice::WriteOnly);
                        dataStream << fixedPlayData;

                        const bool bSendEvent = EFCEvent::SendEvent(ELGO_SYS::Proc::ELGO_VIEWER,
                                                                    VIEWER_EVENT::Event::PLAY_FIXED_PLAY_DATA,
                                                                    bytes);
                        if(false == bSendEvent)
                        {
                            ELGO_MAIN_LOG("ERROR - SendEvent: %d", VIEWER_EVENT::Event::PLAY_FIXED_PLAY_DATA);
                        }
                    }
                    else
                    {
                        ELGO_MAIN_LOG("ERROR - Unknown playDataType: {id: %d, type: %d}",
                                      dataIter->playDataId, dataIter->type);
                    }

                    return;
                }
            }

            ++dataIter;
        }
    }

    // Not Matched Play Schedule - Exec prev PlayData
    if(m_playingData != m_prevSinglePlay && false == bIsMatched)
    {
        if(PlayJson::PlayDataType::CUSTOM == m_prevSinglePlay.second)
        {
            PlayJson::CustomPlayDataJson customPlayData;

            // Get info from DB
            MainController::GetInstance()->GetDBCtrl().GetPlayDataFromDB(m_prevSinglePlay.first,
                                                                         m_prevSinglePlay.second,
                                                                         customPlayData);

            // Send Custom PlayData to Viwer
            QByteArray bytes;
            QDataStream dataStream(&bytes, QIODevice::WriteOnly);
            dataStream << customPlayData;

            const bool bSendEvent = EFCEvent::SendEvent(ELGO_SYS::Proc::ELGO_VIEWER,
                                                        VIEWER_EVENT::Event::PLAY_CUSTOM_PLAY_DATA,
                                                        bytes);
            if(false == bSendEvent)
            {
                ELGO_MAIN_LOG("ERROR - SendEvent: %d", bSendEvent);
            }
        }
        else if(PlayJson::PlayDataType::FIXED == m_prevSinglePlay.second)
        {
            PlayJson::FixedPlayDataJson fixedPlayData;

            // Get info from DB
            MainController::GetInstance()->GetDBCtrl().GetPlayDataFromDB(m_prevSinglePlay.first,
                                                                         m_prevSinglePlay.second,
                                                                         fixedPlayData);

            // Send Fixed PlayData to Viwer
            QByteArray bytes;
            QDataStream dataStream(&bytes, QIODevice::WriteOnly);
            dataStream << fixedPlayData;

            const bool bSendEvent = EFCEvent::SendEvent(ELGO_SYS::Proc::ELGO_VIEWER,
                                                        VIEWER_EVENT::Event::PLAY_FIXED_PLAY_DATA,
                                                        bytes);
            if(false == bSendEvent)
            {
                ELGO_MAIN_LOG("ERROR - SendEvent: %d", bSendEvent);
            }
        }
        else
        {
            ELGO_MAIN_LOG("ERROR - Unknown playDataType: {id: %d, type %d}",
                          m_prevSinglePlay.first, m_prevSinglePlay.second);
        }
    }
}

//========================================================
bool PlayScheduleTimer::CheckValidPlaySchedule(const QDateTime& currentDateTime,
                                               const ScheduleJson::Cron& cron)
//========================================================
{
    bool retValue = false;

    const int month = currentDateTime.date().month();
    const int day = currentDateTime.date().day();
    const int dow = currentDateTime.date().dayOfWeek();

    const int hour = currentDateTime.time().hour();
    const int min = currentDateTime.time().minute();

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
