#ifndef _INSWXAPI_IOS_H_
#define _INSWXAPI_IOS_H_

#include "../../../INSOLWechat.h"
#include "INSWxApiObject.h"

/// INSWxApi_iOS impl interface
class INSWxApi_iOS : public inewsoft::ol::Wechat, public INSWxApiDelegate
{
public:
    virtual ~INSWxApi_iOS(void) {};
    virtual bool init(void);
    virtual void sendTextReq(const char* text, const inewsoft::ol::WxRespCallback& callback = nullptr);
    virtual void sendLinkReq(const char* title, const char* description, const char* url, const inewsoft::ol::WxRespCallback& callback = nullptr);
    virtual void sendImageReq(CCImage* image, CCImage* thumbImage, const inewsoft::ol::WxRespCallback& callback = nullptr);
    
    virtual void changeScene(INSWxSceneType);
    virtual bool isWxAppInstalled(void) ;
    virtual bool isWxAppSupportApi(void) ;
    virtual std::string getWxAppInstallUrl(void) ;
    virtual std::string getApiVersion(void) ;
    virtual bool openWxApp(void);
    virtual void onResp(const INSWxBaseResp& );
    virtual void onReq(const INSWxBaseReq& );
};


#endif


