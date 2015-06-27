#include "CCGameCenter_Objc.h"
#include "CCGameCenter.h"

static CCGameCenter* s_sharedGameCenter = nullptr;

CCGameCenter* CCGameCenter::sharedGameCenter()
{
    if(s_sharedGameCenter != nullptr)
        return s_sharedGameCenter;
    s_sharedGameCenter = new CCGameCenter();
    return s_sharedGameCenter;
}

void CCGameCenter::purgeSharedGameCenter()
{
    if(s_sharedGameCenter != nullptr)
    {
        [CCGameCenter_Objc purgeSharedGameCenter];
    }
}

bool CCGameCenter::isGameCenterAvailable()
{
    return [[CCGameCenter_Objc sharedGameCenter] isGameCenterAvailable];
}

void CCGameCenter::authenticateLocalPlayer()
{
    [[CCGameCenter_Objc sharedGameCenter] authenticateLocalPlayer];
}

