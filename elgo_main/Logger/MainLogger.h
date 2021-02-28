#ifndef MAINLOGGER_H
#define MAINLOGGER_H

#include "Logger/ELogger.h"

#define ELGO_MAIN_LOG(format, ...) ELogger::ELGO_LOG_CONSOLE("elgo_main", __FILE__, __LINE__, __FUNCTION__, format, ##__VA_ARGS__);

#endif // MAINLOGGER_H
