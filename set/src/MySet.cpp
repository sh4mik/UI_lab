#include "../include/ISet.h"
#include <vector>
#include <cmath>

static RESULT_CODE logWithCheckAndReturn(ILogger * pLogger, char const* pMsg, RESULT_CODE err);

namespace  {

class MySet :public ISet {
public:
    MySet(ILogger *pLogger): logger_(pLogger) {}
    ~MySet() {
        for (auto vec: data_) {
            delete vec;
        }
        data_.clear();
    }

    RESULT_CODE insert(const IVector* pVector, IVector::NORM norm, double tolerance) override {
        if (pVector == nullptr) {
            return logWithCheckAndReturn(logger_, "insert: ", RESULT_CODE::CALCULATION_ERROR);
        }
        if (tolerance < 0) {
            return logWithCheckAndReturn(logger_, "insert:", RESULT_CODE::CALCULATION_ERROR);
        }
        if (std::isnan(tolerance)) {
            return logWithCheckAndReturn(logger_, "insert:", RESULT_CODE::NAN_VALUE);
        }
        if (data_.empty()) {
            data_.push_back(pVector->clone());
            return RESULT_CODE::SUCCESS;
        }
        if (pVector->getDim() != getDim()) {
            return logWithCheckAndReturn(logger_, "insert:", RESULT_CODE::WRONG_DIM);
        }
        // TODO
        bool result = false;
        RESULT_CODE res = RESULT_CODE::SUCCESS;
        for (size_t i = 0; i < data_.size(); i++) {
            res = IVector::equals(pVector, data_[i], norm, tolerance, &result, logger_);
            if (res == RESULT_CODE::SUCCESS && result == true) {
                break;
            }
        }
        if (res == RESULT_CODE::SUCCESS && !result) {
            data_.push_back(pVector->clone());
            return RESULT_CODE::SUCCESS;
        }
        return RESULT_CODE::SUCCESS;
    }

    RESULT_CODE get(IVector*& pVector, size_t index)const override {
        pVector = nullptr;
        if (index >= data_.size()) {
            return logWithCheckAndReturn(logger_, "Get by i:", RESULT_CODE::OUT_OF_BOUNDS);
        }
        pVector = nullptr;
        pVector = const_cast<IVector*>(data_[index]->clone());
        return RESULT_CODE::SUCCESS;
    }
    RESULT_CODE get(IVector*& pVector, IVector const* pSample, IVector::NORM norm, double tolerance)const override {
        pVector = nullptr;
        if (!pSample) {
            return logWithCheckAndReturn(logger_, "Get by sample:", RESULT_CODE::WRONG_ARGUMENT);
        }
        if (pSample->getDim() != this->getDim()) {
            return logWithCheckAndReturn(logger_, "Get by sample:", RESULT_CODE::WRONG_DIM);
        }
        if (tolerance < 0) {
            return logWithCheckAndReturn(logger_, "Get by sample:", RESULT_CODE::CALCULATION_ERROR);
        }
        if (std::isnan(tolerance)) {
            return logWithCheckAndReturn(logger_, "Get by sample:", RESULT_CODE::NAN_VALUE);
        }
        for (size_t i = 0; i < data_.size(); i++) {
            IVector* tmp;
            RESULT_CODE res = get(tmp, i);
            if (res != RESULT_CODE::SUCCESS)
                return logWithCheckAndReturn(logger_, "Get by sample:", RESULT_CODE::WRONG_ARGUMENT);
            IVector* sub = IVector::sub(tmp, pSample, logger_);
            double norm_v = sub->norm(norm);
            if (fabs(norm_v) < tolerance) {
                pVector = tmp->clone();
                delete sub;
                delete tmp;
                return RESULT_CODE::SUCCESS;
            }
            delete tmp;
            delete sub;
        }
        return RESULT_CODE::NOT_FOUND;
    }
    size_t getDim() const override { //space dimension
        if (data_.empty()) {
            return 0;
        } else {
            return data_[0]->getDim();
        }
    }

