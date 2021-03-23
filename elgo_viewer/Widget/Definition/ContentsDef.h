#ifndef CONTENTSDEF_H
#define CONTENTSDEF_H

#define CAPATURE_SAVE_PATH  "/home/jaehoon/바탕화면/ELGO/"

// QT
#include <QInternal>
#include <QString>
#include <QLabel>

namespace StyleSheet
{
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
}

namespace Clock
{
    enum Kinds
    {
        HOUR_12 = 1,
        HOUR_24 = 2
    };
}

namespace Weather
{
    enum Value
    {
        e_SUNNY = 1,
    };

    struct DisplayInfo
    {
        Weather::Value weather;
        QString city;
        QString status;
        int temperature;
    };
}

namespace News
{
    struct FeedData
    {
        QLabel displayLabel;
        QLabel AnimationLabel;
        bool bIsTextOverflow;
    };
}


#endif // CONTENTSDEF_H
