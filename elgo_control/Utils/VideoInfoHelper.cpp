
// Viewer
#include "VideoInfoHelper.h"
#include "DownloadThread/DownloadDef.h"


//========================================================
qint64 VideoInfoHelper::GetVideoDuration(const QString& name)
//========================================================
{
    qint64 retValue = 0;

    std::string filePath = RESOURCE_SAVE_PATH;
    filePath += "video/";
    filePath += name.toStdString().c_str();

    AVFormatContext *pFormatCtx = avformat_alloc_context();
    avformat_open_input(&pFormatCtx, filePath.c_str(), NULL, NULL);
    avformat_find_stream_info(pFormatCtx, NULL);
    retValue = static_cast<qint64>(pFormatCtx->duration / 1000);

    avformat_close_input(&pFormatCtx);
    avformat_free_context(pFormatCtx);

    return retValue;
}

//========================================================
void VideoInfoHelper::MatchVideoDuration(const QList<QPair<QString, qint64>>& src,
                                         QList<PlayJson::CustomLayerData>& dest)
//========================================================
{
    foreach(PlayJson::CustomLayerData layerData, dest)
    {
        foreach(auto videoInfo, src)
        {
            if(0 == strcmp(videoInfo.first.toStdString().c_str(),
                           layerData.layerContent.name.toStdString().c_str()))
            {
                layerData.layerContent.fileDuration = videoInfo.second;
            }
        }
    }
}

//========================================================
void VideoInfoHelper::MatchVideoDuration(const QList<QPair<QString, qint64>>& src,
                        QList<PlayJson::FixedLayerData>& dest)
//========================================================
{
    foreach(PlayJson::FixedLayerData layerData, dest)
    {
        foreach(PlayJson::ContentData contentData, layerData.contentDataList)
        {
            foreach(auto videoInfo, src)
            {
                if(0 == strcmp(videoInfo.first.toStdString().c_str(),
                               contentData.name.toStdString().c_str()))
                {
                    contentData.fileDuration = videoInfo.second;
                }
            }
        }
    }
}
