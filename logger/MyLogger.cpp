#include "Logger_global.h"
#include "ILogger.h"
#include <set>
#include <string>
#include <fstream>

class MyLogger : public ILogger {
public:
    MyLogger() = default;

    static MyLogger* createLogger(void* pClient);
    void destroyLogger(void* pClient) override;
    void log(char const* pMsg, RESULT_CODE err) override;
    RESULT_CODE setLogFile(char const* pLogFile) override;


    ~MyLogger() override {}
private:
    static std::set<void*> clients_;
    static std::ofstream fileout_;
};

std::set<void*> MyLogger::clients_;
std::ofstream MyLogger::fileout_;

MyLogger *MyLogger::createLogger(void *pClient) {
    MyLogger::clients_.insert(pClient);
    return new MyLogger();
}

void MyLogger::destroyLogger(void *pClient) {
    clients_.erase(pClient);
}

void MyLogger::log(const char *pMsg, RESULT_CODE err) {
    static const char * enumNames[] = {
        "SUCCESS",
        "OUT_OF_MEMORY",
        "BAD_REFERENCE",
        "WRONG_DIM",
        "DIVISION_BY_ZERO",
        "NAN_VALUE",
        "FILE_ERROR",
        "OUT_OF_BOUNDS",
        "NOT_FOUND",
        "WRONG_ARGUMENT",
        "CALCULATION_ERROR",
        "MULTIPLE_DEFINITION"
    };
    fileout_ << enumNames[static_cast<int>(err)] << ": " << pMsg << std::endl;
}

RESULT_CODE MyLogger::setLogFile(const char *pLogFile) {
    if (pLogFile) {
        return RESULT_CODE::BAD_REFERENCE;
    }
    fileout_.close();
    fileout_.open(pLogFile);
    if (fileout_) {
        return RESULT_CODE::SUCCESS;
    } else {
        return RESULT_CODE::NOT_FOUND;
    }
}

ILogger* ILogger::createLogger(void* pClient) {
    return MyLogger::createLogger(pClient);
}

ILogger::~ILogger() {}

