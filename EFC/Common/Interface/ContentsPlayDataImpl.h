#ifndef ELGODATASTREAM_H
#define ELGODATASTREAM_H

// QT
#include <QDataStream>

//
#include "ConetnsPlayDataDef.h"

namespace ObjectJsonDataStream
{
    /**
     *  @note   elgo_control <-> elgo_viewer, serialize function
     */

    /** @brief  Custom Play Data */
    QDataStream &operator<<(QDataStream& ds, const ObjectJson::CustomPlayDataJson& src);
    /** @brief  Fixed Play Data */
    QDataStream &operator<<(QDataStream& ds, const ObjectJson::FixedPlayDataJson& src);

    /** @@brief */
    QDataStream &operator<<(QDataStream& ds, const ObjectJson::PlayData& src);
    /** @@brief */
    QDataStream &operator<<(QDataStream& ds, const ObjectJson::PageData& src);
    /** @@brief */
    QDataStream &operator<<(QDataStream& ds, const ObjectJson::LayerData& src);
    /** @@brief */
    QDataStream &operator<<(QDataStream& ds, const ObjectJson::SubtitleData& src);
    /** @brief */
    QDataStream &operator<<(QDataStream& ds, const ObjectJson::LayerContent& src);
    /** @brief */
    QDataStream &operator<<(QDataStream& ds, const ObjectJson::LayerContentType& src);


    /**
     *  @note   elgo_control <-> elgo_viewer, deserialize function
     */

    /** @brief  Custom Play Data */
    QDataStream &operator>>(QDataStream& ds, ObjectJson::CustomPlayDataJson& dest);
    /** @brief  Fixed Play Data */
    QDataStream &operator>>(QDataStream& ds, ObjectJson::FixedPlayDataJson& dest);

    /** @brief */
    QDataStream &operator>>(QDataStream& ds, ObjectJson::PlayData& dest);
    /** @brief */
    QDataStream &operator>>(QDataStream& ds, ObjectJson::PageData& dest);
    /** @brief */
    QDataStream &operator>>(QDataStream& ds, ObjectJson::LayerData& dest);
    /** @brief */
    QDataStream &operator>>(QDataStream& ds, ObjectJson::SubtitleData& dest);
    /** @brief */
    QDataStream &operator>>(QDataStream& ds, ObjectJson::LayerContent& dest);
    /** @brief */
    QDataStream &operator>>(QDataStream& ds, ObjectJson::LayerContentType& dest);
}


#endif // ELGODATASTREAM_H
