#include "ContentsPlayDataImpl.h"



//========================================================
QDataStream &operator<<(QDataStream& ds, const PlayJson::CustomPlayDataJson &src)
//========================================================
{
    ds << src.playData;

    const int pageDataListSize = src.pageDataList.size();
    ds << pageDataListSize;

    for(int idx = 0; idx < pageDataListSize; idx++)
    {
        ds << src.pageDataList[idx];
    }

    return ds;
}

//========================================================
QDataStream &operator<<(QDataStream& ds, const PlayJson::FixedPlayDataJson &src)
//========================================================
{
    ds << src.playData;

    const int layerDataListSize = src.layerDataList.size();
    ds << layerDataListSize;

    for(int idx = 0; idx < layerDataListSize; idx++)
    {
        ds << src.layerDataList[idx];
    }

    const int subtitleDataListSize = src.subtitleDataList.size();
    ds << subtitleDataListSize;

    for(int idx = 0; idx < subtitleDataListSize; idx++)
    {
        ds << src.subtitleDataList[idx];
    }

    return ds;
}

//========================================================
QDataStream &operator<<(QDataStream& ds, const PlayJson::PlayData &src)
//========================================================
{
    ds << src.id;
    ds << src.name;
    ds << src.memo;
    ds << src.width;
    ds << src.height;
    ds << src.orientation;
    ds << src.playDataType;

    return ds;
}

//========================================================
QDataStream &operator<<(QDataStream& ds, const PlayJson::PageData& src)
//========================================================
{
    ds << src.duration;

    const int layerDataListSize = src.layerDataList.size();
    ds << layerDataListSize;

    for(int idx = 0; idx < layerDataListSize; idx++)
    {
        ds << src.layerDataList[idx];
    }

    const int subtitleDataListSize = src.subtitleDataList.size();
    ds << subtitleDataListSize;

    for(int idx = 0; idx < subtitleDataListSize; idx++)
    {
        ds << src.subtitleDataList[idx];
    }

    return ds;
}

//========================================================
QDataStream &operator<<(QDataStream& ds, const PlayJson::CustomLayerData& src)
//========================================================
{
    ds << src.left;
    ds << src.top;
    ds << src.width;
    ds << src.height;
    ds << src.layerContent;

    return ds;
}

//========================================================
QDataStream &operator<<(QDataStream& ds, const PlayJson::FixedLayerData& src)
//========================================================
{
    ds << src.left;
    ds << src.top;
    ds << src.width;
    ds << src.height;

    const int contentDataListSize = src.contentDataList.size();
    for(int idx = 0; idx < contentDataListSize; idx++)
    {
        ds << src.contentDataList[idx];
    }
}

//========================================================
QDataStream &operator<<(QDataStream& ds, const PlayJson::ContentData& src)
//========================================================
{
    ds << src.contentType;
    ds << src.name;
    ds << src.fileDuration;
    ds << src.userDuration;

    // weather
    ds << src.metropolCity;
    ds << src.metropolCityName;
    ds << src.city;
    ds << src.cityName;
    ds << src.fontColor;
    ds << src.backgroundColor;
    ds << src.bBackgroundOpacity;
    ds << src.nx;
    ds << src.ny;

    // clock
    ds << src.hourType;

    // date
    ds << src.dateType;

    // news
    ds << src.newsCategory;
    ds << src.newsfontSize;
    ds << src.newsBoxCount;
    ds << src.newsBoxColor;
    ds << src.bNewsBoxOpacity;

    return ds;
}

//========================================================
QDataStream &operator<<(QDataStream& ds, const PlayJson::LayerContentType& src)
//========================================================
{
    ds << src.contentType;
    ds << src.mediaType;

    return ds;
}

//========================================================
QDataStream &operator<<(QDataStream& ds, const PlayJson::SubtitleData& src)
//========================================================
{
    ds << src.left;
    ds << src.top;
    ds << src.width;
    ds << src.height;
    ds << src.text;
    ds << src.bIsFixed;
    ds << src.direction;
    ds << src.orientation;
    ds << src.action;
    ds << src.speed;
    ds << src.backgroundColor;
    ds << src.fontColor;
    ds << src.fontSize;

    return ds;
}


//========================================================
QDataStream &operator>>(QDataStream& ds, PlayJson::CustomPlayDataJson& dest)
//========================================================
{
    ds >> dest.playData;

    int pageDataListSize = 0;
    ds >> pageDataListSize;

    for(int idx = 0; idx < pageDataListSize; idx++)
    {
        PlayJson::PageData pageData;
        ds >> pageData;
        dest.pageDataList.push_back(pageData);
    }

    return ds;
}

