#include "../include/ICompact.h"
#include <cmath>

static inline RESULT_CODE logWithCheckAndReturn(ILogger * pLogger, char const* pMsg, RESULT_CODE err);

namespace  {

class MyCompact : public ICompact {
public:
    MyCompact() {
        begin_ = nullptr;
        end_ = nullptr;
        logger_ = nullptr;
    }
    MyCompact(IVector const* const begin, IVector const* const end, ILogger* logger) {
        begin_ = const_cast<IVector*>(begin->clone());
        end_ = const_cast<IVector*>(end->clone());
        logger_ = logger;
    }

    /* returns a step, end, begin with which you can iterate over all domains of compact*/
    IVector* getBegin() const override {
        return begin_->clone();
    }
    IVector* getEnd() const override {
        return end_->clone();
    }

    iterator* end(IVector const* const step = 0) override {
        if (!step)
            return nullptr;
        if (step->getDim() != this->getDim())
            return nullptr;


        for (size_t i = 0; i < step->getDim(); i++) {
            if (std::isnan(step->getCoord(i)))
                return nullptr;
            if (step->getCoord(i) >= 0)
                return nullptr;
        }
        return new myIterator(true, end_, begin_, step);
    }

    iterator* begin(IVector const* const step = 0) override {
        if (!step)
            return nullptr;
        if (step->getDim() != this->getDim())
            return nullptr;

        for (size_t i = 0; i < step->getDim(); i++) {
            if (std::isnan(step->getCoord(i)))
                return nullptr;
            if (step->getCoord(i) <= 0)
                return nullptr;
        }
        return new myIterator(false, begin_, end_, step);
    }

    RESULT_CODE isContains(IVector const* const vec, bool& result) const override {
        if (vec == nullptr)
            return logWithCheckAndReturn(logger_, "MyCompact::isContains", RESULT_CODE::WRONG_ARGUMENT);
        if (vec->getDim() != this->getDim())
            return logWithCheckAndReturn(logger_, "MyCompact::isContains", RESULT_CODE::WRONG_DIM);

        IVector* begin = this->getBegin();
        IVector* end = this->getEnd();
        result = true;
        for (size_t i = 0; i < vec->getDim(); i++) {
            double coord = vec->getCoord(i);

            if (coord < begin->getCoord(i) || coord > end->getCoord(i)) {
                result = false;
                break;
            }
        }
        delete begin;
        delete end;

        return RESULT_CODE::SUCCESS;
    }
    RESULT_CODE isSubSet(ICompact const* const other, bool& result) const override {
        if (other == nullptr)
            return logWithCheckAndReturn(logger_, "MyCompact::isSubSet", RESULT_CODE::WRONG_ARGUMENT);
        if (other->getDim() != this->getDim())
            return logWithCheckAndReturn(logger_, "MyCompact::isSubSet", RESULT_CODE::WRONG_DIM);

        result = true;

        IVector* begin = this->getBegin();
        IVector* end = this->getEnd();
        IVector* begin_other = other->getBegin();
        IVector* end_other = other->getEnd();

        for (size_t i = 0; i < this->getDim(); i++) {
            if (begin_other->getCoord(i) < begin->getCoord(i)) {
                result = false;
                break;
            }
            if (end_other->getCoord(i) > end->getCoord(i)) {
                result = false;
                break;
            }
        }
        delete begin;
        delete end;
        delete begin_other;
        delete end_other;

        return RESULT_CODE::SUCCESS;
    }

    RESULT_CODE isIntersects(ICompact const* const other, bool& result) const override {
        if (other == nullptr)
            return logWithCheckAndReturn(logger_, "MyCompact::isIntersects", RESULT_CODE::WRONG_ARGUMENT);
        if (other->getDim() != this->getDim())
            return logWithCheckAndReturn(logger_, "MyCompact::isIntersects", RESULT_CODE::WRONG_DIM);

        result = false;
        size_t count = 0;
        IVector* begin = this->getBegin();
        IVector* end = this->getEnd();
        IVector* begin_other = other->getBegin();
        IVector* end_other = other->getEnd();

        for (size_t i = 0; i < this->getDim(); i++) {
            double coordBegin = begin_other->getCoord(i);
            double coordEnd = end_other->getCoord(i);
            if (coordBegin >= begin->getCoord(i) && coordBegin <= end->getCoord(i)) {
                count++;
            }
            if (coordEnd >= begin->getCoord(i) && coordEnd <= end->getCoord(i)) {
                count++;
            }
        }
        if (count >= this->getDim()) {
            result = true;
        }
        delete begin;
        delete end;
        delete begin_other;
        delete end_other;

        return RESULT_CODE::SUCCESS;
    }

