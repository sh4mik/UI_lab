#include <iostream>
#include "ILogger.h"
#include "IVector.h"
#include "ISet.h"
#include <cmath>
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
bool testInsert(ILogger* pLogger) {
    bool status = true;
    double* data_1 = new double[3]{ 1, 2, 3 };
    double* data_2 = new double[3]{ 2, 4, 5 };
    double* data_3 = new double[2]{ 0, 1 };
    double* data_4 = new double[3]{ 1.001, 2.001, 3.001 };

    IVector* pOp1 = IVector::createVector(3, data_1, pLogger);
    IVector* pOp2 = IVector::createVector(3, data_2, pLogger);
    IVector* pOp3 = IVector::createVector(2, data_3, pLogger);
    IVector* pOp4 = IVector::createVector(3, data_4, pLogger);
    ISet* set = ISet::createSet(pLogger);
    RESULT_CODE res = set->insert(pOp1, IVector::NORM::NORM_1, 0.01);
    if (res != RESULT_CODE::SUCCESS)
        status = false;

    res = set->insert(pOp2, IVector::NORM::NORM_1, NAN);
    if (res == RESULT_CODE::SUCCESS)
        status = false;

    res = set->insert(pOp3, IVector::NORM::NORM_1, 0.01);
    if (res == RESULT_CODE::SUCCESS)
        status = false;

    size_t size = set->getSize();
    res = set->insert(pOp4, IVector::NORM::NORM_1, 0.01);
    if (res != RESULT_CODE::SUCCESS || size != set->getSize())
        status = false;

    set->clear();
    delete set;
    delete pOp1;
    delete pOp2;
    delete pOp3;
    delete pOp4;

    delete[] data_1;
    delete[] data_2;
    delete[] data_3;
    delete[] data_4;

    return status;
}


bool testErase(ILogger* pLogger) {
    bool status = true;
    double* data_1 = new double[3]{ 1, 2, 3 };
    double* data_2 = new double[3]{ 2, 4, 5 };
    double* data_3 = new double[2]{ 0, 1 };
    double* data_4 = new double[3]{ 1.001, 2.001, 3.001 };

    IVector* pOp1 = IVector::createVector(3, data_1, pLogger);
    IVector* pOp2 = IVector::createVector(3, data_2, pLogger);
    IVector* pOp3 = IVector::createVector(2, data_3, pLogger);
    IVector* pOp4 = IVector::createVector(3, data_4, pLogger);
    ISet* set = ISet::createSet(pLogger);
    RESULT_CODE res = set->insert(pOp1, IVector::NORM::NORM_1, 0.01);
    res = set->insert(pOp2, IVector::NORM::NORM_1, 0.01);

    size_t size = set->getSize();
    res = set->erase(1);
    if (res != RESULT_CODE::SUCCESS || set->getSize() != size - 1)
        status = false;

    size = set->getSize();
    res = set->erase(pOp4, IVector::NORM::NORM_1, 0.01);
    if (res != RESULT_CODE::SUCCESS || set->getSize() != size - 1)
        status = false;

    res = set->erase(20);
    if (res == RESULT_CODE::SUCCESS)
        status = false;

    res = set->erase(pOp4, IVector::NORM::NORM_1, NAN);
    if (res == RESULT_CODE::SUCCESS)
        status = false;

    set->clear();
    delete set;
    delete pOp1;
    delete pOp2;
    delete pOp3;
    delete pOp4;

    delete[] data_1;
    delete[] data_2;
    delete[] data_3;
    delete[] data_4;

    return status;
}

bool testGet(ILogger* pLogger) {
    bool status = true;
    double* data_1 = new double[3]{ 1, 2, 3 };
    double* data_2 = new double[3]{ 2, 4, 5 };
    double* data_3 = new double[3]{ 2.001, 4.001, 5.001 };

    IVector* pOp1 = IVector::createVector(3, data_1, pLogger);
    IVector* pOp2 = IVector::createVector(3, data_2, pLogger);
    IVector* pOp3 = IVector::createVector(3, data_3, pLogger);

    ISet* set = ISet::createSet(pLogger);
    RESULT_CODE res = set->insert(pOp1, IVector::NORM::NORM_1, 0.01);
    //res = set->insert(pOp2, IVector::NORM::NORM_1, 0.01);

    IVector* result;
    res = set->get(result, 20);
    if (result != nullptr || res == RESULT_CODE::SUCCESS)
        status = false;

    res = set->insert(pOp3, IVector::NORM::NORM_1, 0.01);


    res = set->get(result, pOp2, IVector::NORM::NORM_1, 0.1);
    bool eq_res;
    res = IVector::equals(result, pOp3, IVector::NORM::NORM_1, 0.01, &eq_res, pLogger);
    if (res != RESULT_CODE::SUCCESS || eq_res != true)
        status = false;

    delete result;
    res = set->get(result, pOp2, IVector::NORM::NORM_1, NAN);
    if (result != nullptr)
        status = false;


    set->clear();
    delete set;
    delete pOp1;
    delete pOp2;
    delete pOp3;
    delete result;

    delete[] data_1;
    delete[] data_2;
    delete[] data_3;

    return status;
}


