#ifndef CONTROLLOGGER_H
#define CONTROLLOGGER_H

#include "Logger/ELogger.h"

#define ELGO_CONTROL_LOG(format, ...) ELogger::ELGO_LOG_CONSOLE("elgo_control", __FILE__, __LINE__, __FUNCTION__, format, ##__VA_ARGS__);

#endif // CONTROLLOGGER_H
