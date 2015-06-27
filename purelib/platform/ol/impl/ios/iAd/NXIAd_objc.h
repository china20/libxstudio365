
#import <iAd/iAd.h>

@interface INSIAd_objc : NSObject<ADBannerViewDelegate>
{
    ADBannerView *bannerView;
}

+ (INSIAd_objc*) sharedIAd;
+ (void) purgeSharedIAd;

- (void)initBanner;

- (void)bannerViewAnimation;

- (ADBannerView*) getBannerView;

@end

