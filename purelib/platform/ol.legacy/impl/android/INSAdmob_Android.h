#ifndef _NXWXAPI_ANDROID_H_
#define _NXWXAPI_ANDROID_H_

#include "NxWxApi.h"
#include "ProtocolAds.h"
/// NxWxApi_Android interface
using namespace cocos2d::plugin;

class NxAdmob_Android
{
public:
    virtual ~NxAdmob_Android(void);
    virtual bool init(void);
    /// ads java sdk implement follow
    void showAdView(void);
    void hideAdView(void);
};


#endif


