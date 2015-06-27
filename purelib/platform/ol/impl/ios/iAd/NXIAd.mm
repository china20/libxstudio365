#include "INSIAd.h"
#include "INSIAd_objc.h"

bool INSIAd::init(void)
{
    [INSIAd_objc sharedIAd];
    return true;
}

void INSIAd::showAdView(void)
{
    [[INSIAd_objc sharedIAd] getBannerView].userInteractionEnabled = false;
    [[INSIAd_objc sharedIAd] bannerViewAnimation];
}

void INSIAd::hideAdView(void)
{
    [[INSIAd_objc sharedIAd] bannerViewAnimation];
}

void INSIAd::cleanup(void)
{
    [INSIAd_objc purgeSharedIAd];
}


