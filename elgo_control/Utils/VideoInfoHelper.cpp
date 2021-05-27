
// Viewer
#include "VideoInfoHelper.h"
#include "DownloadThread/DownloadDef.h"

// Common
#include "Common/CommonDef.h"

//========================================================
qint64 VideoInfoHelper::GetVideoDuration(const QString& name)
//========================================================
{
    qint64 retValue = 0;

    std::string filePath = RESOURCE_DIR_PATH;
    filePath += "video/";
    filePath += name.toStdString().c_str();

    AVFormatContext *pFormatCtx = avformat_alloc_context();
    avformat_open_input(&pFormatCtx, filePath.c_str(), NULL, NULL);
    avformat_find_stream_info(pFormatCtx, NULL);

    // usec -> msec
    retValue = static_cast<qint64>(pFormatCtx->duration / 1000);

    avformat_close_input(&pFormatCtx);
    avformat_free_context(pFormatCtx);

    return retValue;
}

//========================================================
void VideoInfoHelper::MatchVideoDuration(const QVector<QPair<QString, qint64>>& src,
                                         QVector<PlayJson::CustomLayerData>& dest)
//========================================================
{
    QVector<PlayJson::CustomLayerData>::iterator iter = dest.begin();

    for(; iter != dest.end(); ++iter)
    {
        foreach(auto videoInfo, src)
        {
            if(0 == strcmp(videoInfo.first.toStdString().c_str(),
                           iter->layerContent.name.toStdString().c_str()))
            {
                iter->layerContent.fileDuration = videoInfo.second;
            }
        }
    }
}

//========================================================
void VideoInfoHelper::MatchVideoDuration(const QVector<QPair<QString, qint64>>& src,
                        QVector<PlayJson::FixedLayerData>& dest)
//========================================================
{
    QVector<PlayJson::FixedLayerData>::iterator layerIter = dest.begin();

    for(; layerIter != dest.end(); ++layerIter)
    {
        QVector<PlayJson::ContentData>::iterator cdIter = layerIter->contentDataList.begin();

        for(; cdIter != layerIter->contentDataList.end(); ++cdIter)
        {
            foreach(auto videoInfo, src)
            {
                if(0 == strcmp(videoInfo.first.toStdString().c_str(),
                               cdIter->name.toStdString().c_str()))
                {
                    cdIter->fileDuration = videoInfo.second;
                }
            }
        }
    }
}
