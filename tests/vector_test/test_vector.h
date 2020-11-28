#ifndef TEST_VECTOR_H
#define TEST_VECTOR_H
#include "ILogger.h"
#include "IVector.h"

void printVector(IVector* pOperand1);
bool testCreateVector(ILogger* pLogger);
bool testAdd(ILogger* pLogger);
bool testMul(ILogger* pLogger);
bool testScaleMul(ILogger* pLogger);
bool testEquals(ILogger* pLogger);
bool testEtc(ILogger* pLogger);
void runAllTests(ILogger* pLogger);
#endif // TEST_VECTOR_H
