#include "NXOLLocationManager.h"
#include "purelib/NXCSingleton.h"

USING_NS_CC;

#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS

#include "impl/ios/gps/NXLocationManager_iOS.h"
//inewsoft::ol::LocationManager* inewsoft::ol::getLocationManager(void)
//{
//	return CCSingleton<inewsoft::ol::LocationManager_iOS>::instance();
//}

#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID

#include "NXOLLocationManager_Android.h"
LocationManager* ol::getLocationManager(void)
{
	return purelib::Singleton<ol::LocationManager_Android>::instance();
}

#else

ol::LocationManager* ol::getLocationManager(void)
{
	return purelib::Singleton<ol::LocationManager>::instance();
}

#endif