    size_t getDim() const override {
        return begin_->getDim();
    }
    ICompact* clone() const override {
        return new MyCompact(begin_, end_, logger_);
    }

    /*dtor*/
    ~MyCompact() override {
        delete begin_;
        delete end_;
        begin_ = nullptr;
        end_ = nullptr;
        logger_ = nullptr;
    }

    class myIterator : public iterator {
    public:
        myIterator(bool reverse, IVector const* const start, IVector const* const end, IVector const* const step) {
            start_ = start->clone();
            end_ = end->clone();
            step_ = step->clone();
            reverse_ = reverse;
            cur_ = start->clone();
        }

        //adds step to current value in iterator
        //+step
        RESULT_CODE doStep() override {
            if (!dir_) {
                double* data = new double[step_->getDim()];
                for (size_t i = 0; i < step_->getDim(); i++) {
                    data[i] = i;
                }
                dir_ = IVector::createVector(step_->getDim(), data, nullptr);
                delete[] data;
            }
            if (dir_ == nullptr || step_ == nullptr)
                return RESULT_CODE::WRONG_ARGUMENT;

            int pDir = 0;
            for (size_t i = 0; i < step_->getDim(); i++) {
                double coord = step_->getCoord(i);
                if (reverse_) {
                    if (coord > 0)
                        return RESULT_CODE::WRONG_ARGUMENT;
                } else if (coord < 0) {
                    return RESULT_CODE::WRONG_ARGUMENT;
                }
            }
            while (true) {
                int index = -1;
                size_t size = dir_->getDim();
                for (size_t i = 0; i < size; i++) {
                    if (pDir == dir_->getCoord(i)) {
                        index = i;
                    }
                }
                if (index == -1) {
                    break;
                }
                if (step_->getCoord(index) == 0) {
                    pDir++;
                    continue;
                }
                if (step_->getCoord(index) > 0) {
                    if (std::fabs(cur_->getCoord(index) + step_->getCoord(index) - end_->getCoord(index)) <= TOLERANCE || \
                            cur_->getCoord(index) + step_->getCoord(index) < end_->getCoord(index)) {
                        cur_->setCoord(index, cur_->getCoord(index) + step_->getCoord(index));
                        return RESULT_CODE::SUCCESS;
                    }
                    else {
                        size_t count = 0;
                        for (size_t i = 0; i < step_->getDim(); i++) {
                            if (std::fabs(cur_->getCoord(i) - end_->getCoord(i)) <= TOLERANCE) {
                                count++;
                            }
                        }
                        if (count == step_->getDim())
                            return RESULT_CODE::OUT_OF_BOUNDS;
                        pDir++;
                        cur_->setCoord(index, start_->getCoord(index));
                    }
                }
                else {
                    if (cur_->getCoord(index) + step_->getCoord(index) > end_->getCoord(index) || \
                            std::fabs(cur_->getCoord(index) + step_->getCoord(index) - end_->getCoord(index)) <= TOLERANCE) {
                        cur_->setCoord(index, cur_->getCoord(index) + step_->getCoord(index));
                        return RESULT_CODE::SUCCESS;
                    }
                    else {
                        size_t count = 0;
                        for (size_t i = 0; i < step_->getDim(); i++) {
                            if (std::fabs(cur_->getCoord(i) - end_->getCoord(i)) <= TOLERANCE) {
                                count++;
                            }
                        }
                        if (count == step_->getDim())
                            return RESULT_CODE::OUT_OF_BOUNDS;
                        pDir++;
                        cur_->setCoord(index, start_->getCoord(index));
                    }
                }

            }
            return RESULT_CODE::OUT_OF_BOUNDS;

        }

        IVector* getPoint() const override {
            return cur_->clone();
        }

