#ifndef _NXWXAPI_H_
#define _NXWXAPI_H_
#include <cocos2d.h>
#include <functional>
#include <map>
#include <unordered_map>
#include <string>
#include <vector>
#include <functional>
#include "thelib/INSMacroDefs.h"
#include "impl/ios/wx/INSWxApiObject.h"

USING_NS_CC;
namespace inewsoft {
namespace ol {

typedef std::function<void(const INSWxBaseResp&)> WxRespCallback;
typedef std::function<void(const INSWxBaseReq&)>  WxReqCallback;

class Wechat : public cocos2d::Ref
{
public:
    virtual ~Wechat(void) {};
    CR_FUNC_NTR(Wechat);
    virtual bool init(void) {return true;};
    virtual void sendTextReq(const char* text, const WxRespCallback& callback = nullptr) {if(callback) callback(INSWxBaseResp());};
    virtual void sendLinkReq(const char* title, const char* description, const char* url, const WxRespCallback& callback = nullptr) {if(callback) callback(INSWxBaseResp());};
    virtual void sendImageReq(CCImage* image, CCImage* thumbImage, const WxRespCallback& callback = nullptr) {if(callback) callback(INSWxBaseResp());};
    
    virtual void changeScene(INSWxSceneType){};
    virtual bool isWxAppInstalled(void) {return false;}
    virtual bool isWxAppSupportApi(void) {return false;}
    virtual std::string getWxAppInstallUrl(void) {return "";}
    virtual std::string getApiVersion(void) {return "0.0.0";}
    virtual bool openWxApp(void){return false;}
    
protected:
    WxRespCallback respCallback;
    WxReqCallback  reqCallback;
};
}; /* namespace inewsoft::ol */
}; /* namespace inewsoft */

extern inewsoft::ol::Wechat* getWxApi(void);
#define  wxapi getWxApi()

#endif
    
    


