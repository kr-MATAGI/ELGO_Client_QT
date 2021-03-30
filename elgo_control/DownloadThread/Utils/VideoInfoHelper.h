#ifndef VIDEOINFOHELPER_H
#define VIDEOINFOHELPER_H


// QT
#include <QString>

// ffmpeg
extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/avutil.h>
}

// Viewer
#include "JSON/Schema/ContentServerSchema.h"

// Common
#include "Common/Interface/ConetnsPlayDataDef.h"

namespace VideoInfoHelper
{
    /** @brief */
    qint64 GetVideoDuration(const QString& name);
}

#endif // VIDEOINFOHELPER_H
