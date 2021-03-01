#ifndef VIEWERLOGGER_H
#define VIEWERLOGGER_H

#include "Logger/ELogger.h"

#define ELGO_VIEWER_LOG(format, ...) ELogger::ELGO_LOG_CONSOLE("elgo_viewer", __FILE__, __LINE__, __FUNCTION__, format, ##__VA_ARGS__);

#endif // VIEWERLOGGER_H
