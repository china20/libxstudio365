//
// Copyright (c) 2014 purelib - All Rights Reserved
//
#ifndef _INS_SharedPtr_H_
#define _INS_SharedPtr_H_
#include <iostream>
#include "platform/CCPlatformMacros.h"

namespace purelib {

template<typename _Ty> inline
void ccReleaseObject(_Ty* ccObject)
{
    CC_SAFE_RELEASE(ccObject);
}

namespace gc {

// TEMPLATE CLASS, equals to cocos2d-x-3.x cocos2d::RefPtr
template< typename _Ty>
class SharedPtr;

template<typename _Ty>
class SharedPtr
{    // wrap an object pointer to ensure destruction
public:
    typedef SharedPtr<_Ty> _Myt;
    typedef _Ty element_type;

    explicit SharedPtr(_Ty *_Ptr = 0) throw()
        : ref(_Ptr)
    {    // construct from object pointer
    }

    SharedPtr(std::nullptr_t) throw() 
        : ref(0)
    {
    }

    SharedPtr(const _Myt& _Right) throw()
    {    // construct by assuming pointer from _Right SharedPtr
        CC_SAFE_RETAIN(_Right.get());

        ref = _Right.get();
    }

    template<typename _Other>
    SharedPtr(const SharedPtr<_Other>& _Right) throw()
    {    // construct by assuming pointer from _Right
        CC_SAFE_RETAIN(_Right.get());

        ref = (_Ty*)_Right.get();
    }

    SharedPtr(_Myt&& _Right) throw()
    {
        CC_SAFE_RETAIN(_Right.get());

        ref = (_Ty*)_Right.get();
    }

    template<typename _Other>
    SharedPtr(SharedPtr<_Other>&& _Right) throw()
    {    // construct by assuming pointer from _Right
        CC_SAFE_RETAIN(_Right.get());

        ref = (_Ty*)_Right.get();
    }

    _Myt& operator=(const _Myt& _Right) throw()
    {    // assign compatible _Right (assume pointer)
        if(this == &_Right)
            return *this;

        CC_SAFE_RETAIN(_Right.get());

        reset( _Right.get() );
        return (*this);
    }

    _Myt& operator=(_Myt&& _Right) throw()
    {    // assign compatible _Right (assume pointer)

        CC_SAFE_RETAIN(_Right.get());

        reset( _Right.get() );
        return (*this);
    }

    template<typename _Other>
    _Myt& operator=(const SharedPtr<_Other>& _Right) throw()
    {    // assign compatible _Right (assume pointer)
        if(this == &_Right)
            return *this;

        CC_SAFE_RETAIN(_Right.get());

        reset( (_Ty*)_Right.get() );
        return (*this);
    }

    template<typename _Other>
    _Myt& operator=(SharedPtr<_Other>&& _Right) throw()
    {    // assign compatible _Right (assume pointer)
        CC_SAFE_RETAIN(_Right.get());

        reset( (_Ty*)_Right.get() );
        return (*this);
    }

    _Myt& operator=(std::nullptr_t) throw()
    {
        reset();
        return (*this);
    }

    ~SharedPtr()
    {    // release the object
        CC_SAFE_RELEASE(ref);
    }

    _Ty& operator*() const throw()
    {    // return designated value
        return (*ref); // return (*get());
    }

    _Ty** operator &() throw()
    {
        return &(ref);
    }

    _Ty *operator->() const throw()
    {    // return pointer to class object
        return (ref); // return (get());
    }

    template<typename _Int>
    _Ty& operator[](_Int index) const throw()
    {
        return (ref[index]);
    }

    _Ty* get() const throw()
    {    // return wrapped pointer
        return (ref);
    }

    _Ty*& get_ref() throw()
    {    // return wrapped pointer
        return (ref);
    }

    operator _Ty*() const throw()
    { // convert to basic type
        return (ref);
    }

    /*
    ** if already have a valid pointer, will call release firstly
    */
    void reset(_Ty *_Ptr = 0) 
    {    // relese designated object and store new pointer

        if (ref != nullptr) 
        { 
            if (ref != _Ptr) // release old
                CC_SAFE_RELEASE(ref);
        }

        if (ref != _Ptr) // retain new
            CC_SAFE_RETAIN(_Ptr);

        ref = _Ptr;
    }

private:
    _Ty* ref;    // the wrapped object pointer
};

}; /* namespace purelib::gc */

}; /* namespace purelib */

using namespace purelib::gc;

#endif
