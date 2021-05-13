#ifndef UPDATELOGGER_H
#define UPDATELOGGER_H

#define ELGO_UPDATE_LOG(format, ...) ELogger::ELGO_LOG_CONSOLE("elgo_update", __FILE__, __LINE__, __FUNCTION__, format, ##__VA_ARGS__);

#endif // UPDATELOGGER_H
