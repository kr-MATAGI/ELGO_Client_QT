#include "ContentsPlayDataImpl.h"



//========================================================
QDataStream &operator<<(QDataStream& ds, const PlayJson::CustomPlayDataJson &src)
//========================================================
{
    ds << src.playData;
    ds << src.pageDataList;

    return ds;
}

//========================================================
QDataStream &operator<<(QDataStream& ds, const PlayJson::FixedPlayDataJson &src)
//========================================================
{
    ds << src.playData;
    ds << src.layerDataList;
    ds << src.subtitleDataList;

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
    ds << src.layerDataList;
    ds << src.subtitleDataList;

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
    ds << src.contentDataList;

    return ds;
}

//========================================================
QDataStream &operator<<(QDataStream& ds, const PlayJson::ContentData& src)
//========================================================
{
    ds << src.contentInfo;
    ds << src.name;
    ds << src.fileDuration;
    ds << src.userDuration;
    ds << src.zIndex;
    ds << src.fontColor;
    ds << src.backgroundColor;
    ds << src.bBackgroundOpacity;

    // weather
    ds << src.metropolCity;
    ds << src.metropolCityName;
    ds << src.city;
    ds << src.cityName;
    ds << src.nx;
    ds << src.ny;
    ds << src.PTY;
    ds << src.SKY;
    ds << src.T1H;
    ds << src.RN1;
    ds << src.REH;
    ds << src.VEC;
    ds << src.WSD;
    ds << src.LGT;

    // clock
    ds << src.hourType;

    // date
    ds << src.dateType;

    // news
    ds << src.newsCategory;
    ds << src.newsCount;
    ds << src.newsFeedList;
    ds << src.newsfontSize;
    ds << src.newsBoxColor;
    ds << src.bNewsBoxOpacity;

    return ds;
}

//========================================================
QDataStream &operator<<(QDataStream& ds, const PlayJson::ContentInfo& src)
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
    ds >> dest.pageDataList;

    return ds;
}

//========================================================
QDataStream &operator>>(QDataStream& ds, PlayJson::FixedPlayDataJson& dest)
//========================================================
{
    ds >> dest.playData;
    ds >> dest.layerDataList;
    ds >> dest.subtitleDataList;

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
    ds >> dest.layerDataList;
    ds >> dest.subtitleDataList;

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
    ds >> dest.contentDataList;

    return ds;
}

