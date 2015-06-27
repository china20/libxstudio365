// Copyright (c) 2014 purelib, All rights reserved.
// cross platform APIs
#ifndef _NX_EXTENSION_NATIVE_H_
#define _NX_EXTENSION_NATIVE_H_

//#include "NXAlertViewDelegate.h"

#if CC_LUA_ENGINE_ENABLED > 0
#include "CCLuaEngine.h"
#endif

#include <string>

///** @brief Activity indicator style */
//typedef enum {
//    kActivityIndicatorViewStyleWhiteLarge,
//    kActivityIndicatorViewStyleWhite,
//    kActivityIndicatorViewStyleGray,
//} NXActivityIndicatorViewStyle;

class NXNativeUtils
{
public:
	enum NetworkType  {
        NETWORKTYPE_INVALID = 0,
	    /** wap网络 */
	    NETWORKTYPE_WAP = 1,
	    /** 2G网络 */
	    NETWORKTYPE_2G = 2,
	    /** 3G和3G以上网络，或统称为快速网络 */
	    NETWORKTYPE_3G = 3,
	    /** wifi网络 */
	    NETWORKTYPE_WIFI = 4,
	};
    
//    /** @brief Show activity indicator */
//    static void showActivityIndicator(NXActivityIndicatorViewStyle style = kActivityIndicatorViewStyleWhiteLarge);
//    
//    /** @brief Hide activity indicator */
//    static void hideActivityIndicator(void);
//
//    
//    /** @brief Create alert view */
//    static void createAlert(const char* title,
//                            const char* message,
//                            const char* cancelButtonTitle);
//    
//    /** @brief Add button to alert view, return button index */
//    static int addAlertButton(const char* buttonTitle);
//
//    /** @brief Show alert view */
//
//    //static void showAlert(purelib::AlertViewDelegate* delegate = NULL);
//
//    /** @brief Hide and remove alert view */
//    static void cancelAlert(void);
//    
//    static void alert(const char* title, const char* message, const char* cancelButtonTitle);
//    
//
//    
//    
//    /** @brief Open a web page in the browser; create an email; or call a phone number. */
//    static void openURL(const char* url);
//    
//	/** @brief Show alert view, and get user input */
//    static const std::string getInputText(const char* title, const char* message, const char* defaultValue);
//    
//    
//    static const char* getDeviceName(void);
//    static void vibrate();
//    
//#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
//    static void showAlertObjc(void *delegate);
//#endif
//    
//#if CC_LUA_ENGINE_ENABLED > 0
//    static int addAlertButtonLua(const char* buttonTitle);
//    static void showAlertLua(cocos2d::LUA_FUNCTION listener);
//#endif
    /** @brief Android: IMEI, IOS:UDID */
    static  std::string getDeviceId(void);

    /** @brief Android: IMEI Software Version */
    static  std::string getIMEISV(void);

    /** @brief Android: SIM Card Id */
    static  std::string getIMSI(void);

    /** @brief Android: Phone Number */
    static  std::string getMSISDN(void);

    /** @brief Android: Network Card Id */
    static  std::string getNICId(void);

    static std::string getLocalIP(void);

    static NetworkType getNetworkType(void);

	static void callTel(const char * telnumber);
private:
    NXNativeUtils(void) {}
};

#endif // __CC_EXTENSION_INSNative_H_
