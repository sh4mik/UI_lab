#include "ICompact.h"
#include "IVector.h"
#include <cmath>
#include <iostream>


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



bool testCreate(ILogger* pLogger) {
    /*ok-test 2D*/
    bool status = true;
    double* data11 = new double[2]{ 0, 1 };
    double* data12 = new double[2]{ 3, 3 };
    IVector* pOp1 = IVector::createVector(2, data11, pLogger);
    IVector* pOp2 = IVector::createVector(2, data12, pLogger);

    ICompact* result = ICompact::createCompact(pOp1, pOp2, pLogger);
    if (result == nullptr) {
        status = false;
    }


    delete[] data12;
    delete pOp2;
    delete result;

    ///* test with NAN */
    data12 = new double[2]{ 3, NAN };
    pOp2 = IVector::createVector(2, data12, pLogger);
    result = ICompact::createCompact(pOp1, pOp2, pLogger);
    if (result != nullptr) {
        status = false;
    }


    delete pOp2;
    delete result;
    /* test with nullptr vector */
    pOp2 = nullptr;
    result = ICompact::createCompact(pOp1, pOp2, pLogger);
    if (result != nullptr) {
        status = false;
    }

    delete[] data11;
    delete[] data12;
    delete pOp1;
    delete pOp2;
    delete result;
    /* coords of begin are bigger than end ones*/
    data11 = new double[2]{ 3, 3 };
    data12 = new double[2]{ 1, 3 };
    pOp1 = IVector::createVector(2, data11, pLogger);
    pOp2 = IVector::createVector(2, data12, pLogger);
    result = ICompact::createCompact(pOp1, pOp2, pLogger);
    if (result != nullptr) {
        status = false;
    }

    delete[] data11;
    delete[] data12;
    delete pOp1;
    delete pOp2;
    delete result;
    /* ok-test 3D*/
    data11 = new double[3]{ -3, 0, 3 };
    data12 = new double[3]{ 1, 3, 6 };
    pOp1 = IVector::createVector(2, data11, pLogger);
    pOp2 = IVector::createVector(2, data12, pLogger);
    result = ICompact::createCompact(pOp1, pOp2, pLogger);
    if (result == nullptr) {
        status =  false;
    }

    delete[] data11;
    delete[] data12;
    delete pOp1;
    delete pOp2;
    delete result;
    /*ok-test 2D - same coords*/
    data11 = new double[2]{ 0, 1 };
    data12 = new double[2]{ 0, 1 };
    pOp1 = IVector::createVector(2, data11, pLogger);
    pOp2 = IVector::createVector(2, data12, pLogger);
    result = ICompact::createCompact(pOp1, pOp2, pLogger);
    if (result == nullptr) {
        status =  false;
    }


    delete[] data11;
    delete[] data12;
    delete pOp1;
    delete pOp2;
    delete result;

    return status;
}

bool testClone(ILogger* pLogger) {
    bool status = true;
    /*ok-test 2D*/
    double* data11 = new double[2]{ 0, 1 };
    double* data12 = new double[2]{ 3, 3 };
    IVector* pOp1 = IVector::createVector(2, data11, pLogger);
    IVector* pOp2 = IVector::createVector(2, data12, pLogger);

    ICompact* result = ICompact::createCompact(pOp1, pOp2, pLogger);
    ICompact* clone_result = result->clone();
    if (result == nullptr) {
        status = false;
    }

    delete[] data11;
    delete[] data12;
    data11 = nullptr, data12 = nullptr;

    delete pOp1;
    delete pOp2;
    pOp1 = nullptr, pOp2 = nullptr;

    delete result;
    result = nullptr;
    delete clone_result;
    clone_result = nullptr;
    return status;
}


