#ifndef DOWNLOADDEF_H
#define DOWNLOADDEF_H

// QT
#include <QString>

namespace DownloadDef
{
    /** @brief */
    enum CURL_TIMEOUT
    {
        WEATHER_INFO = 1 * 1000,
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
            QString baseTime; // hhmmss
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
         *          pop - a rainfall probability
         *          pty - precipitation form
         *          reh - humidity
         *          sky - sky condition
         *          tmn - lowest temperature
         *          tmx - highest temperature
         *          vec - direction of the wind
         *          wsd - wind speed
         */
        struct Response
        {
            Response()
                : bIsSetPop(false)
                , bIsSetPty(false)
                , bIsSetReh(false)
                , bIsSetSky(false)
                , bIsSetTmn(false)
                , bIsSetTmx(false)
                , bIsSetVec(false)
                , bIsSetWsd(false)
            {

            }

            int pop;
            PTY pty;
            int reh;
            SKY sky;
            QString tmn;
            QString tmx;
            QString vec;
            QString wsd;

            bool bIsSetPop;
            bool bIsSetPty;
            bool bIsSetReh;
            bool bIsSetSky;
            bool bIsSetTmn;
            bool bIsSetTmx;
            bool bIsSetVec;
            bool bIsSetWsd;

            /** @brief  POP */
            void SetPop(const int value)
            {
                pop = value;
                bIsSetPop = true;
            }
            bool IsSetPop()
            {
                return bIsSetPop;
            }

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

            /** @brief  TMN */
            void SetTmn(const QString value)
            {
                tmn = value;
                bIsSetTmn = true;
            }
            bool IsSetTmn()
            {
                return bIsSetTmn;
            }

            /** @brief  TMX */
            void SetTmx(const QString value)
            {
                tmx = value;
                bIsSetTmx = true;
            }
            bool IsSetTmx()
            {
                return bIsSetTmx;
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
