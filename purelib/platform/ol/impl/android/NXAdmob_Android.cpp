#include "NxAdmob_Android.h"

#include "PluginManager.h"
#include "PluginUtils.h"
#include "cocos2d.h"

using namespace cocos2d::plugin;
using namespace cocos2d;

static ProtocolAds* s_plugin = nullptr;

NxAdmob_Android::~NxAdmob_Android(void)
{
	s_plugin = nullptr;
	PluginManager::getInstance()->unloadPlugin("AdsYoumi");
}

bool NxAdmob_Android::init(void)
{
	if(s_plugin)
		return true;
	s_plugin = dynamic_cast<ProtocolAds*>(PluginManager::getInstance()->loadPlugin("AdsYoumi"));
	//s_plugin->setListener(this);
	return true;
}

void NxAdmob_Android::showAdView(void)
{
	std::map<std::string,std::string> args;
	return s_plugin->showAds(args);
}

void NxAdmob_Android::hideAdView(void)
{
	std::map<std::string,std::string> args;
    s_plugin->hideAds(args);
}