//========================================================
QDataStream &operator>>(QDataStream& ds, PlayJson::ContentData& dest)
//========================================================
{
    ds >> dest.contentInfo;
    ds >> dest.name;
    ds >> dest.fileDuration;
    ds >> dest.userDuration;
    ds >> dest.zIndex;
    ds >> dest.fontColor;
    ds >> dest.backgroundColor;
    ds >> dest.bBackgroundOpacity;

    // weather
    ds >> dest.metropolCity;
    ds >> dest.metropolCityName;
    ds >> dest.city;
    ds >> dest.cityName;
    ds >> dest.nx;
    ds >> dest.ny;
    ds >> dest.PTY;
    ds >> dest.SKY;
    ds >> dest.T1H;
    ds >> dest.RN1;
    ds >> dest.REH;
    ds >> dest.VEC;
    ds >> dest.WSD;
    ds >> dest.LGT;

    // clock
    ds >> dest.hourType;

    // date
    ds >> dest.dateType;

    // news
    ds >> dest.newsCategory;
    ds >> dest.newsCount;
    ds >> dest.newsFeedList;
    ds >> dest.newsfontSize;
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
QDataStream &operator>>(QDataStream& ds, PlayJson::ContentInfo& dest)
//========================================================
{
    ds >> dest.contentType;
    ds >> dest.mediaType;

    return ds;
}

//========================================================
bool PlayJson::operator==(const PlayJson::FixedPlayDataJson& lhs, const PlayJson::FixedPlayDataJson& rhs)
//========================================================
{
    if(!(lhs.playData == rhs.playData))
        return false;
    if(!(lhs.layerDataList == rhs.layerDataList))
        return false;
    if(!(lhs.subtitleDataList == rhs.subtitleDataList))
        return false;

    return true;
}

//========================================================
bool PlayJson::operator==(const PlayJson::CustomPlayDataJson& lhs, const PlayJson::CustomPlayDataJson& rhs)
//========================================================
{
    if(!(lhs.playData == rhs.playData))
        return false;
    if(!(lhs.pageDataList == rhs.pageDataList))
        return false;

    return true;
}

//========================================================
bool PlayJson::operator==(const PlayJson::PlayData& lhs, const PlayJson::PlayData& rhs)
//========================================================
{
    if(!(lhs.id == rhs.id))
        return false;
    if(!(lhs.name == rhs.name))
        return false;
    if(!(lhs.memo == rhs.memo))
        return false;
    if(!(lhs.width == rhs.width))
        return false;
    if(!(lhs.height == rhs.height))
        return false;
    if(!(lhs.orientation == rhs.orientation))
        return false;
    if(!(lhs.playDataType == rhs.playDataType))
        return false;

    return true;
}

//========================================================
bool PlayJson::operator==(const PlayJson::FixedLayerData& lhs, const PlayJson::FixedLayerData& rhs)
//========================================================
{
    if(!(lhs.left == rhs.left))
        return false;
    if(!(lhs.top == rhs.top))
        return false;
    if(!(lhs.width == rhs.width))
        return false;
    if(!(lhs.height == rhs.height))
        return false;
    if(!(lhs.contentDataList == rhs.contentDataList))
        return false;

    return true;
}

//========================================================
bool PlayJson::operator==(const PlayJson::ContentData& lhs, const PlayJson::ContentData& rhs)
//========================================================
{
    if(!(lhs.contentInfo == rhs.contentInfo))
        return false;
    if(!(lhs.name == rhs.name))
        return false;
    if(!(lhs.fileDuration == rhs.fileDuration))
        return false;
    if(!(lhs.userDuration == rhs.userDuration))
        return false;
    if(!(lhs.zIndex == rhs.zIndex))
        return false;
    if(!(lhs.fontColor == rhs.fontColor))
        return false;
    if(!(lhs.backgroundColor == rhs.backgroundColor))
        return false;
    if(!(lhs.bBackgroundOpacity == rhs.bBackgroundOpacity))
        return false;
    if(!(lhs.metropolCity == rhs.metropolCity))
        return false;
    if(!(lhs.metropolCityName == rhs.metropolCityName))
        return false;
    if(!(lhs.city == rhs.city))
        return false;
    if(!(lhs.cityName == rhs.cityName))
        return false;
    if(!(lhs.nx == rhs.nx))
        return false;
    if(!(lhs.ny == rhs.ny))
        return false;
    if(!(lhs.PTY == rhs.PTY))
        return false;
    if(!(lhs.SKY == rhs.SKY))
        return false;
    if(!(lhs.T1H == rhs.T1H))
        return false;
    if(!(lhs.RN1 == rhs.RN1))
        return false;
    if(!(lhs.REH == rhs.REH))
        return false;
    if(!(lhs.VEC == rhs.VEC))
        return false;
    if(!(lhs.WSD == rhs.WSD))
        return false;
    if(!(lhs.LGT == rhs.LGT))
        return false;
    if(!(lhs.hourType == rhs.hourType))
        return false;
    if(!(lhs.dateType == rhs.dateType))
        return false;
    if(!(lhs.newsCategory == rhs.newsCategory))
        return false;
    if(!(lhs.newsCount == rhs.newsCount))
        return false;
    if(!(lhs.newsfontSize == rhs.newsfontSize))
        return false;   
    if(!(lhs.newsBoxColor == rhs.newsBoxColor))
        return false;
    if(!(lhs.bNewsBoxOpacity == rhs.bNewsBoxOpacity))
        return false;

    return true;
}

//========================================================
bool PlayJson::operator==(const PlayJson::ContentInfo& lhs, const PlayJson::ContentInfo& rhs)
//========================================================
{
    if(!(lhs.contentType == rhs.contentType))
        return false;
    if(!(lhs.mediaType == rhs.mediaType))
        return false;

    return true;
}

//========================================================
bool PlayJson::operator==(const PlayJson::SubtitleData& lhs, const PlayJson::SubtitleData& rhs)
//========================================================
{
    if(!(lhs.left == rhs.left))
        return false;
    if(!(lhs.top == rhs.top))
        return false;
    if(!(lhs.width == rhs.width))
        return false;
    if(!(lhs.height == rhs.height))
        return false;
    if(!(lhs.text == rhs.text))
        return false;
    if(!(lhs.bIsFixed == rhs.bIsFixed))
        return false;
    if(!(lhs.direction == rhs.direction))
        return false;
    if(!(lhs.orientation == rhs.orientation))
        return false;
    if(!(lhs.action == rhs.action))
        return false;
    if(!(lhs.speed == rhs.speed))
        return false;
    if(!(lhs.backgroundColor == rhs.backgroundColor))
        return false;
    if(!(lhs.fontColor == rhs.fontColor))
        return false;
    if(!(lhs.fontSize == rhs.fontSize))
        return false;

    return true;
}

//========================================================
bool PlayJson::operator==(const PlayJson::PageData& lhs, const PlayJson::PageData& rhs)
//========================================================
{
    if(!(lhs.duration == rhs.duration))
        return false;
    if(!(lhs.layerDataList == rhs.layerDataList))
        return false;
    if(!(lhs.subtitleDataList == rhs.subtitleDataList))
        return false;

    return true;
}

//========================================================
bool PlayJson::operator==(const PlayJson::CustomLayerData& lhs, const PlayJson::CustomLayerData& rhs)
//========================================================
{
    if(!(lhs.left == rhs.left))
        return false;
    if(!(lhs.top == rhs.top))
        return false;
    if(!(lhs.width == rhs.width))
        return false;
    if(!(lhs.height == rhs.height))
        return false;
    if(!(lhs.layerContent == rhs.layerContent))
        return false;

    return true;
}
