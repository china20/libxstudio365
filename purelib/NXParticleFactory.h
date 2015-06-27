// Copyright (c) 2014 purelib. All rights reserved.
#ifndef _NXParticleFactory_H_
#define _NXParticleFactory_H_
#include "NXGameDefs.h"

namespace purelib {
class ParticleFactory
{
    ParticleFactory(void);
    ~ParticleFactory(void);
public:

    enum ParticleType {
        ParticleType_Fire,
        ParticleType_Fireworks,
        ParticleType_Sun,
        ParticleType_Galaxy,
        ParticleType_Flower,
        ParticleType_Meteor,
        ParticleType_Spiral,
        ParticleType_Explosion,
        ParticleType_Smoke,
        ParticleType_Snow,
        ParticleType_Rain,
    };
    /* caller need release the object */
    static cocos2d::CCParticleSystem* spawn(ParticleType type, const char* image);
    /* caller need release the object, use by Permanent effects */
    static cocos2d::CCParticleSystem* spawnFromFile(const char* plist);
    /* caller need release the object, use by Instantaneous effects */
    static cocos2d::CCParticleSystem* spawnFromFile(const char* plist, float duration, bool autoRemoveFromParent = true, bool blendAdditive = false);
};

};

#endif