bool testIsContains(ILogger* pLogger) {
    bool status = true;

    /* 2D ok-test point inside compact*/
    double* data11 = new double[2]{ 0, 1 };
    double* data12 = new double[2]{ 3, 3 };
    IVector* pOp1 = IVector::createVector(2, data11, pLogger);
    IVector* pOp2 = IVector::createVector(2, data12, pLogger);

    ICompact* result = ICompact::createCompact(pOp1, pOp2, pLogger);
    double* data_check = new double[2]{ 1, 2 };
    IVector* vect_check = IVector::createVector(2, data_check, pLogger);

    bool resultF;
    RESULT_CODE res = result->isContains(vect_check, resultF);
    if (res != RESULT_CODE::SUCCESS || !resultF) {
        status = false;
    }

    delete[] data_check;
    delete vect_check;

    /* 2D point in the border */
    data_check = new double[2]{ 2, 1 };
    vect_check = IVector::createVector(2, data_check, pLogger);
    res = result->isContains(vect_check, resultF);
    if (res != RESULT_CODE::SUCCESS || !resultF) {
        status = false;
    }

    delete[] data11;
    delete[] data12;
    delete[] data_check;
    delete pOp1;
    delete pOp2;
    delete vect_check;
    delete result;


    /* 3D does not contain */
    double* data3D_1 = new double[3]{ 1, 0, 0 };
    double* data3D_2 = new double[3]{ 5, 3, 2 };
    double* data_check3D = new double[3]{ 0, 0, 0 };
    IVector* pOp3D1 = IVector::createVector(3, data3D_1, pLogger);
    IVector* pOp3D2 = IVector::createVector(3, data3D_2, pLogger);
    IVector* vect_check3D = IVector::createVector(3, data_check3D, pLogger);
    ICompact* result3D = ICompact::createCompact(pOp3D1, pOp3D2, pLogger);
    res = result3D->isContains(vect_check3D, resultF);
    if (res != RESULT_CODE::SUCCESS || resultF) {
        status = false;
    }

    /* 3D point == end */
    res = result3D->isContains(pOp3D2, resultF);
    if (res != RESULT_CODE::SUCCESS || !resultF) {
        status = false;
    }

    delete[] data3D_1;
    delete[] data3D_2;
    delete[] data_check3D;

    delete pOp3D1;
    delete pOp3D2;
    delete vect_check3D;

    delete result3D;

    return status;
}

bool testIsSubset(ILogger* pLogger) {
    bool status = true;

    /* right is subset of left*/
    double* data11 = new double[2]{ 0, 1 };
    double* data12 = new double[2]{ 3, 3 };
    IVector* pOp1 = IVector::createVector(2, data11, pLogger);
    IVector* pOp2 = IVector::createVector(2, data12, pLogger);


    double* data21 = new double[2]{ 2, 1 };
    double* data22 = new double[2]{ 3, 2 };
    IVector* pOp21 = IVector::createVector(2, data21, pLogger);
    IVector* pOp22 = IVector::createVector(2, data22, pLogger);
    ICompact* left = ICompact::createCompact(pOp1, pOp2, pLogger);
    ICompact* right = ICompact::createCompact(pOp21, pOp22, pLogger);

    bool resultF;
    RESULT_CODE res = left->isSubSet(right, resultF);
    if (res != RESULT_CODE::SUCCESS || !resultF) {
        status = false;
    }

    /* backward - false */
    res = right->isSubSet(left, resultF);
    if (res != RESULT_CODE::SUCCESS || resultF) {
        status = false;
    }

    /* own subset */
    res = right->isSubSet(right, resultF);
    if (res != RESULT_CODE::SUCCESS || !resultF) {
        status = false;
    }

    delete[] data11;
    delete[] data12;
    delete[] data21;
    delete[] data22;
    data11 = nullptr, data12 = nullptr, data21 = nullptr, data22 = nullptr;

    delete pOp1;
    delete pOp2;
    delete pOp21;
    delete pOp22;
    pOp1 = nullptr, pOp2 = nullptr, pOp21 = nullptr, pOp22 = nullptr;

    delete left;
    delete right;
    left = nullptr, right = nullptr;
    return status;
}


