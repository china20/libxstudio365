//
// Copyright (c) 2014 purelib - All Rights Reserved
//
#ifndef _NX_MACRODEFS_H_
#define _NX_MACRODEFS_H_
#include <new>

/// converts
#ifdef _WIN32
//#undef cccontrol_selector
//#define cccontrol_selector(_SELECTOR) super_cast::force_cast<SEL_CCControlHandler>(&_SELECTOR)
#endif

/// test static array size
#define ARRAY_SIZE(array) ( sizeof(array) / sizeof(array[0]) )

/// math
#define gap(a1,d,n) ( (a1) + ((n) - 1) * (d) )
#define sap(a1,a2,a3,n) ( (a1) + ( (a2) - (a1) ) * ( (n) - 1) + ( (a3) - 2 * (a2) + (a1) ) * ( (n) - 1) * ( (n) - 2 ) / 2.0 )

/// cocos2d singleton objects
#define CCDIRECTOR  cocos2d::Director::getInstance()
#define CCRUNONGL   CCDIRECTOR->getScheduler()->performFunctionInCocosThread
#define CCEVENTMGR  CCDIRECTOR->getEventDispatcher()
#define CCSCHTASKS  CCDIRECTOR->getScheduler()
#define CCACTIONMGR CCDIRECTOR->getActionManager()
#define CCFILEUTILS cocos2d::FileUtils::getInstance()
#define CCAUDIO     cocos2d::SimpleAudioEngine::getInstance()
#define CCAPP       cocos2d::CCApplication::getInstance()


#define CREATE_FUNC_NTR(__TYPE__) \
    static __TYPE__* create() \
{ \
    __TYPE__*  pRet = new(std::nothrow) __TYPE__(); \
    if (pRet && pRet->init()) \
    { \
        pRet->autorelease(); \
        return pRet; \
    } \
    else \
    { \
        delete pRet; \
        return NULL; \
    } \
}

#define CR_FUNC_NTR CREATE_FUNC_NTR

#endif

