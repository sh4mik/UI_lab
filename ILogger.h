#ifndef ILOGGER_H
#define ILOGGER_H
#include "RC.h"
class ILogger {
public:
    static ILogger* createLogger(void* pClient);
    static void destroyLogger(void* pClient);
    virtual void log(char const* pMsg, enum ERR_CODES err) = 0;
    virtual int setLogFile(char const* pLogFile) = 0;
protected:
    virtual ~ILogger() = 0;
    ILogger() = default;
private:
    ILogger(ILogger const& vector) = delete;
    ILogger& operator=(ILogger const& vector) = delete;
};

#endif // ILOGGER_H