bool testIsIntersects(ILogger* pLogger) {
    bool status = true;

    /* right is subset of left*/
    double* data11 = new double[2]{ 0, 1 };
    double* data12 = new double[2]{ 3, 3 };
    IVector* pOp1 = IVector::createVector(2, data11, pLogger);
    IVector* pOp2 = IVector::createVector(2, data12, pLogger);


    double* data21 = new double[2]{ 2, 1 };
    double* data22 = new double[2]{ 3, 2 };
    IVector* pOp21 = IVector::createVector(2, data21, pLogger);
    IVector* pOp22 = IVector::createVector(2, data22, pLogger);
    ICompact* left = ICompact::createCompact(pOp1, pOp2, pLogger);
    ICompact* right = ICompact::createCompact(pOp21, pOp22, pLogger);

    bool resultF;
    RESULT_CODE res = left->isIntersects(right, resultF);
    if (res != RESULT_CODE::SUCCESS || !resultF) {
        status = false;
    }


    delete[] data22;
    delete pOp22;
    delete right;

    /* intersects by point in line */
    data22 = new double[2]{ 5, 1 };
    pOp22 = IVector::createVector(2, data22, pLogger);
    right = ICompact::createCompact(pOp21, pOp22, pLogger);
    res = left->isIntersects(right, resultF);
    if (res != RESULT_CODE::SUCCESS || !resultF) {
        status = false;
    }


    delete[] data21;
    delete[] data22;
    delete pOp21;
    delete pOp22;
    delete right;
    /* does not intersect */
    data21 = new double[2]{ 5, 5 };
    data22 = new double[2]{ 10, 10 };
    pOp21 = IVector::createVector(2, data21, pLogger);
    pOp22 = IVector::createVector(2, data22, pLogger);
    right = ICompact::createCompact(pOp21, pOp22, pLogger);
    res = left->isIntersects(right, resultF);
    if (res != RESULT_CODE::SUCCESS || resultF) {
        status = false;
    }

    delete[] data21;
    delete[] data22;
    delete pOp21;
    delete pOp22;
    delete right;
    /* 1 common point*/
    data21 = new double[2]{ 3, 3 };
    data22 = new double[2]{ 4, 5 };
    pOp21 = IVector::createVector(2, data21, pLogger);
    pOp22 = IVector::createVector(2, data22, pLogger);
    right = ICompact::createCompact(pOp21, pOp22, pLogger);
    res = left->isIntersects(right, resultF);
    if (res != RESULT_CODE::SUCCESS || !resultF) {
        status = false;
    }

    delete[] data11;
    delete[] data12;
    delete[] data21;
    delete[] data22;
    delete pOp1;
    delete pOp2;
    delete pOp21;
    delete pOp22;

    delete left;
    delete right;
    return status;
}

