#ifndef CONTENTSDEF_H
#define CONTENTSDEF_H

#define CAPATURE_SAVE_PATH  "/home/jaehoon/바탕화면/ELGO/ELGO_ContentPlayer_Branch/screenCapture.png"

// QT
#include <QInternal>
#include <QString>
#include <QLabel>

// Common
#include "Common/Interface/ContentsPlayDataDef.h"

// Widget
#include "Widget/News/FeedBoxWidget.h"

namespace StyleSheet
{
    /** @brief */
    struct StyleInfo
    {
        StyleInfo()
            : bTransparency(false)
        {

        }
        QString backgroundColor;
        QString fontColor;
        quint32 fontSize;
        bool bTransparency;
    };

    /** @brief */
    struct PosSizeInfo
    {
        QPointF pos;
        QSize size;
    };
}

namespace WeatherInfo
{
    /** @brief */
    struct DisplayValue
    {
        DisplayValue()
            : pty(PlayJson::PTY::NO_RAIN)
            , sky(PlayJson::SKY::SUNNY)
            , lgt(false)
        {
        }

        PlayJson::PTY pty;
        PlayJson::SKY sky;
        bool lgt;
        QString metroCity;
        QString city;
        QString temperature;
    };
}

namespace VideoInfo
{
    /** @brief */
    struct Duration
    {
        Duration()
            : user(1)
            , file(1)
        {

        }
        qint64 user; // msec
        qint64 file; // msec
    };

    /** @brief */
    struct MetaData
    {
        QString fileName;
        Duration duration;
    };
}

namespace SubtitleInfo
{
    struct Animation
    {
        Animation()
            : bIsFixed(true)
            , direction(PlayJson::AniFlowDirection::NONE_DIRECTION)
            , orientation(PlayJson::Orientation::NONE_ORIENTATION)
            , action(PlayJson::SubtitleAction::NONE_ACTION)
            , speed(0)
        {
        }
        bool bIsFixed;
        PlayJson::AniFlowDirection direction;
        PlayJson::Orientation orientation;
        PlayJson::SubtitleAction action;
        int speed;
    };
}


#endif // CONTENTSDEF_H
