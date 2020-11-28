#include "../include/IVector.h"
#include "ILogger.h"

#include <cmath>
#include <string>

class MyVector: public IVector {
public:
    MyVector() {}
    MyVector(size_t dim, double * pData, ILogger *pLogger) {
        dim_ = dim;
        data_ = pData;
        logger_ = pLogger;
    }
    virtual IVector* clone() const override;

    double getCoord(size_t index)const override;
    RESULT_CODE setCoord(size_t index, double value) override;
    double norm(NORM norm)const override;
    size_t getDim() const override;

    ~MyVector() {}

private:
    double * data_;
    size_t dim_;
    ILogger* logger_;
};

static void logWithCheck(ILogger * pLogger, char const* pMsg, RESULT_CODE err) {
    if (pLogger != nullptr) {
        pLogger->log(pMsg, err);
    }
}

IVector::~IVector(){}

IVector *IVector::createVector(size_t dim, double *pData, ILogger *pLogger) {
    if (!pData || dim <= 0) {
        logWithCheck(pLogger, "createVector: Wrong parameters", RESULT_CODE::WRONG_ARGUMENT);
        return nullptr;
    }
    for (size_t i = 0; i < dim; ++i) {
        if (std::isnan(pData[i])) {
            logWithCheck(pLogger, "createVector: Nan value", RESULT_CODE::NAN_VALUE);
            return nullptr;
        }
    }
    size_t size = sizeof(MyVector) + dim * sizeof(double);
    void* ptr = new(std::nothrow)unsigned char[size];
    if (!ptr) {
        logWithCheck(pLogger, "createVector: Nan value", RESULT_CODE::OUT_OF_MEMORY);
        return nullptr;
    }
    MyVector* pVector = new(ptr)MyVector(dim,
        reinterpret_cast<double*>((unsigned char*)ptr + sizeof(MyVector)), pLogger);
    memcpy((unsigned char*)ptr + sizeof(MyVector), pData, dim * sizeof(double));
    return pVector;
}



IVector *MyVector::clone() const {
    return createVector(dim_, const_cast<double*>(&data_[0]), logger_);
}

IVector *IVector::add(const IVector *pOperand1, const IVector *pOperand2, ILogger *pLogger){
    if (pOperand1 == nullptr || pOperand2 == nullptr) {
        logWithCheck(pLogger, "add: different dims", RESULT_CODE::WRONG_ARGUMENT);
        return nullptr;
    }
    if (pOperand1->getDim() != pOperand2->getDim()) {
        logWithCheck(pLogger, "add: different dims", RESULT_CODE::WRONG_DIM);
        return nullptr;
    }
    double * buff;
    try {
        buff = new double[pOperand1->getDim()];
    } catch (const std::bad_alloc &) {
        logWithCheck(pLogger, "add: Not enough memory", RESULT_CODE::OUT_OF_MEMORY);
        return nullptr;
    }
    for (size_t i = 0; i < pOperand1->getDim(); ++i) {
        buff[i] = pOperand1->getCoord(i) + pOperand2->getCoord(i);
    }
    IVector * result = createVector(pOperand1->getDim(), buff, pLogger);
    delete [] buff;

    return result;
}

IVector *IVector::sub(const IVector *pOperand1, const IVector *pOperand2, ILogger *pLogger){
    if (pOperand1 == nullptr || pOperand2 == nullptr) {
        logWithCheck(pLogger, "sub: different dims", RESULT_CODE::WRONG_ARGUMENT);
        return nullptr;
    }
    if (pOperand1->getDim() != pOperand2->getDim()) {
        logWithCheck(pLogger, "sub: different dims", RESULT_CODE::WRONG_DIM);
        return nullptr;
    }
    double * buff;
    try {
        buff = new double[pOperand1->getDim()];
    } catch (const std::bad_alloc &) {
        logWithCheck(pLogger, "sub: Not enough memory", RESULT_CODE::OUT_OF_MEMORY);
        return nullptr;
    }
    for (size_t i = 0; i < pOperand1->getDim(); ++i) {
        buff[i] = pOperand1->getCoord(i) - pOperand2->getCoord(i);
        if (std::isnan(buff[i])) {
            logWithCheck(pLogger, "sub: nan res", RESULT_CODE::NAN_VALUE);
            return nullptr;
        }
    }
    IVector * result = createVector(pOperand1->getDim(), buff, pLogger);
    delete [] buff;

    return result;
}

