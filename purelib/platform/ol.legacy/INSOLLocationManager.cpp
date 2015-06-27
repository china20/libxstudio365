#include "INSOLLocationManager.h"
#include "thelib/INSCCSingleton.h"

USING_NS_CC;

#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS

#include "impl/ios/gps/INSLocationManager_iOS.h"
//inewsoft::ol::LocationManager* inewsoft::ol::getLocationManager(void)
//{
//	return CCSingleton<inewsoft::ol::LocationManager_iOS>::instance();
//}

#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID

#include "INSOLLocationManager_Android.h"
LocationManager* inewsoft::ol::getLocationManager(void)
{
	return CCSingleton<inewsoft::ol::LocationManager_Android>::instance();
}

#else

ins::ol::LocationManager* inewsoft::ol::getLocationManager(void)
{
	return CCSingleton<inewsoft::ol::LocationManager>::instance();
}

#endif
