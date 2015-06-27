#include "INSOLWechat.h"
#include "thelib/INSCCSingleton.h"
#include <algorithm>
#include <cocos2d.h>
//#include "HttpAgent.h"

#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
//#include "impl/os/wx/INSWxApi_iOS.h"
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#include "INSWxApi_Android.h"
#endif

inewsoft::ol::Wechat* getWxApi(void)
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
    //return inewsoft::CCSingleton<INSWxApi_iOS>::instance();
#elif CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
    return inewsoft::CCSingleton<INSWxApi>::instance();
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    return inewsoft::CCSingleton<INSWxApi_Android>::instance();
#elif CC_TARGET_PLATFORM == CC_PLATFORM_WP8
    return nullptr;
#else
    return nullptr;
#endif
}

