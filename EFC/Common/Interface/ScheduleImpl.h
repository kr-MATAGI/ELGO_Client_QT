#ifndef PLAYSCHEDULEIMPL_H
#define PLAYSCHEDULEIMPL_H

// QT
#include <QDataStream>

// Common
#include "ScheduleJsonDef.h"

/**
 *  @note   elgo_control <-> elgo_viewer, serialize function
 */

/** @brief  Play Schedules Serialize */
QDataStream& operator<<(QDataStream& ds, const ScheduleJson::PlaySchedule& src);
/** @brief */
QDataStream& operator<<(QDataStream& ds, const ScheduleJson::PlayScheduleData& src);
/** @brief */
QDataStream& operator<<(QDataStream& ds, const ScheduleJson::Cron& src);
/** @brief */
QDataStream& operator<<(QDataStream& ds, const ScheduleJson::CronOption& src);


/** @brief  Power Schedules Serialize */
QDataStream& operator<<(QDataStream& ds, const ScheduleJson::PowerSchedule& src);
/** @brief */
QDataStream& operator<<(QDataStream& ds, const ScheduleJson::PowerScheduleData& src);


/** @brief  Single Play Schdeuls Serialize */
QDataStream& operator<<(QDataStream& ds, const ScheduleJson::SinglePlaySchedules& src);
/** @brief */
QDataStream& operator<<(QDataStream& ds, const ScheduleJson::SinglePlayScheduleData& src);


/**
 *  @note   elgo_control <-> elgo_viewer, deserialize function
 */
/** @brief  Play Schedules Desrialize */
QDataStream& operator>>(QDataStream& ds, ScheduleJson::PlaySchedule& dest);
/** @brief */
QDataStream& operator>>(QDataStream& ds, ScheduleJson::PlayScheduleData& dest);
/** @brief */
QDataStream& operator>>(QDataStream& ds, ScheduleJson::Cron& dest);
/** @brief */
QDataStream& operator>>(QDataStream& ds, ScheduleJson::CronOption& dest);


/** @brief  Power Schedules Deserialize */
QDataStream& operator>>(QDataStream& ds, ScheduleJson::PowerSchedule& dest);
/** @brief */
QDataStream& operator>>(QDataStream& ds, ScheduleJson::PowerScheduleData& dest);


/** @brief  Single Play Schdeuls Deserialize */
QDataStream& operator>>(QDataStream& ds, ScheduleJson::SinglePlaySchedules& dest);
/** @brief */
QDataStream& operator>>(QDataStream& ds, ScheduleJson::SinglePlayScheduleData& dest);

#endif // PLAYSCHEDULEIMPL_H