bool testIntersection(ILogger* pLogger) {
    bool status = true;

    /* simple 2D intersection */
    double* data11 = new double[2]{ 0, 1 };
    double* data12 = new double[2]{ 3, 3 };
    IVector* pOp1 = IVector::createVector(2, data11, pLogger);
    IVector* pOp2 = IVector::createVector(2, data12, pLogger);


    double* data21 = new double[2]{ 2, 0 };
    double* data22 = new double[2]{ 5, 2 };
    IVector* pOp12 = IVector::createVector(2, data21, pLogger);
    IVector* pOp22 = IVector::createVector(2, data22, pLogger);
    ICompact* left = ICompact::createCompact(pOp1, pOp2, pLogger);
    ICompact* right = ICompact::createCompact(pOp12, pOp22, pLogger);

    ICompact* result = ICompact::intersection(left, right, pLogger);

    if (result == nullptr)
        status = false;
    else {
        IVector* begin = result->getBegin();
        IVector* end = result->getEnd();
        if (begin->getCoord(0) != 2 || begin->getCoord(1) != 1)
            status = false;
        if (end ->getCoord(0) != 3 || end->getCoord(1) != 2)
            status = false;
        delete begin;
        delete end;
    }



    delete[] data21;
    delete[] data22;
    delete pOp12;
    delete pOp22;
    delete right;
    delete result;
    /* 2D intersection with another side */
    data21 = new double[2]{ 2, 2 };
    data22 = new double[2]{ 5, 4 };
    pOp12 = IVector::createVector(2, data21, pLogger);
    pOp22 = IVector::createVector(2, data22, pLogger);
    right = ICompact::createCompact(pOp12, pOp22, pLogger);
    result = ICompact::intersection(left, right, pLogger);
    if (result == nullptr)
        status = false;
    else {
        IVector* begin = result->getBegin();
        IVector* end = result->getEnd();
        if (begin->getCoord(0) != 2 || begin->getCoord(1) != 2)
            status = false;
        if (end->getCoord(0) != 3 || end->getCoord(1) != 3)
            status = false;
        delete begin;
        delete end;
    }


    delete[] data21;
    delete[] data22;
    delete pOp12;
    delete pOp22;
    delete right;
    delete result;
    /* 2D intersection in 1 point*/
    data21 = new double[2]{ 3, 3 };
    data22 = new double[2]{ 5, 5 };
    pOp12 = IVector::createVector(2, data21, pLogger);
    pOp22 = IVector::createVector(2, data22, pLogger);
    right = ICompact::createCompact(pOp12, pOp22, pLogger);
    result = ICompact::intersection(left, right, pLogger);
    if (result == nullptr)
        status = false;
    else {
        IVector* begin = result->getBegin();
        IVector* end = result->getEnd();
        if (begin->getCoord(0) != 3 || begin->getCoord(1) != 3)
            status = false;
        if (end->getCoord(0) != 3 || end->getCoord(1) != 3)
            status = false;
        delete begin;
        delete end;
    }



    delete[] data11;
    delete[] data12;
    delete[] data21;
    delete[] data22;

    delete pOp1;
    delete pOp2;
    delete pOp12;
    delete pOp22;

    delete left;
    delete right;
    delete result;
    /* 3D simple intersection */
    data11 = new double[3]{ 0, 0, 0 };
    data12 = new double[3]{ 2, 2, 2 };
    pOp1 = IVector::createVector(3, data11, pLogger);
    pOp2 = IVector::createVector(3, data12, pLogger);


    data21 = new double[3]{ 1, 1, 1 };
    data22 = new double[3]{ 3, 3, 3 };
    pOp12 = IVector::createVector(3, data21, pLogger);
    pOp22 = IVector::createVector(3, data22, pLogger);
    left = ICompact::createCompact(pOp1, pOp2, pLogger);
    right = ICompact::createCompact(pOp12, pOp22, pLogger);

    result = ICompact::intersection(left, right, pLogger);

    if (result == nullptr)
        status = false;
    else {
        IVector* begin = result->getBegin();
        IVector* end = result->getEnd();
        if (begin->getCoord(0) != 1 || begin->getCoord(1) != 1 || begin->getCoord(2) != 1)
            status = false;
        if (end->getCoord(0) != 2 || end->getCoord(1) != 2 || end->getCoord(2) != 2)
            status = false;
        delete begin;
        delete end;
    }




    delete[] data12;
    delete[] data21;
    delete[] data22;


    delete pOp2;
    delete pOp12;
    delete pOp22;

    delete left;
    delete right;
    delete result;
    /* 3D with intersection in full edge */
    data12 = new double[3]{ 1, 1, 1 };
    pOp2 = IVector::createVector(3, data12, pLogger);
    left = ICompact::createCompact(pOp1, pOp2, pLogger);
    data21 = new double[3]{ 1, 0, 0 };
    data22 = new double[3]{ 2, 1, 1 };
    pOp12 = IVector::createVector(3, data21, pLogger);
    pOp22 = IVector::createVector(3, data22, pLogger);
    right = ICompact::createCompact(pOp12, pOp22, pLogger);
    result = ICompact::intersection(left, right, pLogger);

    if (result == nullptr)
        status = false;
    else {
        IVector* begin = result->getBegin();
        IVector* end = result->getEnd();
        if (begin->getCoord(0) != 1 || begin->getCoord(1) != 0 || begin->getCoord(2) != 0)
            status = false;
        if (end->getCoord(0) != 1 || end->getCoord(1) != 1 || end->getCoord(2) != 1)
            status = false;
        delete begin;
        delete end;
    }





    delete[] data21;
    delete[] data22;


    delete pOp12;
    delete pOp22;

    delete right;
    delete result;
    /* 3D intersection another side */
    data21 = new double[3]{ -1, 0, 0 };
    data22 = new double[3]{ 1.75, 0.5, 0.5 };
    pOp12 = IVector::createVector(3, data21, pLogger);
    pOp22 = IVector::createVector(3, data22, pLogger);
    right = ICompact::createCompact(pOp12, pOp22, pLogger);
    result = ICompact::intersection(left, right, pLogger);

    if (result == nullptr)
        status = false;
    else {
        IVector* begin = result->getBegin();
        IVector* end = result->getEnd();
        if (begin->getCoord(0) != 0 || begin->getCoord(1) != 0 || begin->getCoord(2) != 0)
            status = false;
        if (end->getCoord(0) != 1 || end->getCoord(1) != 0.5 || end->getCoord(2) != 0.5)
            status = false;
        delete begin;
        delete end;
    }


    delete[] data11;
    delete[] data12;
    delete[] data21;
    delete[] data22;

    delete pOp1;
    delete pOp2;
    delete pOp12;
    delete pOp22;

    delete left;
    delete right;
    delete result;
    return status;
}

