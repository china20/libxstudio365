
#include "NXNativeUtils.h"
#include <purelib/utils/nsconv.h>
#import <UIKit/UIKit.h>
#import "NXNativeUtils_objc.h"
#import "NXOpenUDID_objc.h"
#import "AudioToolbox/AudioServices.h"

#pragma mark -
#pragma mark activity indicator

void NXNative::showActivityIndicator(ActivityIndicatorViewStyle style)
{
    [[NXNative_objc sharedInstance] showActivityIndicator:static_cast<UIActivityIndicatorViewStyle>(style)];
}

void NXNative::hideActivityIndicator(void)
{
    [[NXNative_objc sharedInstance] hideActivityIndicator];
}

#pragma mark -
#pragma mark alert view

void NXNative::createAlert(const char* title,
                           const char* message,
                           const char* cancelButtonTitle)
{
    NSString *title_ = [NSString stringWithUTF8String:title ? title : ""];
    NSString *message_ = [NSString stringWithUTF8String:message ? message : ""];
    NSString *cancelButtonTitle_ = cancelButtonTitle ? [NSString stringWithUTF8String:cancelButtonTitle] : nil;
    [[NXNative_objc sharedInstance] createAlertView:title_
                                         andMessage:message_
                               andCancelButtonTitle:cancelButtonTitle_];
}

int NXNative::addAlertButton(const char* buttonTitle)
{
    NSString *buttonTitle_ = [NSString stringWithUTF8String:buttonTitle ? buttonTitle : "Button"];
    return [[NXNative_objc sharedInstance] addAlertButton:buttonTitle_];
}

    void NXNative::showAlert(inewsoft::AlertViewDelegate* delegate)
{
    [[NXNative_objc sharedInstance] showAlertViewWithDelegate:delegate];
}

void NXNative::cancelAlert(void)
{
    [[NXNative_objc sharedInstance] cancelAlertView];
}

void NXNative::alert(const char* title, const char* message, const char* cancelButtonTitle)
{
    NXNative::cancelAlert();
    NXNative::createAlert(title, message, cancelButtonTitle);
    NXNative::showAlert();
}

void NXNative::openURL(const char* url)
{
    if (!url) return;
    NSURL *nsurl = [NSURL URLWithString:[NSString stringWithCString:url encoding:NSUTF8StringEncoding]];
    [[UIApplication sharedApplication] openURL:nsurl];
}

#pragma mark -
#pragma mark DeviceId

std::string NXNativeUtils::getDeviceId(void)
{
    return purelib::nsconv::charstring2hexstring(std::string([[OpenUDID_objc value] cStringUsingEncoding:NSUTF8StringEncoding]));
}


const char* NXNative::getDeviceName(void)
{
    UIDevice *device = [UIDevice currentDevice];
    return [[device name] cStringUsingEncoding:NSUTF8StringEncoding];
}

void NXNative::vibrate()
{
    AudioServicesPlaySystemSound(kSystemSoundID_Vibrate);
}

#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
void NXNative::showAlertObjc(void *delegate)
{
    [[NXNative_objc sharedInstance] showAlertViewWithObjcDelegate:(id<UIAlertViewDelegate>)delegate];
}
#endif

#if CC_LUA_ENGINE_ENABLED > 0
int NXNative::addAlertButtonLua(const char* buttonTitle)
{
    return addAlertButton(buttonTitle) + 1;
}

void NXNative::showAlertLua(cocos2d::LUA_FUNCTION listener)
{
    [[NXNative_objc sharedInstance] showAlertViewWithLuaListener:listener];
}
#endif