    size_t getSize() const override { //num elements in set
        return data_.size();
    }

    void clear() override { // delete all
        for (auto vec: data_) {
            delete vec;
        }
        data_.clear();
    }

    RESULT_CODE erase(size_t index) override {
        if (index >= data_.size()) {
            return logWithCheckAndReturn(logger_, "erase: ", RESULT_CODE::OUT_OF_BOUNDS);
        }
        auto toDel = data_[index];
        data_.erase(data_.begin() + index);
        delete toDel;
        return RESULT_CODE::SUCCESS;
    }

    RESULT_CODE erase(IVector const* pSample, IVector::NORM norm, double tolerance) override {
        if (!pSample) {
            return logWithCheckAndReturn(logger_, "erase by tol: ", RESULT_CODE::WRONG_ARGUMENT);
        }
        if (tolerance < 0) {
            return logWithCheckAndReturn(logger_, "erase by tol: ", RESULT_CODE::CALCULATION_ERROR);

        }
        if (std::isnan(tolerance)) {
            return logWithCheckAndReturn(logger_, "erase by tol: ", RESULT_CODE::NAN_VALUE);
        }

        size_t i = 0;
        size_t prevSize = data_.size();
        IVector *sub = nullptr;
        for (auto vector : data_) {
            sub = IVector::sub(pSample, vector, logger_);
            if (sub == nullptr) {
              continue;
            }
            if (sub->norm(norm) < tolerance) {
              delete sub;
              delete *(data_.begin() + i);
              data_.erase(data_.begin() + i);
            }
            ++i;
        }
        if (data_.size() == prevSize) {
            return logWithCheckAndReturn(logger_, "erase by tol", RESULT_CODE::NOT_FOUND);
        } else {
            return RESULT_CODE::SUCCESS;
        }

    }
    ISet* clone()const override {
        MySet * res = new MySet(logger_);
        res->data_ = std::vector<const IVector*>();
        for (size_t i = 0; i < data_.size(); i++) {
            res->data_.push_back(const_cast<IVector*>(data_[i]->clone()));
        }
        return res;
    }
private:
    std::vector<const IVector*> data_;
    ILogger * logger_;
};

}

ISet::~ISet() {}

ISet *ISet::createSet(ILogger *pLogger) {
    return new MySet(pLogger);
}