bool testAdd(ILogger* pLogger) {
    bool status = true;
    /* simple 2D add false */
    double* data11 = new double[2]{ 0, 1 };
    double* data12 = new double[2]{ 3, 3 };
    IVector* pOp1 = IVector::createVector(2, data11, pLogger);
    IVector* pOp2 = IVector::createVector(2, data12, pLogger);
    double* data21 = new double[2]{ 2, 0 };
    double* data22 = new double[2]{ 5, 2 };
    IVector* pOp12 = IVector::createVector(2, data21, pLogger);
    IVector* pOp22 = IVector::createVector(2, data22, pLogger);
    ICompact* left = ICompact::createCompact(pOp1, pOp2, pLogger);
    ICompact* right = ICompact::createCompact(pOp12, pOp22, pLogger);
    ICompact* result = ICompact::add(left, right, pLogger);
    if (result != nullptr)
        status = false;



    delete[] data21;
    delete[] data22;
    delete pOp12;
    delete pOp22;
    delete right;
    delete result;
    /* simple 2D add correct */
    data21 = new double[2]{ 3, 1 };
    data22 = new double[2]{ 6, 3 };
    pOp12 = IVector::createVector(2, data21, pLogger);
    pOp22 = IVector::createVector(2, data22, pLogger);
    right = ICompact::createCompact(pOp12, pOp22, pLogger);
    result = ICompact::add(left, right, pLogger);
    if (result == nullptr)
        status = false;
    else {
        IVector* begin = result->getBegin();
        IVector* end = result->getEnd();
        if (begin->getCoord(0) != 0 || begin->getCoord(1) != 1)
            status = false;
        if (end->getCoord(0) != 6 || end->getCoord(1) != 3)
            status = false;
        delete begin;
        delete end;
    }


    delete[] data21;
    delete[] data22;
    delete pOp12;
    delete pOp22;
    delete right;
    delete result;
    /* simple 2D add correct */
    data21 = new double[2]{ 2, 1 };
    data22 = new double[2]{ 6, 3 };
    pOp12 = IVector::createVector(2, data21, pLogger);
    pOp22 = IVector::createVector(2, data22, pLogger);
    right = ICompact::createCompact(pOp12, pOp22, pLogger);
    result = ICompact::add(left, right, pLogger);
    if (result == nullptr)
        status = false;
    else {
        IVector* begin = result->getBegin();
        IVector* end = result->getEnd();
        if (begin->getCoord(0) != 0 || begin->getCoord(1) != 1)
            status = false;
        if (end->getCoord(0) != 6 || end->getCoord(1) != 3)
            status = false;
        delete begin;
        delete end;
    }

    delete[] data21;
    delete[] data22;
    delete pOp12;
    delete pOp22;
    delete right;
    delete result;
    /* 2D test add incorrect - 1 same point*/
    data21 = new double[2]{ 3, 3 };
    data22 = new double[2]{ 6, 6 };
    pOp12 = IVector::createVector(2, data21, pLogger);
    pOp22 = IVector::createVector(2, data22, pLogger);
    right = ICompact::createCompact(pOp12, pOp22, pLogger);
    result = ICompact::add(left, right, pLogger);
    if (result != nullptr)
        status = false;


    delete[] data11;
    delete[] data12;
    delete[] data21;
    delete[] data22;
    delete pOp1;
    delete pOp2;
    delete pOp12;
    delete pOp22;
    delete left;
    delete right;
    delete result;
    /* simple 3D add failed */
    data11 = new double[3]{ 0, 0, 0 };
    data12 = new double[3]{ 2, 2, 2 };
    pOp1 = IVector::createVector(3, data11, pLogger);
    pOp2 = IVector::createVector(3, data12, pLogger);
    data21 = new double[3]{ 1, 1, 1 };
    data22 = new double[3]{ 3, 3, 3 };
    pOp12 = IVector::createVector(3, data21, pLogger);
    pOp22 = IVector::createVector(3, data22, pLogger);
    left = ICompact::createCompact(pOp1, pOp2, pLogger);
    right = ICompact::createCompact(pOp12, pOp22, pLogger);
    result = ICompact::add(left, right, pLogger);
    if (result != nullptr)
        status = false;


    delete[] data12;
    delete[] data21;
    delete[] data22;
    delete pOp2;
    delete pOp12;
    delete pOp22;
    delete left;
    delete right;
    delete result;
    /* 3D with add in full edge */
    data12 = new double[3]{ 1, 1, 1 };
    pOp2 = IVector::createVector(3, data12, pLogger);
    left = ICompact::createCompact(pOp1, pOp2, pLogger);
    data21 = new double[3]{ 1, 0, 0 };
    data22 = new double[3]{ 2, 1, 1 };
    pOp12 = IVector::createVector(3, data21, pLogger);
    pOp22 = IVector::createVector(3, data22, pLogger);
    right = ICompact::createCompact(pOp12, pOp22, pLogger);
    result = ICompact::add(left, right, pLogger);
    if (result == nullptr)
        status = false;
    else {
        IVector* begin = result->getBegin();
        IVector* end = result->getEnd();
        if (begin->getCoord(0) != 0 || begin->getCoord(1) != 0 || begin->getCoord(2) != 0)
            status = false;
        if (end->getCoord(0) != 2 ||  end->getCoord(1) != 1 || end->getCoord(2) != 1)
            status = false;
        delete begin;
        delete end;
    }
    delete[] data11;
    delete[] data12;
    delete[] data21;
    delete[] data22;
    delete pOp1;
    delete pOp2;
    delete pOp12;
    delete pOp22;
    delete left;
    delete right;
    delete result;
    return status;
}

