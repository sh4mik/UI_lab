#include <QCoreApplication>
#include "test_set.h"

int main(int argc, char *argv[])
{
    ILogger* pLogger = ILogger::createLogger(nullptr);
    runAllTests(pLogger);
    pLogger->destroyLogger(nullptr);
}