        //change order of step
        RESULT_CODE setDirection(IVector const* const dir) override {
            if (dir == nullptr)
                return RESULT_CODE::WRONG_ARGUMENT;
            if (dir->getDim() != step_->getDim())
                return RESULT_CODE::WRONG_DIM;

            for (size_t i = 0; i < dir->getDim(); i++) {
                double coord = dir->getCoord(i);
                if (std::isnan(coord)) {
                    return RESULT_CODE::NAN_VALUE;
                }
                if ((coord < 0)) {
                    return RESULT_CODE::WRONG_ARGUMENT;
                }
                for (size_t j = 0; j < dir->getDim(); j++) {
                    if (i != j && std::fabs(dir->getCoord(j) - coord) < TOLERANCE) {
                        return RESULT_CODE::WRONG_ARGUMENT;
                    }
                }
            }
            dir_ = dir->clone();
            return RESULT_CODE::SUCCESS;
        }

        ~myIterator() {
            delete start_;
            delete end_;
            delete step_;
            delete cur_;
            delete dir_;
        }
    private:
        IVector* cur_ = nullptr;
        IVector* dir_ = nullptr;
        IVector* step_;
        IVector* start_;
        IVector* end_;
        bool reverse_;

        constexpr static const double TOLERANCE = 1e-5;
    };

private:
    IVector* begin_;
    IVector* end_;
    ILogger* logger_ = nullptr;
};

}

ICompact::~ICompact() {}

ICompact* ICompact::createCompact(IVector const* const begin, IVector const* const end, ILogger* logger) {
    if (begin == nullptr || end == nullptr) {
        logWithCheckAndReturn(logger, "ICompact::createCompact: ", RESULT_CODE::WRONG_ARGUMENT);
        return nullptr;
    }
    if (begin->getDim() != end->getDim()) {
        logWithCheckAndReturn(logger, "ICompact::createCompact: ", RESULT_CODE::WRONG_DIM);
        return nullptr;
    }

    size_t size = begin->getDim();
    for (size_t i = 0; i < size; i++) {
        if (std::isnan(begin->getCoord(i)) || std::isnan(end->getCoord(i))) {
            logWithCheckAndReturn(logger, "ICompact::createCompact: ", RESULT_CODE::NAN_VALUE);
            return nullptr;
        }
        if (begin->getCoord(i) > end->getCoord(i)) {
            logWithCheckAndReturn(logger, "ICompact::createCompact: ", RESULT_CODE::WRONG_ARGUMENT);
            return nullptr;
        }
    }
    return new MyCompact(begin, end, logger);
}
/*static operations*/
ICompact* ICompact::intersection(ICompact const* const left, ICompact const* const right, ILogger* logger) {
    if (left == nullptr || right == nullptr) {
        logWithCheckAndReturn(logger, "ICompact::createCompact: ", RESULT_CODE::WRONG_ARGUMENT);
        return nullptr;
    }
    if (left->getDim() != right->getDim()) {
        logWithCheckAndReturn(logger, "ICompact::createCompact: ", RESULT_CODE::WRONG_DIM);
        return nullptr;
    }

    bool result;
    ICompact* Res = nullptr;
    //errors are checked there -> no additional checks
    RESULT_CODE res = left->isSubSet(right, result);
    if (res == RESULT_CODE::SUCCESS && result) {
        return right->clone();
    }
    res = right->isSubSet(left, result);
    if (res == RESULT_CODE::SUCCESS && result) {
        return left->clone();
    }
    res = left->isIntersects(right, result);
    //suppose that right and left have correct location
    if (res == RESULT_CODE::SUCCESS && result) {
        size_t size = right->getDim();
        double* dataBegin = new double[size];
        double* dataEnd = new double[size];
        IVector* leftBegin = left->getBegin();
        IVector* leftEnd = left->getEnd();
        IVector* rightBegin = right->getBegin();
        IVector* rightEnd =  right->getEnd();

        for (size_t i = 0; i < size; i++) {
            dataBegin[i] = std::max(leftBegin->getCoord(i), rightBegin->getCoord(i));
            dataEnd[i] = std::min(leftEnd->getCoord(i), rightEnd->getCoord(i));
        }
        delete rightBegin;
        delete rightEnd;
        delete leftBegin;
        delete leftEnd;

        IVector* beginNew = IVector::createVector(size, dataBegin, logger);
        IVector* endNew = IVector::createVector(size, dataEnd, logger);
        Res = new MyCompact(beginNew, endNew, logger);
        delete beginNew;
        delete endNew;
        delete[] dataBegin;
        delete[] dataEnd;
        beginNew = nullptr, endNew = nullptr;
        dataBegin = nullptr, dataEnd = nullptr;
        return Res;
    } else {
        logWithCheckAndReturn(logger, "ICompact::createCompact: ", RESULT_CODE::CALCULATION_ERROR);
        return nullptr;
    }


    return nullptr;
}

