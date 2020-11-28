#include "test_vector.h"
#include "IVector.h"
#include "ILogger.h"
#include <iostream>
#include <cmath>
#include <stdlib.h>


void printVector(IVector* pOperand1) {
    if (pOperand1 != nullptr) {
        size_t size = pOperand1->getDim();
        std::cout << "{";
        for (size_t i = 0; i < size - 1; i++) {
            std::cout << pOperand1->getCoord(i) << ", ";
        }
        std::cout << pOperand1->getCoord(size - 1) << "}";
    }
    else
        std::cout << "invalid";
}
bool testCreateVector(ILogger* pLogger) {
    bool status = true;
    double* data = nullptr;
    IVector* result = IVector::createVector(2, data, pLogger);
    if (result != nullptr)
        status = false;

    if (data)
        delete data;
    if (result)
        delete result;

    data = new double[2]{ NAN, -1 };
    result = IVector::createVector(2, data, pLogger);
    if (result != nullptr)
        status = false;

    delete data;
    if (result)
        delete result;

    return status;
}

bool testAdd(ILogger* pLogger) {
    bool status = true;
    double* data_1 = new double[2]{ 0, 1 };
    double* data_2 = new double[3]{ 0, 2, 3 };
    IVector* v_1 = IVector::createVector(2, data_1, pLogger);
    IVector* v_2 = IVector::createVector(3, data_2, pLogger);

    IVector* result = IVector::add(v_1, v_2, pLogger);
    if (result != nullptr) {
        status = false;
    }

    delete[] data_1;
    delete[] data_2;
    delete v_1;
    delete v_2;
    if (result)
        delete result;

    data_1 = new double[2]{ 0, 1 };
    data_2 = new double[2]{ 0.5, 0.6 };
    v_1 = IVector::createVector(2, data_1, pLogger);
    v_2 = IVector::createVector(2, data_2, pLogger);

    result = IVector::add(v_1, v_2, pLogger);
    if (result == nullptr) {
        status = false;
    }
    if (result->getCoord(0) != 0.5 || result->getCoord(1) != 1.6) {
        status = false;
    }

    delete[] data_1;
    delete[] data_2;
    delete v_1;
    delete v_2;
    if (result)
        delete result;
    return status;
}


bool testMul(ILogger* pLogger) {
    bool status = true;
    double* data_1 = new double[2]{ 0, 1 };
    double* data_2 = new double[3]{ 0, 2, 3 };
    IVector* v_1 = IVector::createVector(2, data_1, pLogger);
    IVector* v_2 = IVector::createVector(3, data_2, pLogger);

    double result = IVector::mul(v_1, v_2, pLogger);
    if (!std::isnan(result)) {
        status = false;
    }

    delete[] data_1;
    delete[] data_2;
    delete v_1;
    delete v_2;


    data_1 = new double[2]{ 0, 1 };
    data_2 = new double[2]{ 0.5, 0.6 };
    v_1 = IVector::createVector(2, data_1, pLogger);
    v_2 = IVector::createVector(2, data_2, pLogger);

    result = IVector::mul(v_1, v_2, pLogger);
    if (std::isnan(result)) {
        status = false;
    }
    if (result != 0.6) {
        status = false;
    }

    delete[] data_1;
    delete[] data_2;
    delete v_1;
    delete v_2;
    return status;
}

bool testScaleMul(ILogger* pLogger) {
    bool status = true;
    double* data_1 = new double[2]{ 0, 1 };
    IVector* v_1 = IVector::createVector(2, data_1, pLogger);
    double scale = -2;

    IVector* result = IVector::mul(v_1, scale, pLogger);
    if (result == nullptr) {
        status = false;
    }
    if (result->getCoord(0) != 0 || result->getCoord(1) != -2)
        status = false;

    delete[] data_1;
    delete v_1;
    if (result)
        delete result;

    data_1 = new double[2]{ 0, 1 };
    v_1 = IVector::createVector(2, data_1, pLogger);
    scale = NAN;

    result = IVector::mul(v_1, scale, pLogger);
    if (result != nullptr) {
        status = false;
    }

    delete[] data_1;
    delete v_1;
    if (result)
        delete result;
    return status;
}

bool testEquals(ILogger* pLogger) {
    bool status = true;
    double* data_1 = new double[2]{ 0, 1 };
    IVector* v_1 = IVector::createVector(2, data_1, pLogger);
    IVector* v_2 = nullptr;

    bool eq_res;
    RESULT_CODE result = IVector::equals(v_1, v_2, IVector::NORM::NORM_1, 0.1, &eq_res,pLogger);
    if (result == RESULT_CODE::SUCCESS) {
        status = false;
    }

    delete[] data_1;
    delete v_1;
    if (v_2)
        delete v_2;
    return status;
}

bool testEtc(ILogger* pLogger) {
    bool status = true;
    double* data_1 = new double[2]{ 0, 1 };
    IVector* v_1 = IVector::createVector(2, data_1, pLogger);

    double result = v_1->getCoord(10);
    if (!std::isnan(result))
        status = false;

    RESULT_CODE res = v_1->setCoord(0, NAN);
    if (res == RESULT_CODE::SUCCESS) {
        status = false;
    }

    delete [] data_1;
    delete v_1;
    return status;
}

static inline void PrintRes(const char * name, bool test) {
    std::cout << name << " | ";
    if (test) {
        std::cout << "OK" << std::endl;
    }
    else {
        std::cout << "FAILED" << std::endl;
    }
}

void runAllTests(ILogger* pLogger) {
    bool test = testCreateVector(pLogger);
    PrintRes("testCreateVector", test);

    test = testAdd(pLogger);
    PrintRes("testAdd", test);

    test = testMul(pLogger);
    PrintRes("testMul", test);

    test = testScaleMul(pLogger);
    PrintRes("testScaleMul", test);

    test = testEquals(pLogger);
    PrintRes("testEquals", test);

    test = testEtc(pLogger);
    PrintRes("testEtc", test);
}
