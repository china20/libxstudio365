#include "platform/android/jni/JniHelper.h"
#include "NXNativeUtils.h"
#include "purelib/utils/nsconv.h"
#include <android/log.h>
#include <string>
#include <jni.h>
#include "NXJniWrapper.h"

using namespace cocos2d;

/** @brief Android: IMEI, IOS:UDID */
 std::string NXNativeUtils::getDeviceId(void)
 {
	 auto deviceId = NXJniWrapper::callJavaStaticStringFunc("getCompatibleDeviceId");
	 purelib::nsc::replace(deviceId, "-", "");
	 return std::move(deviceId);
 }

/** @brief Android: IMEI Software Version */
 std::string NXNativeUtils::getIMEISV(void)
 {
	 return NXJniWrapper::callJavaStaticStringFunc("getIMEISV");
 }

/** @brief Android: SIM Card Id */
 std::string NXNativeUtils::getIMSI(void)
 {
	 return NXJniWrapper::callJavaStaticStringFunc("getIMSI");
 }

/** @brief Android: Phone Number */
 std::string NXNativeUtils::getMSISDN(void)
 {
	 return NXJniWrapper::callJavaStaticStringFunc("getMSISDN");
 }

std::string NXNativeUtils::getNICId(void)
{
	return NXJniWrapper::callJavaStaticStringFunc("getLocalMacAddress");
}

std::string NXNativeUtils::getLocalIP(void)
{
	return NXJniWrapper::callJavaStaticStringFunc("getLocalIP");
}

NXNativeUtils::NetworkType NXNativeUtils::getNetworkType(void)
{
    return static_cast<NXNativeUtils::NetworkType>(NXJniWrapper::callJavaStaticIntFunc("getNetWorkType"));
}

void NXNativeUtils::callTel(const char * telnumber)
{
	if (!telnumber) {
		return;
	}
	
	NXJniWrapper::callJavaStaticFunc("callTel", telnumber);
}