bool testAdd(ILogger* pLogger) {
    bool status = true;
    double* data_1 = new double[3]{ 1, 2, 3 };
    double* data_2 = new double[3]{ 2, 4, 5 };

    IVector* pOp1 = IVector::createVector(3, data_1, pLogger);
    IVector* pOp2 = IVector::createVector(3, data_2, pLogger);

    ISet* set_1 = ISet::createSet(pLogger);
    ISet* set_2 = ISet::createSet(pLogger);

    set_1->insert(pOp1, IVector::NORM::NORM_1, 0.01);
    set_2->insert(pOp2, IVector::NORM::NORM_1, 0.01);

    ISet* addition = ISet::add(set_1, set_2, IVector::NORM::NORM_1, 0.01, pLogger);

    if (addition == nullptr || addition->getSize() != 2) {
        status = false;
    }

    addition->clear();
    delete addition;
    set_1->insert(pOp2, IVector::NORM::NORM_1, 0.01);
    addition = ISet::add(set_1, set_2, IVector::NORM::NORM_1, 0.01, pLogger);
    if (addition == nullptr || addition->getSize() != 2) {
        status = false;
    }

    addition->clear();
    delete addition;
    addition = ISet::add(set_1, nullptr, IVector::NORM::NORM_1, 0.01, pLogger);
    if (addition != nullptr)
        status = false;

    set_1->clear();
    set_2->clear();
    delete set_1;
    delete set_2;
    delete pOp1;
    delete pOp2;

    if (addition)
        addition->clear();

    delete addition;
    delete[] data_1;
    delete[] data_2;

    return status;
}

bool testIntersect(ILogger* pLogger) {
    bool status = true;
    double* data_1 = new double[3]{ 1, 2, 3 };
    double* data_2 = new double[3]{ 2, 4, 5 };
    double* data_3 = new double[3]{ 2.001, 4.001, 5.001 };

    IVector* pOp1 = IVector::createVector(3, data_1, pLogger);
    IVector* pOp2 = IVector::createVector(3, data_2, pLogger);
    IVector* pOp3 = IVector::createVector(3, data_3, pLogger);
    ISet* set_1 = ISet::createSet(pLogger);
    ISet* set_2 = ISet::createSet(pLogger);

    set_1->insert(pOp1, IVector::NORM::NORM_1, 0.01);
    set_2->insert(pOp2, IVector::NORM::NORM_1, 0.01);

    ISet* intersection = ISet::intersect(set_1, set_2, IVector::NORM::NORM_1, 0.01, pLogger);

    if (intersection->getSize() != 0 || intersection == nullptr) {
        status = false;
    }

    if (intersection)
        intersection->clear();
    delete  intersection;


    set_1->insert(pOp3, IVector::NORM::NORM_1, 0.01);
    intersection = ISet::intersect(set_1, set_2, IVector::NORM::NORM_1, 0.1, pLogger);


    if (intersection == nullptr || intersection->getSize() != 1) {
        status = false;
    }
    if (intersection)
        intersection->clear();
    delete intersection;


    intersection = ISet::intersect(set_1, nullptr, IVector::NORM::NORM_1, 0.01, pLogger);
    if (intersection != nullptr)
        status = false;

    intersection = ISet::intersect(set_1, set_2, IVector::NORM::NORM_1, NAN, pLogger);
    if (intersection != nullptr)
        status = false;

    set_1->clear();
    set_2->clear();
    delete set_1;
    delete set_2;
    delete pOp1;
    delete pOp2;
    delete pOp3;
    if (intersection)
        intersection->clear();
    delete intersection;
    delete[] data_1;
    delete[] data_2;
    delete[] data_3;

    return status;
}

void runAllTests(ILogger* pLogger) {
    bool test = testInsert(pLogger);
    std::cout << "INSERT SET TEST | ";
    if (test)
        std::cout << "OK";
    else
        std::cout << "FAILED";

    std::cout << std::endl;
    test = testErase(pLogger);
    std::cout << "ERASE SET TEST | ";
    if (test)
        std::cout << "OK";
    else
        std::cout << "FAILED";

    std::cout << std::endl;
    test = testGet(pLogger);
    std::cout << "GET SET TEST | ";
    if (test)
        std::cout << "OK";
    else
        std::cout << "FAILED";

    std::cout << std::endl;
    test = testAdd(pLogger);
    std::cout << "ADDITION SET TEST | ";
    if (test)
        std::cout << "OK";
    else
        std::cout << "FAILED";

    std::cout << std::endl;
    test = testIntersect(pLogger);
    std::cout << "INTERSECTION SET TEST | ";
    if (test)
        std::cout << "OK";
    else
        std::cout << "FAILED";
    std::cout << std::endl;
}
