#ifndef DOWNLOADDEF_H
#define DOWNLOADDEF_H

// QT
#include <QString>

namespace DownloadDef
{
    /** @brief */
    enum Action
    {
        NONE_KIND = 0,
        SINGLE_PLAY = 1,
        PLAY_SCHEDULES = 2,
    };

    /** @brief */
    enum CURL_TIMEOUT
    {
        DEFAULT_TIME_OUT = 1 * 1000,
        VIDEO_DOWNLOAD_TIME_OUT = 5 * 1000,
        NEWS_FEED = 4 * 1000
    };

    // News Feed
    /** @brief */
    enum NewsCategory
    {
        LATEST = 1,
        HEAD_LINE = 2,
        POLITICS = 3,
        ECONOMY =4,
        SOCIETY = 5,
        LOCAL = 6,
        INTERNAL = 7,
        CULTURE = 8,
        SPORTS = 9,
        WEATHER = 10
    };

    static const char* categoryEnumToStr[] = { "latest", "headline", "politics",
                                               "economy", "society", "local",
                                                "international", "culture", "sports", "weather"};

    // Weather Info
    namespace Weather
    {
        // Reqeust
        /** @brief */
        struct Request
        {
            int nx;
            int ny;
            QString baseDate; // yyyymmdd
            QString baseTime; // hhmm
        };

        // Response
        /** @brief */
        enum SKY
        {
            SUNNY = 1,
            CLOUDY = 2,
            FOG = 3
        };

        /** @brief */
        enum PTY
        {
            NO_RAIN = 0,
            RAIN = 1,
            SLEET = 2,
            SNOW = 3,
            SHOWER = 4,
            RAIN_DROP = 5,
            RAIN_DROP_SNOW = 6,
            SNOW_DRIFT = 7
        };

        /**
         *  @note
         *          pty - precipitation form
         *          sky - sky condition
         *          t1h - temperature
         *          rn1 - rainfall (1 hour)
         *          reh - humidity
         *          vec - direction of the wind
         *          wsd - wind speed
         */
        struct Response
        {
            Response()
                : bIsSetPty(false)
                , bIsSetSky(false)
                , bIsSetT1h(false)
                , bIsSetRn1(false)
                , bIsSetReh(false)
                , bIsSetVec(false)
                , bIsSetWsd(false)
            {

            }

            PTY pty;
            SKY sky;
            QString t1h;
            int rn1;
            int reh;
            QString vec;
            QString wsd;

            bool bIsSetPty;
            bool bIsSetSky;
            bool bIsSetT1h;
            bool bIsSetRn1;
            bool bIsSetReh;
            bool bIsSetVec;
            bool bIsSetWsd;


            /** @brief  PTY */
            void SetPty(const PTY value)
            {
                pty = value;
                bIsSetPty = true;
            }
            bool IsSetPty()
            {
                return bIsSetPty;
            }

            /** @brief  SKY */
            void SetSky(const SKY value)
            {
                sky = value;
                bIsSetSky = true;
            }
            bool IsSetSky()
            {
                return bIsSetSky;
            }

            /** @brief  T1H */
            void SetT1h(const QString value)
            {
                t1h = value;
                bIsSetT1h = true;
            }
            bool IsSetT1h()
            {
                return bIsSetT1h;
            }

            /** @brief  RN1 */
            void SetRn1(const int value)
            {
                rn1 = value;
                bIsSetRn1 = true;
            }
            bool IsSetRn1()
            {
                return bIsSetRn1;
            }

            /** @brief  REH */
            void SetReh(const int value)
            {
                reh = value;
                bIsSetReh = true;
            }
            bool IsSetReh()
            {
                return bIsSetReh;
            }

            /** @brief  VEC */
            void SetVec(const QString value)
            {
                vec = value;
                bIsSetVec = true;
            }
            bool IsSetVec()
            {
                return bIsSetVec;
            }

            /** @brief  WSD */
            void SetWsd(const QString value)
            {
                wsd = value;
                bIsSetWsd = true;
            }
            bool IsSetWsd()
            {
                return bIsSetWsd;
            }

        };
    }
}

#endif // DOWNLOADDEF_H