IVector *IVector::mul(const IVector *pOperand1, double scaleParam, ILogger *pLogger) {
    if (pOperand1 == nullptr) {
        logWithCheck(pLogger, "Mul to number: different dims", RESULT_CODE::WRONG_ARGUMENT);
        return nullptr;
    }
    if (std::isnan(scaleParam)) {
        logWithCheck(pLogger, "Mul to number: nan scale parametr", RESULT_CODE::NAN_VALUE);
        return nullptr;
    }

    double * buff;
    try {
        buff = new double[pOperand1->getDim()];
    } catch (const std::bad_alloc &) {
        logWithCheck(pLogger, "sub: Not enough memory", RESULT_CODE::OUT_OF_MEMORY);
        return nullptr;
    }
    for (size_t i = 0; i < pOperand1->getDim(); ++i) {
        buff[i] = pOperand1->getCoord(i) * scaleParam;
        if (std::isnan(buff[i])) {
            logWithCheck(pLogger, "Mul to number: nan res", RESULT_CODE::NAN_VALUE);
            return nullptr;
        }
    }
    IVector * result = createVector(pOperand1->getDim(), buff, pLogger);
    delete [] buff;

    return result;
}

double IVector::mul(const IVector *pOperand1, const IVector *pOperand2, ILogger *pLogger){
    if (pOperand1 == nullptr || pOperand2 == nullptr) {
        logWithCheck(pLogger, "Mul to vec: different dims", RESULT_CODE::WRONG_ARGUMENT);
        return NAN;
    }
    if (pOperand1->getDim() != pOperand2->getDim()) {
        logWithCheck(pLogger, "Mul to vec: different dims", RESULT_CODE::WRONG_DIM);
        return NAN;
    }
    double res = 0;
    for (size_t i = 0; i < pOperand1->getDim(); ++i) {
        res += pOperand1->getCoord(i) * pOperand2->getCoord(i);
    }
    if (std::isnan(res)) {
        logWithCheck(pLogger, "Mul to vec: nan res", RESULT_CODE::NAN_VALUE);
    }
    return res;
}

RESULT_CODE IVector::equals(const IVector *pOperand1, const IVector *pOperand2, IVector::NORM norm,
                              double tolerance, bool *result, ILogger *pLogger){

    if (pOperand1 == nullptr || pOperand2 == nullptr) {
        logWithCheck(pLogger, "Equals: different dims", RESULT_CODE::WRONG_ARGUMENT);
        return RESULT_CODE::WRONG_ARGUMENT;
    }
    if (pOperand1->getDim() != pOperand2->getDim()) {
        logWithCheck(pLogger, "Equals: different dims", RESULT_CODE::WRONG_DIM);
        return RESULT_CODE::WRONG_DIM;
    }

    double norm1 = pOperand1->norm(norm);
    double norm2 = pOperand2->norm(norm);
    if (std::isnan(norm1) || std::isnan(norm2)) {
        logWithCheck(pLogger, "Equals: calculation", RESULT_CODE::CALCULATION_ERROR);
        return RESULT_CODE::CALCULATION_ERROR;
    }
    double delta = std::fabs(norm1 - norm2);
    if (delta < tolerance) {
        *result = true;
    }
    else {
        *result = false;
    }
    return RESULT_CODE::SUCCESS;
}

double MyVector::getCoord(size_t index) const {
    if (index >= dim_){
        logWithCheck(logger_, "getCoord: out of index", RESULT_CODE::WRONG_ARGUMENT);
        return NAN;
    }
    return data_[index];
}

RESULT_CODE MyVector::setCoord(size_t index, double value) {
    if (index >= dim_){
        logWithCheck(logger_, "setCoord: out of index", RESULT_CODE::WRONG_ARGUMENT);
        return RESULT_CODE::WRONG_ARGUMENT;
    }
    if (std::isnan(value)) {
        logWithCheck(logger_, "setCoord: nan value arg", RESULT_CODE::NAN_VALUE);
        return RESULT_CODE::NAN_VALUE;
    }
    data_[index] = value;
    return RESULT_CODE::SUCCESS;
}

double MyVector::norm(IVector::NORM norm) const{
    switch (norm) {
        case NORM::NORM_1: {
            double sum = 0;
            for (size_t i = 0; i < dim_; i++)
                sum += std::fabs(data_[i]);
            return sum;
        }
        case NORM::NORM_2: {
            double sum2 = 0;
            for (size_t i = 0; i < dim_; i++)
                sum2 += std::fabs(data_[i]) * std::fabs(data_[i]);
            return std::sqrt(sum2);
        }
        case NORM::NORM_INF: {
            double max = data_[0];
            for (size_t i = 0; i < dim_; i++)
                if (data_[i] > max)
                    max = data_[i];
            return max;
        }
    }
    logWithCheck(logger_, "norm: out of index", RESULT_CODE::WRONG_ARGUMENT);
    return NAN;
}

size_t MyVector::getDim() const {
    return dim_;
}
