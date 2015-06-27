
#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

#include "NXAlertViewDelegate.h"
#include <cocos2d.h>
//#include <cocos-ext.h>

#if CC_LUA_ENGINE_ENABLED > 0
#include "CCLuaEngine.h"
#endif

USING_NS_CC;
//USING_NS_CC_EXT;

@interface NXNative_objc : NSObject < UIAlertViewDelegate >
{
    UIActivityIndicatorView *activityIndicatorView_;
    
    UIAlertView *alertView_;
    inewsoft::AlertViewDelegate *alertViewDelegates_;
#if CC_LUA_ENGINE_ENABLED > 0
    LUA_FUNCTION alertViewLuaListener_;
#endif
}

+ (NXNative_objc *)sharedInstance;


#pragma mark -
#pragma mark activity indicator

- (void)showActivityIndicator:(UIActivityIndicatorViewStyle)style;
- (void)hideActivityIndicator;


#pragma mark -
#pragma mark alert view

- (void)createAlertView:(NSString *)title
             andMessage:(NSString *)message
   andCancelButtonTitle:(NSString *)cancelButtonTitle;
- (NSInteger)addAlertButton:(NSString *)buttonTitle;
- (void)showAlertViewWithDelegate:(inewsoft::AlertViewDelegate *)delegate;
- (void)removeAlertView;
- (void)cancelAlertView;

#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
- (void)showAlertViewWithObjcDelegate:(id<UIAlertViewDelegate>)delegate;
#endif

#if CC_LUA_ENGINE_ENABLED > 0
- (void)showAlertViewWithLuaListener:(LUA_FUNCTION)listener;
- (void)removeAlertViewLuaListener;
#endif

#pragma mark -
#pragma mark UIAlertView delegates

- (void)alertView:(UIAlertView *)alertView clickedButtonAtIndex:(NSInteger)buttonIndex;

@end
