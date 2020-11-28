#include <QCoreApplication>
#include "test_vector.h"

int main(int argc, char *argv[])
{
    ILogger* pLogger = ILogger::createLogger(nullptr);
    runAllTests(pLogger);
    pLogger->destroyLogger(nullptr);
}
