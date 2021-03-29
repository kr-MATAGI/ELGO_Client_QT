#ifndef PLAYSCHEDULEIMPL_H
#define PLAYSCHEDULEIMPL_H

// QT
#include <QDataStream>

// Common
#include "PlayScheduleDef.h"

/**
 *  @note   elgo_control <-> elgo_viewer, serialize function
 */

/** @brief  PlaySchedules Serialize */
QDataStream& operator<<(QDataStream& ds, const ScheduleJson::Schedules& src);

/** @brief */
QDataStream& operator<<(QDataStream& ds, const ScheduleJson::ScheduleData& src);
/** @brief */
QDataStream& operator<<(QDataStream& ds, const ScheduleJson::Cron& src);
/** @brief */
QDataStream& operator<<(QDataStream& ds, const ScheduleJson::CronOption& src);


/**
 *  @note   elgo_control <-> elgo_viewer, deserialize function
 */
/** @brief  PlaySchedules Desrialize */
QDataStream &operator>>(QDataStream& ds, ScheduleJson::Schedules& dest);

/** @brief */
QDataStream &operator>>(QDataStream& ds, ScheduleJson::ScheduleData& dest);
/** @brief */
QDataStream &operator>>(QDataStream& ds, ScheduleJson::Cron& dest);
/** @brief */
QDataStream &operator>>(QDataStream& ds, ScheduleJson::CronOption& dest);


#endif // PLAYSCHEDULEIMPL_H
