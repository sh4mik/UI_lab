#ifndef ICOMPACT_H
#define ICOMPACT_H

class IVector;

#include "ILogger.h"

class ICompact
{
public:
    class iterator;
    enum InterfaceTypes
    {
        INTERFACE_0,
        DIMENSION_INTERFACE_IMPL
    };

    virtual int getId() const = 0;

    /*factories*/

    static ICompact* createCompact(IVector const* const begin, IVector const* const end,IVector const*const step, ILogger*logger);

    /*operations*/
    virtual RESULT_CODE Intersection(ICompact const& c) = 0;
    virtual RESULT_CODE Union(ICompact const& c) = 0;
    virtual RESULT_CODE Difference(ICompact const& c) = 0;
    virtual RESULT_CODE SymDifference(ICompact const& c) = 0;
    virtual RESULT_CODE MakeConvex() = 0;

    /*static operations*/
    static ICompact* Intersection(ICompact const* const left, ICompact const* const right, ILogger*logger);
    static ICompact* Union(ICompact const* const left, ICompact const* const right, ILogger*logger);
    static ICompact* Difference(ICompact const* const left, ICompact const* const right, ILogger*logger);
    static ICompact* SymDifference(ICompact const* const left, ICompact const* const right, ILogger*logger);
    static ICompact* MakeConvex(ICompact const* const left, ICompact const* const right, ILogger*logger);

    /* returns a step with which you can iterate over all domains of compact*/
    virtual IVector* getStep() const = 0;

    virtual iterator* end(IVector const* const step = 0) = 0;
    virtual iterator* begin(IVector const* const step = 0) = 0;

    virtual RESULT_CODE isContains(IVector const* const vec, bool& result) const = 0;
    virtual RESULT_CODE isSubSet(ICompact const* const other,bool& result) = 0;
    virtual RESULT_CODE isSimplyConn(bool& result) = 0;
    virtual RESULT_CODE isIntersects(ICompact const* const other, bool& result) = 0;

    virtual ICompact* clone() const = 0;

    /*dtor*/
    virtual ~ICompact() = default;

    class iterator
    {
    public:
        //adds step to current value in iterator
        virtual RESULT_CODE doStep() = 0;

        //change step
        virtual RESULT_CODE setDirection(IVector const* const dir) = 0;


    protected:
        iterator(IVector const* const step);

        /*dtor*/
        virtual ~iterator() = default;

    private:
        /*non default copyable*/
        iterator(const iterator& other) = delete;
        void operator=( const iterator& other) = delete;
    };
protected:
    ICompact() = default;
    virtual ~ICompact();

private:
    /*non default copyable*/
    ICompact(const ICompact& other) = delete;
    void operator=( const ICompact& other) = delete;
};

#endif // ICOMPACT_H
