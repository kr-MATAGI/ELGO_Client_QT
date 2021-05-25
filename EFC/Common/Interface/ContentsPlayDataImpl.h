#ifndef ELGODATASTREAM_H
#define ELGODATASTREAM_H

// QT
#include <QDataStream>

//
#include "ContentsPlayDataDef.h"


/**
 *  @note   elgo_control <-> elgo_viewer, serialize function
 */

/** @brief  Custom Play Data */
QDataStream &operator<<(QDataStream& ds, const PlayJson::CustomPlayDataJson& src);
/** @brief  Fixed Play Data */
QDataStream &operator<<(QDataStream& ds, const PlayJson::FixedPlayDataJson& src);

/** @@brief */
QDataStream &operator<<(QDataStream& ds, const PlayJson::PlayData& src);
/** @@brief */
QDataStream &operator<<(QDataStream& ds, const PlayJson::PageData& src);
/** @@brief */
QDataStream &operator<<(QDataStream& ds, const PlayJson::CustomLayerData& src);
/** @@brief */
QDataStream &operator<<(QDataStream& ds, const PlayJson::FixedLayerData& src);
/** @@brief */
QDataStream &operator<<(QDataStream& ds, const PlayJson::SubtitleData& src);
/** @brief */
QDataStream &operator<<(QDataStream& ds, const PlayJson::ContentData& src);
/** @brief */
QDataStream &operator<<(QDataStream& ds, const PlayJson::ContentInfo& src);

/** @brief */
QDataStream &operator<<(QDataStream& ds, const PlayJson::UpdateWidgetInfo& src);

/**
 *  @note   elgo_control <-> elgo_viewer, deserialize function
 */

/** @brief  Custom Play Data */
QDataStream &operator>>(QDataStream& ds, PlayJson::CustomPlayDataJson& dest);
/** @brief  Fixed Play Data */
QDataStream &operator>>(QDataStream& ds, PlayJson::FixedPlayDataJson& dest);

/** @brief */
QDataStream &operator>>(QDataStream& ds, PlayJson::PlayData& dest);
/** @brief */
QDataStream &operator>>(QDataStream& ds, PlayJson::PageData& dest);
/** @brief */
QDataStream &operator>>(QDataStream& ds, PlayJson::CustomLayerData& dest);
/** @brief */
QDataStream &operator>>(QDataStream& ds, PlayJson::FixedLayerData& dest);
/** @brief */
QDataStream &operator>>(QDataStream& ds, PlayJson::SubtitleData& dest);
/** @brief */
QDataStream &operator>>(QDataStream& ds, PlayJson::ContentData& dest);
/** @brief */
QDataStream &operator>>(QDataStream& ds, PlayJson::ContentInfo& dest);

/** @brief */
QDataStream &operator>>(QDataStream& ds, PlayJson::UpdateWidgetInfo& dest);



#endif // ELGODATASTREAM_H
