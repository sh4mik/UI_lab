#ifndef IBROCKER_H
#define IBROCKER_H

#include "RC.h"

class IBrocker
{
public:
    enum InterfaceTypes
    {
        INTERFACE_0,
        DIMENSION_INTERFACE_IMPL
    };

    enum Type
    {
        PROBLEM,
        SOLVER,
        DIMENSION_TYPE
    };


    virtual bool canCastTo(Type type) const = 0;
    virtual void* getInterfaceImpl(Type type) const = 0;
    virtual RESULT_CODE release() = 0; // harakiri

protected:
    /*dtor*/
    virtual ~IBrocker(){}

    IBrocker() = default;

private:
    /* no way copy/assignment */
    IBrocker(const IBrocker& other) = delete;
    void operator=(const IBrocker& other) = delete;
};


#endif // IBROCKER_H
