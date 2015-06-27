#include "NXOLWechat.h"
#include "thelib/INSCCSingleton.h"
#include <algorithm>
#include <cocos2d.h>
//#include "HttpAgent.h"

#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
//#include "impl/os/wx/NXWxApi_iOS.h"
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#include "NXWxApi_Android.h"
#endif

ol::Wechat* getWxApi(void)
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
    //return inewsoft::CCSingleton<INSWxApi_iOS>::instance();
#elif CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
    return purelib::Singleton<NXWxApi>::instance();
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    return purelib::Singleton<NXWxApi_Android>::instance();
#elif CC_TARGET_PLATFORM == CC_PLATFORM_WP8
    return nullptr;
#else
    return nullptr;
#endif
}

