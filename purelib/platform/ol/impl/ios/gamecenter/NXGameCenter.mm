#include "CCGameCenter_Objc.h"
#include "NXGameCenter.h"

static NXGameCenter* s_sharedGameCenter = nullptr;

NXGameCenter* NXGameCenter::sharedGameCenter()
{
    if(s_sharedGameCenter != nullptr)
        return s_sharedGameCenter;
    s_sharedGameCenter = new NXGameCenter();
    return s_sharedGameCenter;
}

void NXGameCenter::purgeSharedGameCenter()
{
    if(s_sharedGameCenter != nullptr)
    {
        [CCGameCenter_Objc purgeSharedGameCenter];
    }
}

bool NXGameCenter::isGameCenterAvailable()
{
    return [[CCGameCenter_Objc sharedGameCenter] isGameCenterAvailable];
}

void NXGameCenter::authenticateLocalPlayer()
{
    [[CCGameCenter_Objc sharedGameCenter] authenticateLocalPlayer];
}