bool testMakeConvex(ILogger* pLogger) {
    bool status = true;
    /* simple 2D makeConvex */
    double* data11 = new double[2]{ 0, 1 };
    double* data12 = new double[2]{ 3, 3 };
    IVector* pOp1 = IVector::createVector(2, data11, pLogger);
    IVector* pOp2 = IVector::createVector(2, data12, pLogger);
    double* data21 = new double[2]{ 2, 0 };
    double* data22 = new double[2]{ 5, 2 };
    IVector* pOp12 = IVector::createVector(2, data21, pLogger);
    IVector* pOp22 = IVector::createVector(2, data22, pLogger);
    ICompact* left = ICompact::createCompact(pOp1, pOp2, pLogger);
    ICompact* right = ICompact::createCompact(pOp12, pOp22, pLogger);
    ICompact* result = ICompact::makeConvex(left, right, pLogger);

    if (result == nullptr)
        status = false;
    else {
        IVector* begin = result->getBegin();
        IVector* end = result->getEnd();
        if (begin->getCoord(0) != 0 || begin->getCoord(1) != 0)
            status = false;
        if (end->getCoord(0) != 5 || end->getCoord(1) != 3)
            status = false;
        delete begin;
        delete end;
    }



    delete[] data21;
    delete[] data22;
    delete pOp12;
    delete pOp22;
    delete right;
    delete result;
    /* simple 2D makeConvex correct */
    data21 = new double[2]{ 2, 1 };
    data22 = new double[2]{ 6, 3 };
    pOp12 = IVector::createVector(2, data21, pLogger);
    pOp22 = IVector::createVector(2, data22, pLogger);
    right = ICompact::createCompact(pOp12, pOp22, pLogger);
    result = ICompact::makeConvex(left, right, pLogger);
    if (result == nullptr)
        status = false;
    else {
        IVector* begin = result->getBegin();
        IVector* end = result->getEnd();
        if (begin->getCoord(0) != 0 || begin->getCoord(1) != 1)
            status = false;
        if (end->getCoord(0) != 6 || end->getCoord(1) != 3)
            status = false;
        delete end;
        delete begin;
    }

    delete[] data11;
    delete[] data12;
    delete[] data21;
    delete[] data22;
    delete pOp1;
    delete pOp2;
    delete pOp12;
    delete pOp22;
    delete left;
    delete right;
    delete result;
    /* 3D test makeConvex*/
    data11 = new double[3]{ 0, 0, 0 };
    data12 = new double[3]{ 2, 2, 2 };
    pOp1 = IVector::createVector(3, data11, pLogger);
    pOp2 = IVector::createVector(3, data12, pLogger);
    data21 = new double[3]{ 1, 1, 1 };
    data22 = new double[3]{ 3, 3, 3 };
    pOp12 = IVector::createVector(3, data21, pLogger);
    pOp22 = IVector::createVector(3, data22, pLogger);
    left = ICompact::createCompact(pOp1, pOp2, pLogger);
    right = ICompact::createCompact(pOp12, pOp22, pLogger);
    result = ICompact::makeConvex(left, right, pLogger);
    if (result == nullptr)
        status = false;
    else {
        IVector* begin = result->getBegin();
        IVector* end = result->getEnd();
        if (begin->getCoord(0) != 0 || begin->getCoord(1) != 0 || begin->getCoord(2) != 0)
            status = false;
        if (end->getCoord(0) != 3 || end->getCoord(1) != 3 || end->getCoord(2) != 3)
            status = false;
        delete begin;
        delete end;
    }


    delete[] data11;
    delete[] data12;
    delete[] data21;
    delete[] data22;

    delete pOp1;
    delete pOp2;
    delete pOp12;
    delete pOp22;

    delete left;
    delete right;
    delete result;
    return status;
}

