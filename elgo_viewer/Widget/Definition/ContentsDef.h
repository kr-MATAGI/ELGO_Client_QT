#ifndef CONTENTSDEF_H
#define CONTENTSDEF_H

#define CAPATURE_SAVE_PATH  "/home/jaehoon/바탕화면/ELGO/screenCapture.png"

// QT
#include <QInternal>
#include <QString>
#include <QLabel>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>


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


namespace ClockInfo
{
    /** @brief */
    enum Kinds
    {
        HOUR_12 = 1,
        HOUR_24 = 2
    };
}


namespace WeatherInfo
{
    /** @brief */
    enum Value
    {
        e_SUNNY = 1,
    };

    /** @brief */
    struct DisplayInfo
    {
        WeatherInfo::Value weather;
        QString city;
        QString status;
        int temperature;
    };
}


namespace NewsInfo
{
    /** @brief */
    enum Topic
    {
        e_NONE = 0,
        e_LATEST = 1,
        e_HEADLINE = 2,
        e_POLITICS = 3,
        e_ECONOMY = 4,
        e_SOCIETY = 5,
        e_SPORT = 6,
        e_CULTURE = 7,
        e_LOCAL = 8,
        e_GLOBAL = 9,
        e_WEATHER = 10
    };

    static const char* topicEnumToStr[] = {"NONE", "최신", "헤드라인",
                                          "정치", "경제", "사회",
                                          "스포츠", "문화연예", "지역",
                                          "세계", "날씨"};

    /** @brief */
    struct FeedData
    {
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

            delete parallAniGroup;
            parallAniGroup = NULL;
        }
        QPropertyAnimation *originLabelAni;
        QPropertyAnimation *changedLabelAni;
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
        qint64 user;
        qint64 file;
    };

    /** @brief */
    struct MetaData
    {
        QString fileName;
        Duration duration;
    };
}


#endif // CONTENTSDEF_H
