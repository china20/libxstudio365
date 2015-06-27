#import "INSIAd_objc.h"
#include "cocos2d.h"
#include <cocos/platform/ios/CCEAGLView.h>

#define OUT_POS CGPointMake(-1024, -1024)
static float BANNER_HEIGHT = 50;
;
@implementation INSIAd_objc

static INSIAd_objc* s_sharedIAd = nil;

#pragma mark - 
#pragma mark init

+ (INSIAd_objc*) sharedIAd
{
    if(s_sharedIAd != nil)
        return s_sharedIAd;
    s_sharedIAd = [[INSIAd_objc alloc] init];
    return s_sharedIAd;
}

+ (void) purgeSharedIAd
{
    if(s_sharedIAd != nil)
    {
        [s_sharedIAd release];
    }
}

- (id)init{
    self = [super init];
    if (self) {
        [self initBanner];
    }
    
    return self;
}

-(void)relaease
{
    if(bannerView ) {
        [bannerView removeFromSuperview];
        [bannerView release];
    }
    [super dealloc];
}

- (void)dealloc
{
    
    [super dealloc];
}

//自定义的函式 Banner初始化（以画面直立）
- (void)initBanner {
    
    CCEAGLView* glview = [CCEAGLView sharedEGLView];
    
    BANNER_HEIGHT = glview.frame.size.height;
    
    //以画面直立的方式设定Banner于画面顶部
    bannerView = [[ADBannerView alloc]initWithFrame:CGRectMake(0.0, 0.0/*glview.frame.size.height - BANNER_HEIGHT*/, glview.frame.size.width, BANNER_HEIGHT)];
    
    //此Banner所能支援的类型
    bannerView.requiredContentSizeIdentifiers = [NSSet setWithObjects:ADBannerContentSizeIdentifierPortrait, ADBannerContentSizeIdentifierLandscape, nil];
    
    //目前的Banner 类型
    bannerView.currentContentSizeIdentifier = ADBannerContentSizeIdentifierPortrait;
    
    //设定代理
    bannerView.delegate = self;
    
    //无法按下触发广告
    bannerView.userInteractionEnabled = NO;
    
    //设定偏位移量隐藏在顶部
    bannerView.frame = CGRectOffset(bannerView.frame, 0, -BANNER_HEIGHT);
    
    //将Banner view添加到游戏主界面
    [glview addSubview:bannerView];
    
    [glview bringSubviewToFront: bannerView];
}

- (ADBannerView*) getBannerView
{
    return bannerView;
}

- (void)bannerViewAnimation
{
    //动画设定
    [UIView beginAnimations:@"BannerViewAnimation" context:NULL];
    
    //以userInteractionEnabled状态排判断bannerView是否在画面内
    if (bannerView.userInteractionEnabled) { // will hide banner view
        bannerView.frame = CGRectOffset(bannerView.frame, 0, -BANNER_HEIGHT);
    }
    else { // will show banner view
        bannerView.frame = CGRectOffset(bannerView.frame, 0, BANNER_HEIGHT);
    }
    
    //开始动画
    [UIView commitAnimations];
    
    //将userInteractionEnabled做反向设定
    bannerView.userInteractionEnabled = !bannerView.userInteractionEnabled;
}

//当ADBannerView完成读取广告时会触发的事件
- (void)bannerViewDidLoadAd:(ADBannerView *)banner {
    [self bannerViewAnimation];
}

-(void) bannerView:(ADBannerView *)banner didFailToReceiveAdWithError:(NSError *)error
{
    /*if (CCIAd::bannerIsVisible_) {
        [UIView beginAnimations:@"animateAdBannerOff" context:NULL];
        // banner is visible and we move it out of the screen, due to connection issue
        ((ADBannerView *)CCIAd::adView_).center = OUT_POS;
        [UIView commitAnimations];
        CCIAd::bannerIsVisible_ = false;
    }*/
    bannerView.center = OUT_POS;
}

//按下广告时会触发
-(BOOL)bannerViewActionShouldBegin:(ADBannerView *)banner willLeaveApplication:(BOOL)willLeave
{
    //这里可以放置暂停你应用程式的程式码
    cocos2d::CCDirector::sharedDirector()->stopAnimation();
    return YES;
}

//在广告中按下返回应用程序按钮时所触发
- (void)bannerViewActionDidFinish:(ADBannerView *)banner
{
    //这里可以放置重启你应用程式的程式码
    [self bannerViewAnimation];
    cocos2d::CCDirector::sharedDirector()->startAnimation();
}

/*-(void) willRotateToInterfaceOrientation:(UIInterfaceOrientation) toInterfaceOrientation duration:(NSTimeInterval)durationhttp://item.jd.com/832264.html
{
    //adView.frame = CGRectZero;
    if (UIInterfaceOrientationIsLandscape(toInterfaceOrientation)) {
        ((ADBannerView *)CCIAd::adView_).currentContentSizeIdentifier = ADBannerContentSizeIdentifierPortrait;
        ((ADBannerView *)CCIAd::adView_).frame = CGRectZero;
    } else {
        ((ADBannerView *)CCIAd::adView_).currentContentSizeIdentifier = ADBannerContentSizeIdentifierPortrait;
        ((ADBannerView *)CCIAd::adView_).frame = CGRectZero;
    } 
}*/

//画面转动时回调
- (void)willRotateToInterfaceOrientation:(UIInterfaceOrientation)toInterfaceOrientation duration:(NSTimeInterval)duration {
    
    return; // fixed no roate
    // assert(false);
    
    //还原bannerView预设值
    bannerView.userInteractionEnabled = NO;
    bannerView.frame = CGRectOffset(bannerView.frame, 0,0);
    
    CCEAGLView* glview = [CCEAGLView sharedEGLView];
    
    //判断画面是否倾置
    if (UIInterfaceOrientationIsLandscape(toInterfaceOrientation)) {
        bannerView.currentContentSizeIdentifier = ADBannerContentSizeIdentifierLandscape;
        bannerView.frame = CGRectMake(0.0, 288.0, glview.frame.size.width, 32.0);
    }
    else {
        bannerView.currentContentSizeIdentifier = ADBannerContentSizeIdentifierPortrait;
        bannerView.frame = CGRectMake(0.0, 430.0, glview.frame.size.width, BANNER_HEIGHT);
    }
    
    //重新设定偏移量
    bannerView.frame = CGRectOffset(bannerView.frame, 0, BANNER_HEIGHT);
}

@end
