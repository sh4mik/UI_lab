#ifndef IVECTOR_H
#define IVECTOR_H
#include<stddef.h>

class IVector{
public:
    static IVector* createVector(size_t dim, double* pData);
    virtual IVector* clone() const = 0;
    static IVector* add(IVector const* pOperand1, IVector const* pOperand2);
    static IVector* sub(IVector const* pOperand1, IVector const* pOperand2);
    static IVector* mul(IVector const* pOperand1, double scaleParam);
    static double mul(IVector const* pOperand1, IVector const* pOperand2);
    virtual double getCoord(size_t index)const = 0;
    virtual int setCoord(size_t index, double value) = 0;
    virtual size_t getDim()const = 0;
protected:
    IVector() = default;
    virtual ~IVector() = default;
};


#endif // IVECTOR_H
