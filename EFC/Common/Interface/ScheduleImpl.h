#ifndef PLAYSCHEDULEIMPL_H
#define PLAYSCHEDULEIMPL_H

// QT
#include <QDataStream>

// Common
#include "ScheduleDef.h"

/**
 *  @note   elgo_control <-> elgo_viewer, serialize function
 */

/** @brief  PlaySchedules Serialize */
QDataStream& operator<<(QDataStream& ds, const ScheduleJson::PlaySchedules& src);

/** @brief */
QDataStream& operator<<(QDataStream& ds, const ScheduleJson::PlayScheduleData& src);
/** @brief */
QDataStream& operator<<(QDataStream& ds, const ScheduleJson::Cron& src);
/** @brief */
QDataStream& operator<<(QDataStream& ds, const ScheduleJson::CronOption& src);


/** @brief  PowerSchedules Serialize */
QDataStream& operator<<(QDataStream& ds, const ScheduleJson::PowerSchedules& src);

/** @brief */
QDataStream& operator<<(QDataStream& ds, const ScheduleJson::PowerScheduleData& src);


/**
 *  @note   elgo_control <-> elgo_viewer, deserialize function
 */
/** @brief  PlaySchedules Desrialize */
QDataStream& operator>>(QDataStream& ds, ScheduleJson::PlaySchedules& dest);

/** @brief */
QDataStream& operator>>(QDataStream& ds, ScheduleJson::PlayScheduleData& dest);
/** @brief */
QDataStream& operator>>(QDataStream& ds, ScheduleJson::Cron& dest);
/** @brief */
QDataStream& operator>>(QDataStream& ds, ScheduleJson::CronOption& dest);



/** @brief  PowerSchedules Deserialize */
QDataStream& operator>>(QDataStream& ds, ScheduleJson::PowerSchedules& dest);

/** @brief */
QDataStream& operator>>(QDataStream& ds, ScheduleJson::PowerScheduleData& dest);


#endif // PLAYSCHEDULEIMPL_H