bool testCreateIterator(ILogger* pLogger) {
    bool status = true;
    double* data11 = new double[2]{ 0, 1 };
    double* data12 = new double[2]{ 3, 3 };
    IVector* pOp1 = IVector::createVector(2, data11, pLogger);
    IVector* pOp2 = IVector::createVector(2, data12, pLogger);

    ICompact* result = ICompact::createCompact(pOp1, pOp2, pLogger);

    double* step_data = new double[2]{ 0.1, 0.2 };
    IVector* step = IVector::createVector(2, step_data, pLogger);
    ICompact::iterator* iterator = result->begin(step);
    if (iterator == nullptr)
        status = false;

    delete[] step_data;
    delete step;
    delete iterator;
    step_data = new double[2]{ -0.5, -0.8 };
    step = IVector::createVector(2, step_data, pLogger);
    iterator = result->end(step);
    if (iterator == nullptr)
        status = false;

    delete[] step_data;
    delete step;
    delete iterator;
    step = nullptr;
    iterator = result->end(step);
    if (iterator != nullptr)
        status = false;

    delete step;
    delete iterator;
    step_data = new double[2]{ NAN, 0 };
    step = IVector::createVector(2, step_data, pLogger);
    iterator = result->end(step);
    if (iterator != nullptr)
        status = false;

    delete step;
    delete iterator;
    delete[] step_data;
    step_data = new double[4]{ 1, 1, 2, 1 };
    step = IVector::createVector(4, step_data, pLogger);
    iterator = result->end(step);
    if (iterator != nullptr)
        status = false;



    delete[] data11;
    delete[] data12;
    delete pOp1;
    delete pOp2;
    delete result;
    delete step;
    delete iterator;
    delete[] step_data;
    return status;
}

bool testSetDirection(ILogger* pLogger) {
    bool status = true;
    double* data11 = new double[2]{ 0, 1 };
    double* data12 = new double[2]{ 3, 3 };
    IVector* pOp1 = IVector::createVector(2, data11, pLogger);
    IVector* pOp2 = IVector::createVector(2, data12, pLogger);

    ICompact* result = ICompact::createCompact(pOp1, pOp2, pLogger);

    double* step_data = new double[2]{ 2, 1 };
    IVector* step = IVector::createVector(2, step_data, pLogger);
    ICompact::iterator* iterator = result->begin(step);
    IVector* dir = IVector::createVector(2, nullptr, pLogger);
    RESULT_CODE res = iterator->setDirection(dir);
    if (res == RESULT_CODE::SUCCESS)
        status = false;

    delete dir;
    double* dir_data = new double[2]{ 0, NAN };
    dir = IVector::createVector(2, dir_data, pLogger);
    res = iterator->setDirection(dir);
    if (res == RESULT_CODE::SUCCESS)
        status = false;

    delete[] dir_data;
    delete dir;
    dir_data = new double[2]{ 0, -1 };
    dir = IVector::createVector(2, dir_data, pLogger);
    res = iterator->setDirection(dir);
    if (res == RESULT_CODE::SUCCESS) {
        status = false;
    }

    delete[] dir_data;
    delete dir;
    delete[] data11;
    delete[] data12;
    delete pOp1;
    delete pOp2;
    delete result;
    delete step;
    delete iterator;
    delete[] step_data;
    return status;
}


