#ifndef PLAYSCHEDULEIMPL_H
#define PLAYSCHEDULEIMPL_H

// QT
#include <QDataStream>

// Common
#include "ScheduleDef.h"

/**
 *  @note   elgo_control <-> elgo_viewer, serialize function
 */

/** @brief  Play Schedules Serialize */
QDataStream& operator<<(QDataStream& ds, const ScheduleJson::PlaySchedules& src);
/** @brief */
QDataStream& operator<<(QDataStream& ds, const ScheduleJson::PlayScheduleData& src);
/** @brief */
QDataStream& operator<<(QDataStream& ds, const ScheduleJson::Cron& src);
/** @brief */
QDataStream& operator<<(QDataStream& ds, const ScheduleJson::CronOption& src);


/** @brief  Power Schedules Serialize */
QDataStream& operator<<(QDataStream& ds, const ScheduleJson::PowerSchedules& src);
/** @brief */
QDataStream& operator<<(QDataStream& ds, const ScheduleJson::PowerScheduleData& src);


/** @brief  Single Play Schdeuls Serialize */
QDataStream& operator<<(QDataStream& ds, const ScheduleJson::SinglePlaySchedules& src);
/** @brief */
QDataStream& operator<<(QDataStream& ds, const ScheduleJson::SinglePlayData& src);


/**
 *  @note   elgo_control <-> elgo_viewer, deserialize function
 */
/** @brief  Play Schedules Desrialize */
QDataStream& operator>>(QDataStream& ds, ScheduleJson::PlaySchedules& dest);
/** @brief */
QDataStream& operator>>(QDataStream& ds, ScheduleJson::PlayScheduleData& dest);
/** @brief */
QDataStream& operator>>(QDataStream& ds, ScheduleJson::Cron& dest);
/** @brief */
QDataStream& operator>>(QDataStream& ds, ScheduleJson::CronOption& dest);


/** @brief  Power Schedules Deserialize */
QDataStream& operator>>(QDataStream& ds, ScheduleJson::PowerSchedules& dest);
/** @brief */
QDataStream& operator>>(QDataStream& ds, ScheduleJson::PowerScheduleData& dest);


/** @brief  Single Play Schdeuls Serialize */
QDataStream& operator>>(QDataStream& ds, ScheduleJson::SinglePlaySchedules dest);
/** @brief */
QDataStream& operator>>(QDataStream& ds, ScheduleJson::SinglePlayData dest);

#endif // PLAYSCHEDULEIMPL_H