ISet* ISet::add(ISet const* pOperand1, ISet const* pOperand2, IVector::NORM norm, double tolerance, ILogger* pLogger) {
    if (!pOperand1 || !pOperand2) {
        logWithCheckAndReturn(pLogger, "set add: ", RESULT_CODE::WRONG_ARGUMENT);
        return nullptr;
    }
    if (pOperand1->getDim() != pOperand2->getDim()) {
        logWithCheckAndReturn(pLogger, "set add: ", RESULT_CODE::WRONG_DIM);
        return nullptr;
    }
    if (tolerance < 0) {
        logWithCheckAndReturn(pLogger, "set add: ", RESULT_CODE::CALCULATION_ERROR);
        return nullptr;
    }
    if (std::isnan(tolerance)) {
        logWithCheckAndReturn(pLogger, "set add: ", RESULT_CODE::NAN_VALUE);
        return nullptr;
    }
    ISet * result = pOperand1->clone();
    IVector * toInsert = nullptr;
    for (size_t i = 0; i < pOperand2->getSize(); ++i) {
        pOperand2->get(toInsert, i);
        //TODO
        result->insert(toInsert, norm, tolerance);
        delete toInsert;
    }
    return result;
}
ISet* ISet::intersect(ISet const* pOperand1, ISet const* pOperand2, IVector::NORM norm, double tolerance, ILogger* pLogger) {
    if (!pOperand1 || !pOperand2) {
        logWithCheckAndReturn(pLogger, "set intersect: ", RESULT_CODE::WRONG_ARGUMENT);
        return nullptr;
    }
    if (pOperand1->getDim() != pOperand2->getDim()) {
        logWithCheckAndReturn(pLogger, "set intersect: ", RESULT_CODE::WRONG_DIM);
        return nullptr;
    }
    if (tolerance < 0) {
        logWithCheckAndReturn(pLogger, "set intersect: ", RESULT_CODE::CALCULATION_ERROR);
        return nullptr;
    }
    if (std::isnan(tolerance)) {
        logWithCheckAndReturn(pLogger, "set intersect: ", RESULT_CODE::NAN_VALUE);
        return nullptr;
    }
    ISet * result = new MySet(pLogger);
    IVector * toInsert = nullptr;
    IVector * vecToCheck = nullptr;

    for (size_t i = 0; i < pOperand2->getSize(); ++i) {
        pOperand2->get(toInsert, i);
        auto toCheck = pOperand1->get(vecToCheck, toInsert, norm, tolerance);
        if (toCheck == RESULT_CODE::SUCCESS) {
            result->insert(toInsert, norm, tolerance);
            delete vecToCheck;
        }
        delete toInsert;
    }
    return result;
}
ISet* ISet::sub(ISet const* pOperand1, ISet const* pOperand2, IVector::NORM norm, double tolerance, ILogger* pLogger) {
    if (!pOperand1 || !pOperand2) {
        logWithCheckAndReturn(pLogger, "set sub: ", RESULT_CODE::WRONG_ARGUMENT);
        return nullptr;
    }
    if (pOperand1->getDim() != pOperand2->getDim()) {
        logWithCheckAndReturn(pLogger, "set sub: ", RESULT_CODE::WRONG_DIM);
        return nullptr;
    }
    if (tolerance < 0) {
        logWithCheckAndReturn(pLogger, "set sub: ", RESULT_CODE::CALCULATION_ERROR);
        return nullptr;
    }
    if (std::isnan(tolerance)) {
        logWithCheckAndReturn(pLogger, "set sub: ", RESULT_CODE::NAN_VALUE);
        return nullptr;
    }
    ISet * result = pOperand1->clone();
    IVector * toInsert = nullptr;
    IVector * vecToCheck = nullptr;

    for (size_t i = 0; i < pOperand2->getSize(); ++i) {
        pOperand2->get(toInsert, i);
        auto toCheck = pOperand1->get(vecToCheck, toInsert, norm, tolerance);
        if (toCheck == RESULT_CODE::SUCCESS) {
            result->erase(toInsert, norm, tolerance);
            delete vecToCheck;
        }
        delete toInsert;
    }
    return result;
}
ISet* ISet::symSub(ISet const* pOperand1, ISet const* pOperand2, IVector::NORM norm, double tolerance, ILogger* pLogger) {
    if (!pOperand1 || !pOperand2) {
        logWithCheckAndReturn(pLogger, "set symSub: ", RESULT_CODE::WRONG_ARGUMENT);
        return nullptr;
    }
    if (pOperand1->getDim() != pOperand2->getDim()) {
        logWithCheckAndReturn(pLogger, "set symSub: ", RESULT_CODE::WRONG_DIM);
        return nullptr;
    }
    if (tolerance < 0) {
        logWithCheckAndReturn(pLogger, "set symSub: ", RESULT_CODE::CALCULATION_ERROR);
        return nullptr;
    }
    if (std::isnan(tolerance)) {
        logWithCheckAndReturn(pLogger, "set symSub: ", RESULT_CODE::NAN_VALUE);
        return nullptr;
    }

    ISet* setAdd = ISet::add(pOperand1, pOperand2, norm, tolerance, pLogger);
    ISet* setInt = ISet::intersect(pOperand1, pOperand2, norm, tolerance, pLogger);
    ISet* result = ISet::sub(setAdd, setInt, norm, tolerance, pLogger);
    setAdd->clear();
    delete setAdd;
    setInt->clear();
    delete setInt;
    return result;
}

static RESULT_CODE logWithCheckAndReturn(ILogger * pLogger, char const* pMsg, RESULT_CODE err) {
    if (!pLogger) {
        pLogger->log(pMsg, err);
    }
    return err;
}

