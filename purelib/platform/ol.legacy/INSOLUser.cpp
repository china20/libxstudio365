/****************************************************************************
Copyright (c) 2014 inewsoft corp.
****************************************************************************/
#include "INSOLUser.h"
#include "PluginManager.h"
#include "cocos2d.h"


using namespace cocos2d::plugin;
using namespace cocos2d;

namespace inewsoft {
namespace ol {

User::User(const char* name)
: plugin(nullptr),
  name(name)
{

}

User::~User()
{
	PluginManager::getInstance()->unloadPlugin(this->name.c_str());
    this->plugin = NULL;
}

void User::init(void)
{
    if(this->plugin != nullptr)
        return;

    this->plugin = dynamic_cast<ProtocolUser*>(PluginManager::getInstance()->loadPlugin(this->name.c_str()));

    cocos2d::plugin::TUserDeveloperInfo devInfo;

    this->setupDeveloperInfo(devInfo);

    this->plugin->setDebugMode(true);
    //this->plugin->configDeveloperInfo(devInfo);
    this->plugin->setActionListener(this);
}

void User::login(const UserActionCallback& cb)
{
    this->callback = cb;
    this->plugin->login();
}

void User::logout(const UserActionCallback& cb)
{
    this->callback = cb;
    this->plugin->logout();
}

bool User::isLogined(void)
{
    this->isLogined();
}

void User::onActionResult(cocos2d::plugin::ProtocolUser* pPlugin, cocos2d::plugin::UserActionResultCode code, const char* msg)
{
	char userStatus[1024] = { 0 };
	switch (code)
	{
	case kLoginSucceed:
	case kLoginFailed:
		sprintf(userStatus, "User of \"%s\" login %s", pPlugin->getPluginName(), (code == kLoginSucceed)? "Successed" : "Failed");
		break;
	case kLogoutSucceed:
		sprintf(userStatus, "User of \"%s\" logout", pPlugin->getPluginName());
		break;
	default:
		break;
	}
	CCMessageBox(msg, userStatus);

	// get session ID
	std::string sessionID = pPlugin->getSessionID();
	CCLOG("User Session ID of plugin %s is : %s", pPlugin->getPluginName(), sessionID.c_str());

	std::string strStatus = pPlugin->isLogined() ? "online" : "offline";
	CCLOG("User status of plugin %s is : %s", pPlugin->getPluginName(), strStatus.c_str());

	this->callback(code, msg);
}

};
};

