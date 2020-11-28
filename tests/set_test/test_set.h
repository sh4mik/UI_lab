#ifndef TEST_SET_H
#define TEST_SET_H
#include "IVector.h"
#include "ILogger.h"

void printVector(IVector* pOperand1);
bool testInsert(ILogger* pLogger);
bool testErase(ILogger* pLogger);
bool testGet(ILogger* pLogger);
bool testAdd(ILogger* pLogger);
bool testIntersect(ILogger* pLogger);
void runAllTests(ILogger* pLogger);
#endif // TEST_SET_H
