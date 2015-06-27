#include "NxLocationManager_Android.h"
#include "PluginManager.h"
#include "PluginUtils.h"
#include "cocos2d.h"

using namespace cocos2d::plugin;
using namespace cocos2d;

static ProtocolLM* s_plugin = nullptr;

NxLocationManager_Android::~NxLocationManager_Android(void)
{
	s_plugin = nullptr;
	PluginManager::getInstance()->unloadPlugin("LMPlugin");
}

bool NxLocationManager_Android::init(void)
{
	s_plugin = dynamic_cast<ProtocolLM*>(PluginManager::getInstance()->loadPlugin("LMPlugin"));
	s_plugin->setListener(this);
	return true;
}

bool NxLocationManager_Android::isLocationAvailable(void) const
{
	return PluginUtils::callJavaBoolFuncWithName(s_plugin, "isLocationAvailable");
}

void NxLocationManager_Android::startLocate(const LocationUpdateCallback& callback)
{
	this->callback = callback;

	s_plugin->startLocate();
}

void NxLocationManager_Android::stopLocate(void)
{
	return PluginUtils::callJavaFunctionWithName(s_plugin, "stopLocate");
}

void NxLocationManager_Android::onLocateResult(int code, double latitude, double longitude)
{
	if(this->callback)
	{
		this->callback(kLocateSucceed == code, CCPointMake(latitude, longitude));
		this->callback = nullptr;
	}
}