bool testIterator(ILogger* pLogger) {
    bool status = true;
    double* data11 = new double[2]{ 0, 1 };
    double* data12 = new double[2]{ 3, 3 };
    IVector* pOp1 = IVector::createVector(2, data11, pLogger);
    IVector* pOp2 = IVector::createVector(2, data12, pLogger);

    ICompact* result = ICompact::createCompact(pOp1, pOp2, pLogger);

    double* step_data = new double[2]{ 0.5, 0.2 };
    IVector* step = IVector::createVector(2, step_data, pLogger);
    ICompact::iterator* iterator = result->begin(step);
    double* dir_data = new double[2]{ 0, 1 };
    IVector* dir = IVector::createVector(2, dir_data, pLogger);
    RESULT_CODE res = iterator->setDirection(dir);

    RESULT_CODE fin = iterator->doStep();
    if (fin != RESULT_CODE::SUCCESS) {
        status = false;
    }


    while (iterator->doStep() == RESULT_CODE::SUCCESS) {
        //IVector* point = iterator->getPoint();
        //printVector(point);
        //delete point;
        //std::cout << std::endl;
    }
    IVector* point = iterator->getPoint();
    if (std::fabs(point->getCoord(0)-3) > 1e-5 || std::fabs(point->getCoord(1) - 3) > 1e-5)
        status = false;
    delete point;

    delete[] step_data;
    delete step;
    delete iterator;
    step_data = new double[2]{ -0.2, -0.2 };
    step = IVector::createVector(2, step_data, pLogger);
    iterator = result->end(step);
    res = iterator->setDirection(dir);
    fin = iterator->doStep();
    if (fin != RESULT_CODE::SUCCESS) {
        status = false;
    }

    delete[] dir_data;
    delete dir;
    delete[] data11;
    delete[] data12;
    delete pOp1;
    delete pOp2;
    delete result;
    delete step;
    delete iterator;
    delete[] step_data;
    return status;
}
void runAllTest(ILogger* pLogger) {
    bool test = testCreate(pLogger);
    if (test)
        std::cout << "CREATE COMPACT TEST| OK\n";
    else
        std::cout << "CREATE COMPACT TEST| FAILED\n";

    test = testClone(pLogger);
    if (test)
        std::cout << "CLONE COMPACT TEST| OK\n";
    else
        std::cout << "CLONE COMPACT TEST| FAILED\n";

    test = testIsContains(pLogger);
    if (test)
        std::cout << "ISCONTAINS COMPACT TEST| OK\n";
    else
        std::cout << "ISCONTAINS COMPACT TEST| FAILED\n";

    test = testIsSubset(pLogger);
    if (test)
        std::cout << "ISSUBSET COMPACT TEST| OK\n";
    else
        std::cout << "ISSUBSET COMPACT TEST| FAILED\n";

    test = testIsIntersects(pLogger);
    if (test)
        std::cout << "ISINTERSECTS COMPACT TEST| OK\n";
    else
        std::cout << "ISINTERSECTS COMPACT TEST| FAILED\n";

    test = testIntersection(pLogger);
    if (test)
        std::cout << "INTERSECTION COMPACT TEST| OK\n";
    else
        std::cout << "INTERSECTION COMPACT TEST| FAILED\n";

    test = testAdd(pLogger);
    if (test)
        std::cout << "ADD COMPACT TEST| OK\n";
    else
        std::cout << "ADD COMPACT TEST| FAILED\n";

    test = testMakeConvex(pLogger);
    if (test)
        std::cout << "MAKE CONVEX COMPACT TEST| OK\n";
    else
        std::cout << "MAKE CONVEX COMPACT TEST| FAILED\n";


    test = testCreateIterator(pLogger);
    if (test)
        std::cout << "CREATE ITERATOR COMPACT TEST| OK\n";
    else
        std::cout << "CREATE ITERATOR COMPACT TEST| FAILED\n";

    test = testSetDirection(pLogger);
    if (test)
        std::cout << "DIRECTION COMPACT TEST| OK\n";
    else
        std::cout << "DIRECTION COMPACT TEST| FAILED\n";
    test = testIterator(pLogger);
    if (test)
        std::cout << "ITERATOR COMPACT TEST| OK\n";
    else
        std::cout << "ITERATOR COMPACT TEST| FAILED\n";

}