//========================================================
QDataStream &operator>>(QDataStream& ds, PlayJson::FixedPlayDataJson& dest)
//========================================================
{
    ds >> dest.playData;

    int layDataListSize = 0;
    ds >> layDataListSize;

    for(int idx = 0; idx < layDataListSize; idx++)
    {
        PlayJson::FixedLayerData layerData;
        ds >> layerData;
        dest.layerDataList.push_back(layerData);
    }

    int subtitleDataListSize = 0;
    ds >> subtitleDataListSize;

    for(int idx = 0; idx < subtitleDataListSize; idx++)
    {
        PlayJson::SubtitleData subtitleData;
        ds >> subtitleData;
        dest.subtitleDataList.push_back(subtitleData);
    }

    return ds;
}

//========================================================
QDataStream &operator>>(QDataStream& ds, PlayJson::PlayData& dest)
//========================================================
{
    ds >> dest.id;
    ds >> dest.name;
    ds >> dest.memo;
    ds >> dest.width;
    ds >> dest.height;
    ds >> dest.orientation;
    ds >> dest.playDataType;

    return ds;
}

//========================================================
QDataStream &operator>>(QDataStream& ds, PlayJson::PageData& dest)
//========================================================
{
    ds >> dest.duration;

    int layDataListSize = 0;
    ds >> layDataListSize;

    for(int idx = 0; idx < layDataListSize; idx++)
    {
        PlayJson::CustomLayerData layerData;
        ds >> layerData;
        dest.layerDataList.push_back(layerData);
    }

    int subtitleDataListSize = 0;
    ds >> subtitleDataListSize;

    for(int idx = 0; idx < subtitleDataListSize; idx++)
    {
        PlayJson::SubtitleData subtitleData;
        ds >> subtitleData;
        dest.subtitleDataList.push_back(subtitleData);
    }

    return ds;
}

//========================================================
QDataStream &operator>>(QDataStream& ds, PlayJson::CustomLayerData& dest)
//========================================================
{
    ds >> dest.left;
    ds >> dest.top;
    ds >> dest.width;
    ds >> dest.height;
    ds >> dest.layerContent;

    return ds;
}

//========================================================
QDataStream &operator>>(QDataStream& ds, PlayJson::FixedLayerData& dest)
//========================================================
{
    ds >> dest.left;
    ds >> dest.top;
    ds >> dest.width;
    ds >> dest.height;

    int contentDataListSize = 0;
    ds >> contentDataListSize;
    for(int idx = 0; idx < contentDataListSize; idx++)
    {
        PlayJson::ContentData contentData;
        ds >> contentData;
        dest.contentDataList.push_back(contentData);
    }

    return ds;
}

//========================================================
QDataStream &operator>>(QDataStream& ds, PlayJson::ContentData& dest)
//========================================================
{
    ds >> dest.contentType;
    ds >> dest.name;
    ds >> dest.fileDuration;
    ds >> dest.userDuration;

    // weather
    ds >> dest.metropolCity;
    ds >> dest.metropolCityName;
    ds >> dest.city;
    ds >> dest.cityName;
    ds >> dest.fontColor;
    ds >> dest.backgroundColor;
    ds >> dest.bBackgroundOpacity;
    ds >> dest.nx;
    ds >> dest.ny;

    // clock
    ds >> dest.hourType;

    // date
    ds >> dest.dateType;

    // news
    ds >> dest.newsCategory;
    ds >> dest.newsfontSize;
    ds >> dest.newsBoxCount;
    ds >> dest.newsBoxColor;
    ds >> dest.bNewsBoxOpacity;

    return ds;
}

//========================================================
QDataStream &operator>>(QDataStream& ds, PlayJson::SubtitleData& dest)
//========================================================
{
    ds >> dest.left;
    ds >> dest.top;
    ds >> dest.width;
    ds >> dest.height;
    ds >> dest.text;
    ds >> dest.bIsFixed;
    ds >> dest.direction;
    ds >> dest.orientation;
    ds >> dest.action;
    ds >> dest.speed;
    ds >> dest.backgroundColor;
    ds >> dest.fontColor;
    ds >> dest.fontSize;

    return ds;
}

//========================================================
QDataStream &operator>>(QDataStream& ds, PlayJson::LayerContentType& dest)
//========================================================
{
    ds >> dest.contentType;
    ds >> dest.mediaType;

    return ds;
}
