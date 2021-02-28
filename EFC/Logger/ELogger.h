#ifndef ELOGGER_H
#define ELOGGER_H

namespace ELogger
{
    /** @brief Print Log to Console */
    void ELGO_LOG_CONSOLE(const char *procName, const char* fileName, const int line, const char *funcName,
                          const char *format, ...);
};

#endif // ELOGGER_H
