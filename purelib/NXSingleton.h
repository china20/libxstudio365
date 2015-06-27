//
// Copyright (c) 2014 purelib - All Rights Reserved
//
#ifndef _NXSINGLETON_H_
#define _NXSINGLETON_H_
#include "NXLibDefs.h"
#include "NXSharedPtr.h"

/*
** Singleton for cocos2d's autorelease objects
*/
namespace cocos2d {

template<typename _Ty>
class Singleton
{
    typedef Singleton<_Ty> _Myt;
public:
    template<typename ..._Args>
    static _Ty* instance(_Args...args)
    {
        if (nullptr == Singleton<_Ty>::__single__.get())
        {
            Singleton<_Ty>::__single__.reset(new(std::nothrow) _Ty());
            Singleton<_Ty>::__single__->release();
            if (Singleton<_Ty>::__single__ != nullptr)
                Singleton<_Ty>::delay_init(args...);
        }
        return Singleton<_Ty>::__single__.get();
    }

    static void destroy(void)
    {
        if (Singleton<_Ty>::__single__.get() != nullptr)
        {
            Singleton<_Ty>::__single__.reset();
        }
    }

    template<typename _Fty, typename..._Args>
    static void delay_init(const _Fty& memf, _Args...args)
    { // init use specific member func with more than 1 args
        std::mem_fn(memf)(_Myt::__single__.get(), args...);
    }

    template<typename _Fty, typename _Arg>
    static void delay_init(const _Fty& memf, const _Arg& arg)
    { // init use specific member func with 1 arg
        std::mem_fun(memf)(_Myt::__single__.get(), arg);
    }

    template<typename _Fty>
    static void delay_init(const _Fty& memf)
    { // init use specific member func without arg
        std::mem_fun(memf)(_Myt::__single__.get());
    }

    static void delay_init(void)
    { // dummy init
    }

private:
    static typename purelib::gc::SharedPtr<_Ty> __single__;
};

template<typename _Ty>
typename purelib::gc::SharedPtr<_Ty> Singleton<_Ty>::__single__;

};

#endif
