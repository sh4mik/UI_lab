#ifndef COMPACT_TEST_H
#define COMPACT_TEST_H

#include "ILogger.h"
#include "ICompact.h"
#include "IVector.h"
void printVector(IVector* pOperand1);
bool testCreate(ILogger* pLogger);
bool testClone(ILogger* pLogger);
bool testIsContains(ILogger* pLogger);
bool testIsSubset(ILogger* pLogger);
bool testIsIntersects(ILogger* pLogger);
bool testIntersection(ILogger* pLogger) ;
bool testAdd(ILogger* pLogger);
bool testMakeConvex(ILogger* pLogger);
bool testCreateIterator(ILogger* pLogger);
bool testSetDirection(ILogger* pLogger);
bool testIterator(ILogger* pLogger);
void runAllTest(ILogger* pLogger);

#endif // COMPACT_TEST_H
