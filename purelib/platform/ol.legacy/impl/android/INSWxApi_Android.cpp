#include "NxWxApi_Android.h"

#include "PluginManager.h"
#include "PluginUtils.h"
#include "cocos2d.h"

using namespace cocos2d::plugin;
using namespace cocos2d;

static ProtocolWechat* s_plugin = nullptr;

NxWxApi_Android::~NxWxApi_Android(void)
{
	s_plugin = nullptr;
	PluginManager::getInstance()->unloadPlugin("WechatPlugin");
}

bool NxWxApi_Android::init(void)
{
	s_plugin = dynamic_cast<ProtocolWechat*>(PluginManager::getInstance()->loadPlugin("WechatPlugin"));
	s_plugin->setListener(this);
	return true;
}

bool NxWxApi_Android::isWxAppInstalled(void)
{
	return PluginUtils::callJavaBoolFuncWithName(s_plugin, "isWXAppInstalled");
}

bool NxWxApi_Android::isWxAppSupportApi(void)
{
	return PluginUtils::callJavaBoolFuncWithName(s_plugin, "isWXAppSupportAPI");
}

bool NxWxApi_Android::openWxApp(void)
{
	return PluginUtils::callJavaBoolFuncWithName(s_plugin, "openWXApp");
}

void NxWxApi_Android::changeScene(enum CCWxSceneType type)
{
	PluginUtils::callJavaFunctionWithName_oneParam(s_plugin, "changeScene", "I", (int)type);
}

void NxWxApi_Android::sendTextReq(const char* text, const NxWxRespCallback& callback)
{
    this->respCallback = callback;
    s_plugin->sendTextReq(text);
}

void NxWxApi_Android::sendLinkReq(const char* title, const char* description, const char* url, const NxWxRespCallback& callback)
{
	this->respCallback = callback;
	s_plugin->sendLinkReq(title, description, url);
}

void NxWxApi_Android::onResp(int code, const char* msg)
{
	if(this->respCallback) {
		CCWxBaseResp resp;
		resp.errCode = code;
		resp.errStr = msg;
		this->respCallback(resp);
		this->respCallback = nullptr;
	}
}

void NxWxApi_Android::onReq(int code, const char* msg)
{ // no need implement currently
}


