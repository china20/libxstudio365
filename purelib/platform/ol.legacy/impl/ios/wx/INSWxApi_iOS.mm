#include "INSWxApi_iOS.h"
#include "INSWxApi_objc.h"

// [WXApi registerApp:[NSString stringWithUTF8String:appid]];
// [WXApi registerApp:[ NSString stringWithUTF8String:appid] withDescription:[NSString stringWithUTF8String:appdesc] ];
bool INSWxApi_iOS::init(void)
{
    [[INSWxApi_objc sharedWxApi] setDelegate:this];
    return true;
}

void INSWxApi_iOS::sendTextReq(const char* text, const inewsoft::ol::WxRespCallback& callback)
{
    this->respCallback = callback;
    [[INSWxApi_objc sharedWxApi] sendTextContent:STR2NSS(text)];
}

// - (void) sendLinkContent:(NSString*)title andDesc:(NSString*)description andUrl:(NSString*)url andImage:(NSString*)image;
void INSWxApi_iOS::sendLinkReq(const char* title, const char* description, const char* url, const inewsoft::ol::WxRespCallback& callback)
{
    this->respCallback = callback;
    [[INSWxApi_objc sharedWxApi] sendLinkContent:STR2NSS(title)
                                        andDesc:STR2NSS(description)
                                        andUrl:STR2NSS(url)
                                        andImage:STR2NSS("Icon.png")];
}

void INSWxApi_iOS::sendImageReq(CCImage* image, CCImage* thumbImage, const inewsoft::ol::WxRespCallback& callback)
{
    this->respCallback = callback;
    // std::string fullpath = CCFileUtils::sharedFileUtils()->fullPathForFilename("images/common/b1.png");
    [[INSWxApi_objc sharedWxApi] sendImageContent:image
                                  andThumbImage:thumbImage];
    
}

/* 更改分享范围
 */
void INSWxApi_iOS::changeScene(enum INSWxSceneType type)
{
    [[INSWxApi_objc sharedWxApi] changeScene:type];
}

/*! @brief 检查微信是否已被用户安装
 *
 * @return 微信已安装返回YES，未安装返回NO。
 */
bool INSWxApi_iOS::isWxAppInstalled(void)
{
    return [WXApi isWXAppInstalled];
}

/*! @brief 判断当前微信的版本是否支持OpenApi
 *
 * @return 支持返回YES，不支持返回NO。
 */
bool INSWxApi_iOS::isWxAppSupportApi(void)
{
    return [WXApi isWXAppSupportApi];
}

/*! @brief 获取微信的itunes安装地址
 *
 * @return 微信的安装地址字符串。
 */
std::string INSWxApi_iOS::getWxAppInstallUrl(void)
{
    return [ [WXApi getWXAppInstallUrl] cStringUsingEncoding:NSUTF8StringEncoding];
}

/*! @brief 获取当前微信SDK的版本号
 *
 * @return 返回当前微信SDK的版本号
 */
std::string INSWxApi_iOS::getApiVersion(void)
{
    return [ [WXApi getApiVersion] cStringUsingEncoding:NSUTF8StringEncoding];
}

/*! @brief 打开微信
 *
 * @return 成功返回YES，失败返回NO。
 */
bool INSWxApi_iOS::openWxApp(void)
{
    return [WXApi openWXApp];
}

// callbacks
void INSWxApi_iOS::onResp(const INSWxBaseResp& resp )
{
    if(respCallback) {
        respCallback(resp);
        respCallback = nullptr;
    }
}

void INSWxApi_iOS::onReq(const INSWxBaseReq&  req)
{
    if(reqCallback) {
        reqCallback(req);
        reqCallback = nullptr;
    }
}