//union
ICompact* ICompact::add(ICompact const* const left, ICompact const* const right, ILogger* logger){
    if (left == nullptr || right == nullptr) {
        logWithCheckAndReturn(logger, "ICompact::createCompact: ", RESULT_CODE::WRONG_ARGUMENT);
        return nullptr;
    }
    if (left->getDim() != right->getDim()) {
        logWithCheckAndReturn(logger, "ICompact::createCompact: ", RESULT_CODE::WRONG_DIM);
        return nullptr;
    }
    bool result;
    ICompact* Res = nullptr;

    RESULT_CODE res = left->isSubSet(right, result);
    if (res == RESULT_CODE::SUCCESS && result) {
        return left->clone();
    }
    res = right->isSubSet(left, result);
    if (res == RESULT_CODE::SUCCESS && result) {
        return right->clone();
    }
    res = left->isIntersects(right, result);
    if (res == RESULT_CODE::SUCCESS && result) {
        IVector* beginLeft = left->getBegin();
        IVector* endLeft = left->getEnd();
        IVector* beginRight = right->getBegin();
        IVector* endRight = right->getEnd();
        IVector* subBegin = IVector::sub(beginLeft, beginRight, logger);
        IVector* subEnd = IVector::sub(endLeft, endRight, logger);
        int countBegin = 0;
        int countEnd = 0;
        size_t index_begin = 0;
        size_t index_end = 0;

        for (size_t i = 0; i < left->getDim(); i++) {
            if (subBegin->getCoord(i) != 0) {
                countBegin++;
                index_begin = i;
            }
            if (subEnd->getCoord(i) != 0) {
                countEnd++;
                index_end = i;
            }
        }
        if (countBegin <= 1 && countEnd <= 1 && index_begin == index_end) {
            Res = ICompact::makeConvex(left, right, logger);
        }

        delete beginLeft;
        delete endLeft;
        delete beginRight;
        delete endRight;
        delete subBegin;
        delete subEnd;
    }
    return Res;
}

//methods kill closed!!!
//static ICompact* Difference(ICompact const* const left, ICompact const* const right, ILogger*logger);
//static ICompact* SymDifference(ICompact const* const left, ICompact const* const right, ILogger*logger);
ICompact* ICompact::makeConvex(ICompact const* const left, ICompact const* const right, ILogger* logger) {
    if (left == nullptr || right == nullptr) {
        logWithCheckAndReturn(logger, "ICompact::createCompact: ", RESULT_CODE::WRONG_ARGUMENT);
        return nullptr;
    }
    if (left->getDim() != right->getDim()) {
        logWithCheckAndReturn(logger, "ICompact::createCompact: ", RESULT_CODE::WRONG_DIM);
        return nullptr;
    }

    ICompact* result = nullptr;
    size_t size = right->getDim();
    double* dataBegin = new double[size];
    double* dataEnd = new double[size];
    IVector* leftBegin = left->getBegin();
    IVector* leftEnd = left->getEnd();
    IVector* rightBegin = right->getBegin();
    IVector* rightEnd = right->getEnd();
    for (size_t i = 0; i < size; i++) {
        dataBegin[i] = std::min(leftBegin->getCoord(i), rightBegin->getCoord(i));
        dataEnd[i] = std::max(leftEnd->getCoord(i), rightEnd->getCoord(i));
    }
    delete rightBegin;
    delete rightEnd;
    delete leftBegin;
    delete leftEnd;

    IVector* beginNew = IVector::createVector(size, dataBegin, logger);
    IVector* endNew = IVector::createVector(size, dataEnd, logger);
    result = new MyCompact(beginNew, endNew, logger);
    delete beginNew;
    delete endNew;
    delete[] dataBegin;
    delete[] dataEnd;

    beginNew = nullptr, endNew = nullptr;
    dataBegin = nullptr, dataEnd = nullptr;
    return result;


}

static inline RESULT_CODE logWithCheckAndReturn(ILogger * pLogger, char const* pMsg, RESULT_CODE err) {
    if (!pLogger) {
        pLogger->log(pMsg, err);
    }
    return err;
}
