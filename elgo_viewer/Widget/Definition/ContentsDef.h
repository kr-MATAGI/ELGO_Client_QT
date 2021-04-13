#ifndef CONTENTSDEF_H
#define CONTENTSDEF_H

#define CAPATURE_SAVE_PATH  "/home/jaehoon/바탕화면/ELGO/screenCapture.png"

// QT
#include <QInternal>
#include <QString>
#include <QLabel>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>

// Common
#include "Common/Interface/ContentsPlayDataDef.h"

#define DEFAULT_FONT_SIZE   50;

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


namespace NewsInfo
{
    /** @brief */
    struct FeedData
    {
        FeedData()
            : bIsTextOverflow(false)
        {
        }
        ~FeedData()
        {
            delete originLabel;
            originLabel = NULL;

            delete changedLabel;
            changedLabel = NULL;
        }
        QLabel *originLabel;
        QLabel *changedLabel;
        bool bIsTextOverflow;
    };

    /** @brief */
    struct FeedAnimation
    {
        ~FeedAnimation()
        {
            delete originLabelAni;
            originLabelAni = NULL;

            delete changedLabelAni;
            changedLabelAni = NULL;

            delete overflowTextAni;
            overflowTextAni = NULL;

            delete parallAniGroup;
            parallAniGroup = NULL;
        }
        QPropertyAnimation *originLabelAni;
        QPropertyAnimation *changedLabelAni;
        QPropertyAnimation *overflowTextAni;
        QParallelAnimationGroup *parallAniGroup;
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


#endif // CONTENTSDEF_H
