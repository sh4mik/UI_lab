#include <QCoreApplication>
#include "compact_test.h"
int main(int argc, char *argv[])
{
    ILogger* pLogger = ILogger::createLogger(nullptr);
    runAllTest(pLogger);
    pLogger->destroyLogger(nullptr);
}
