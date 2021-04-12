#ifndef DOWNLOADDEF_H
#define DOWNLOADDEF_H

// QT
#include <QString>

#define NEWS_BASE_URL    "https://www.yonhapnewstv.co.kr/"
#define WEATHER_BASE_URL    "http://apis.data.go.kr/1360000/VilageFcstInfoService/getUltraSrtFcst?serviceKey="
#define WEATHER_KEY "0E%2Fz8rMxyn%2FK9t68RkJW2vlWcYJJN1M%2FF9Ai56KLbT2mEeDaZXSI24liwFcTuTKXM1RHJzmOA8Nwr5YWXpphhg%3D%3D"

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
        VIDEO_DOWNLOAD_TIME_OUT = 10 * 1000,
        NEWS_FEED = 5 * 1000
    };
}

#endif // DOWNLOADDEF_H
