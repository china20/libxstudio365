/****************************************************************************
Copyright (c) 2014 inewsoft corp.
http://www.inewsoft.com
****************************************************************************/
#ifndef _ONLINE_USER_H_
#define _ONLINE_USER_H_
#include <functional>
#include <string>
#include "ProtocolUser.h"
#include "plugin_configs.h"

typedef std::function<void(cocos2d::plugin::UserActionResultCode code, const char* msg)> UserActionCallback;

namespace inewsoft {
namespace ol {

class User : public cocos2d::plugin::UserActionListener
{
public:
    User(const char* name);
    virtual ~User();

    void init(void);

    virtual void setupDeveloperInfo(cocos2d::plugin::TUserDeveloperInfo& devInfo) = 0;

    void login(const UserActionCallback& cb = [](cocos2d::plugin::UserActionResultCode, const char*)->void{});
    void logout(const UserActionCallback& cb = [](cocos2d::plugin::UserActionResultCode, const char*)->void{});
    bool isLogined(void);

    virtual void onActionResult(cocos2d::plugin::ProtocolUser* pPlugin, cocos2d::plugin::UserActionResultCode code, const char* msg);

protected:
    std::string                    name;
    cocos2d::plugin::ProtocolUser* plugin;
    UserActionCallback             callback;
};

};
};


#endif // _INS_ONLINE_USER_H_
