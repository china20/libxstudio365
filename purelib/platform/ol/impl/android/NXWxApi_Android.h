#ifndef _NXWXAPI_ANDROID_H_
#define _NXWXAPI_ANDROID_H_

#include "NxWxApi.h"
#include "ProtocolWechat.h"
/// NxWxApi_Android interface
using namespace cocos2d::plugin;

class NxWxApi_Android : public NxWxApi, public WechatListener
{
public:
    virtual ~NxWxApi_Android(void);
    virtual bool init(void);
    virtual void sendTextReq(const char* text, const NxWxRespCallback& callback);
    virtual void sendLinkReq(const char* title, const char* description, const char* url, const NxWxRespCallback& callback);

    /// wx java sdk implement follow
    virtual bool isWxAppInstalled(void) ;
    virtual bool isWxAppSupportApi(void);
    virtual bool openWxApp(void);
    virtual void changeScene(enum CCWxSceneType);
    virtual void onResp(int code, const char* msg);
    virtual void onReq(int code, const char* msg);
};


#endif


